#include "mobdebug_server.h"
#include "mobdebug_types.h"

#ifdef _WIN32
#define _WIN32_WINNT 0x0601
#endif

#include <boost/asio.hpp>
#include <boost/regex.hpp>

#include <sstream>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

namespace MobDebugCpp
{
    namespace
    {
        std::string normalize_mob_path(const std::string& raw)
        {
            std::string p = raw;
            if (!p.empty() && p[0] == '@')
                p = p.substr(1);
            std::replace(p.begin(), p.end(), '\\', '/');
            if (p.size() >= 2 && p[0] == '.' && p[1] == '/')
                p = p.substr(2);
            return p;
        }
    }

    // =======================================================================
    // CommandEntry —— 指令队列中的每一项
    // =======================================================================
    struct CommandEntry
    {
        // 指令类型
        enum class Kind
        {
            Fire, // 只发送、无需等待响应（SETB/DELB/EXIT 等）
            Eval, // 需要等待 200 OK + 数据
            Exec, // 需要等待 200 OK + 数据
            Stack, // 需要等待 200 OK + 数据
            Control, // RUN/STEP/OVER/OUT —— 等待 200 OK，但响应是 202 Paused
        };

        Kind kind = Kind::Fire;
        std::string raw_cmd; // 发往 Lua 的原始命令字符串

        EvalCallback eval_cb; // Eval / Exec 回调
        StackCallback stack_cb; // Stack 回调
    };

    // =======================================================================
    // MobDebugSession
    // =======================================================================
    class MobDebugSession : public std::enable_shared_from_this<MobDebugSession>
    {
    public:
        explicit MobDebugSession(tcp::socket socket, MobDebugServer& server)
            : socket_(std::move(socket))
              , server_(server)
              , strand_(asio::make_strand(socket_.get_executor()))
        {
        }

        void start()
        {
            // 将已有断点作为 Fire 指令预填到队列（无需等待响应）
            {
                std::lock_guard<std::mutex> lk(server_.bp_mutex_);
                for (auto& [file, lines] : server_.breakpoints_)
                {
                    for (int line : lines)
                    {
                        CommandEntry e;
                        e.kind = CommandEntry::Kind::Fire;
                        e.raw_cmd = "SETB " + file + " " + std::to_string(line);
                        cmd_queue_.push_back(std::move(e));
                    }
                }
            }
            do_read_line();
            try_send_next(); // 开始发送队列中的第一条
        }

        void stop()
        {
            boost::system::error_code ec;
            socket_.shutdown(tcp::socket::shutdown_both, ec);
            socket_.close(ec);
        }

        // ── 外部投递指令（线程安全，投递到 strand） ──────────────────────
        void enqueue(CommandEntry entry)
        {
            auto self = shared_from_this();
            asio::post(strand_, [self, entry = std::move(entry)]() mutable
            {
                self->cmd_queue_.push_back(std::move(entry));
                // 如果当前没有在途指令，立即发送
                if (!self->waiting_response_)
                    self->try_send_next();
            });
        }

    private:
        // ── 尝试发送队列头部指令 ─────────────────────────────────────────
        // 只在 strand 内调用
        void try_send_next()
        {
            if (cmd_queue_.empty() || waiting_response_)
                return;

            CommandEntry& front = cmd_queue_.front();

            // Fire 类型：直接发送，不需要等响应，立刻继续下一条
            if (front.kind == CommandEntry::Kind::Fire)
            {
                do_write_cmd(front.raw_cmd);
                cmd_queue_.pop_front();
                // Fire 不等响应，继续发送下一条
                auto self = shared_from_this();
                asio::post(strand_, [self]() mutable
                {
                    self->try_send_next();
                });
                return;
            }

            // 其他类型：发送后进入等待状态
            waiting_response_ = true;
            current_entry_ = std::move(front);
            cmd_queue_.pop_front();
            do_write_cmd(current_entry_.raw_cmd);
        }

        // ── 底层写一条命令（自动追加 \n） ───────────────────────────────
        void do_write_cmd(const std::string& cmd)
        {
            auto self = shared_from_this();
            std::string data = cmd + "\n";
            write_queue_.push_back(std::move(data));
            if (write_queue_.size() == 1) // 当前没有写操作在进行
                do_write();
        }

        // ── 异步读一行 ───────────────────────────────────────────────────
        void do_read_line()
        {
            auto self = shared_from_this();
            asio::async_read_until(
                socket_, read_buf_, '\n',
                asio::bind_executor(
                    strand_,
                    [self](boost::system::error_code ec, std::size_t line_size)
                    {
                        if (ec)
                        {
                            self->on_network_error(ec.message());
                            return;
                        }
                        std::string line(
                            (char*)self->read_buf_.data().data(), line_size);
                        self->read_buf_.consume(line_size);
                        while (!line.empty() &&
                            (line.back() == '\r' || line.back() == '\n'))
                            line.pop_back();

                        self->on_line_received(line);

                        // 如果还有多行数据要读
                        if (self->pending_multiline_count_ > 0)
                            self->do_read_bytes();
                        else
                            self->do_read_line();
                    }));
        }

        void do_read_bytes()
        {
            auto self = shared_from_this();
            asio::async_read(
                socket_, read_buf_,
                asio::transfer_exactly(pending_multiline_count_),
                asio::bind_executor(
                    strand_,
                    [self](boost::system::error_code ec, std::size_t read_size)
                    {
                        if (ec)
                        {
                            self->on_network_error(ec.message());
                            return;
                        }
                        self->pending_multiline_buf_ =
                            std::string((char*)self->read_buf_.data().data(), read_size);
                        self->read_buf_.consume(read_size);
                        self->pending_multiline_count_ = 0;
                        self->flush_pending_multiline();
                        self->do_read_line();
                    }));
        }

        // ── 处理收到的一行 ───────────────────────────────────────────────
        void on_line_received(const std::string& line)
        {
            if (line.empty()) return;

            // ── 202 Paused ──────────────────────────────────────────────
            if (line.rfind("202 Paused", 0) == 0)
            {
                handle_paused(line);
                // Control 类指令（RUN/STEP/OVER/OUT）在收到 202 Paused
                // 时视为"完成"，解除等待
                if (waiting_response_ &&
                    current_entry_.kind == CommandEntry::Kind::Control)
                {
                    finish_current_entry();
                }
                return;
            }

            // ── 200 OK ──────────────────────────────────────────────────
            if (line.rfind("200 OK", 0) == 0)
            {
                handle_ok_response(line);
                return;
            }

            // ── 401 Error ───────────────────────────────────────────────
            if (line.rfind("401 Error", 0) == 0)
            {
                boost::regex pattern(R"(\d+$)");
                boost::smatch match;
                if (boost::regex_search(line, match, pattern))
                {
                    std::istringstream is(match[0].str());
                    is >> pending_multiline_count_;
                    pending_multiline_type_ = PendingType::Error;
                    pending_multiline_buf_.clear();
                }
                return;
            }

            // ── 其他：作为输出推送 ──────────────────────────────────────
            MobDebugServer::Event ev;
            ev.type = MobDebugServer::Event::Type::Output;
            ev.message = line;
            server_.push_event(std::move(ev));
        }

        // ── 处理 202 Paused ──────────────────────────────────────────────
        void handle_paused(const std::string& line)
        {
            server_.state_.store(DebuggerState::Paused);

            PauseEvent pe;
            pe.reason = PauseReason::Breakpoint;

            std::istringstream ss(line);
            std::string code, word, file;
            int ln = 0;
            ss >> code >> word >> file >> ln;

            pe.file = normalize_mob_path(file);
            pe.line = ln;

            MobDebugServer::Event ev;
            ev.type = MobDebugServer::Event::Type::Paused;
            ev.pause_event = pe;
            server_.push_event(std::move(ev));
        }

        // ── 处理 200 OK ──────────────────────────────────────────────────
        void handle_ok_response(const std::string& line)
        {
            if (!waiting_response_)
            {
                // 没有在途指令，当作 Running 状态推进
                server_.state_.store(DebuggerState::Running);
                MobDebugServer::Event ev;
                ev.type = MobDebugServer::Event::Type::Resumed;
                server_.push_event(std::move(ev));
                return;
            }

            std::istringstream ss(line);
            std::string code, ok;
            int n = 0;
            ss >> code >> ok >> n;

            switch (current_entry_.kind)
            {
            case CommandEntry::Kind::Control:
                // RUN/STEP 等：200 OK 表示命令已接受，
                // 真正的"完成"是收到 202 Paused；
                // 但如果 n==0 且没有后续数据，也直接完成
                if (n == 0)
                {
                    // 标记为 Running，等待 202 Paused
                    server_.state_.store(DebuggerState::Running);
                    MobDebugServer::Event ev;
                    ev.type = MobDebugServer::Event::Type::Resumed;
                    server_.push_event(std::move(ev));
                    // FIXME: AI这么做真的合理吗
                    //// Control 指令等 202 Paused 才真正结束，这里不 finish
                    // finish_current_entry();
                }
                break;

            case CommandEntry::Kind::Eval:
            case CommandEntry::Kind::Exec:
                if (n == 0)
                {
                    // 无数据，直接回调
                    dispatch_eval_result(true, "");
                    finish_current_entry();
                }
                else
                {
                    // 有 n 字节数据要读
                    pending_multiline_count_ = n;
                    pending_multiline_type_ = PendingType::QueryResult;
                    pending_multiline_buf_.clear();
                }
                break;

            case CommandEntry::Kind::Stack:
                {
                    // STACK 响应：200 OK <json/text>
                    std::string data = line.size() > 7 ? line.substr(7) : "";
                    dispatch_stack_result(true, data);
                    finish_current_entry();
                }
                break;

            default:
                finish_current_entry();
                break;
            }
        }

        // ── 多行数据收集完毕 ─────────────────────────────────────────────
        void flush_pending_multiline()
        {
            if (pending_multiline_type_ == PendingType::Error)
            {
                if (waiting_response_)
                {
                    switch (current_entry_.kind)
                    {
                    case CommandEntry::Kind::Eval:
                    case CommandEntry::Kind::Exec:
                        dispatch_eval_result(false, pending_multiline_buf_);
                        break;
                    case CommandEntry::Kind::Stack:
                        dispatch_stack_result(false, pending_multiline_buf_);
                        break;
                    default:
                        {
                            MobDebugServer::Event ev;
                            ev.type = MobDebugServer::Event::Type::Error;
                            ev.message = pending_multiline_buf_;
                            server_.push_event(std::move(ev));
                        }
                        break;
                    }
                    finish_current_entry();
                }
                else
                {
                    MobDebugServer::Event ev;
                    ev.type = MobDebugServer::Event::Type::Error;
                    ev.message = pending_multiline_buf_;
                    server_.push_event(std::move(ev));
                }
            }
            else if (pending_multiline_type_ == PendingType::QueryResult)
            {
                if (waiting_response_)
                {
                    switch (current_entry_.kind)
                    {
                    case CommandEntry::Kind::Eval:
                    case CommandEntry::Kind::Exec:
                        dispatch_eval_result(true, pending_multiline_buf_);
                        break;
                    case CommandEntry::Kind::Stack:
                        dispatch_stack_result(true, pending_multiline_buf_);
                        break;
                    default:
                        break;
                    }
                    finish_current_entry();
                }
            }

            pending_multiline_buf_.clear();
            pending_multiline_count_ = 0;
            pending_multiline_type_ = PendingType::None;
        }

        // ── 当前指令完成，解除等待，发送下一条 ──────────────────────────
        // 只在 strand 内调用
        void finish_current_entry()
        {
            waiting_response_ = false;
            current_entry_ = {};
            try_send_next();
        }

        // ── 结果分发 ─────────────────────────────────────────────────────
        void dispatch_eval_result(bool ok, const std::string& data)
        {
            MobDebugServer::Event ev;
            ev.type = MobDebugServer::Event::Type::EvalResult;
            ev.eval_ok = ok;
            ev.eval_result = data;
            ev.eval_cb = current_entry_.eval_cb;
            server_.push_event(std::move(ev));
        }

        void dispatch_stack_result(bool ok, const std::string& data)
        {
            MobDebugServer::Event ev;
            ev.type = MobDebugServer::Event::Type::StackResult;
            ev.stack_ok = ok;
            ev.stack_result = data;
            ev.stack_cb = current_entry_.stack_cb;
            server_.push_event(std::move(ev));
        }

        // ── 异步写队列 ───────────────────────────────────────────────────
        void do_write()
        {
            if (write_queue_.empty()) return;
            auto self = shared_from_this();
            asio::async_write(
                socket_,
                asio::buffer(write_queue_.front()),
                asio::bind_executor(
                    strand_,
                    [self](boost::system::error_code ec, std::size_t)
                    {
                        if (ec)
                        {
                            self->on_network_error(ec.message());
                            return;
                        }
                        self->write_queue_.pop_front();
                        if (!self->write_queue_.empty())
                            self->do_write();
                    }));
        }

        void on_network_error(const std::string& msg)
        {
            // 如果有等待回调的指令，先给它一个错误回调
            if (waiting_response_)
            {
                switch (current_entry_.kind)
                {
                case CommandEntry::Kind::Eval:
                case CommandEntry::Kind::Exec:
                    dispatch_eval_result(false, "disconnected: " + msg);
                    break;
                case CommandEntry::Kind::Stack:
                    dispatch_stack_result(false, "disconnected: " + msg);
                    break;
                default:
                    break;
                }
                // 清空剩余队列中的回调
                for (auto& e : cmd_queue_)
                {
                    if (e.kind == CommandEntry::Kind::Eval ||
                        e.kind == CommandEntry::Kind::Exec)
                    {
                        if (e.eval_cb)
                            e.eval_cb(false, "disconnected");
                    }
                    else if (e.kind == CommandEntry::Kind::Stack)
                    {
                        if (e.stack_cb)
                            e.stack_cb(false, "disconnected");
                    }
                }
                cmd_queue_.clear();
                waiting_response_ = false;
            }

            MobDebugServer::Event ev;
            ev.type = MobDebugServer::Event::Type::Disconnected;
            ev.message = msg;
            server_.push_event(std::move(ev));
        }

    private:
        tcp::socket socket_;
        MobDebugServer& server_;
        asio::streambuf read_buf_;
        std::deque<std::string> write_queue_;
        asio::strand<tcp::socket::executor_type> strand_;

        // ── 指令串行化核心状态（只在 strand 内访问） ────────────────────
        std::deque<CommandEntry> cmd_queue_; // 待发送队列
        bool waiting_response_ = false; // 是否有指令在途
        CommandEntry current_entry_; // 当前在途指令

        // ── 多行数据收集 ─────────────────────────────────────────────────
        enum class PendingType { None, Error, QueryResult };

        PendingType pending_multiline_type_ = PendingType::None;
        int pending_multiline_count_ = 0;
        std::string pending_multiline_buf_;
    };

    // =======================================================================
    // TcpAcceptor Pimpl
    // =======================================================================
    class TcpAcceptor
    {
    public:
        TcpAcceptor(asio::io_context& ctx, uint16_t port)
            : acceptor_(ctx, tcp::endpoint(tcp::v4(), port))
        {
            acceptor_.set_option(tcp::acceptor::reuse_address(true));
        }

        tcp::acceptor acceptor_;
    };

    // =======================================================================
    // MobDebugServer 实现
    // =======================================================================
    MobDebugServer::MobDebugServer()
        : io_ctx_(std::make_unique<asio::io_context>())
          , strand_(new asio::strand<asio::io_context::executor_type>(
              asio::make_strand(*io_ctx_)))
    {
    }

    MobDebugServer::~MobDebugServer()
    {
        stop();
        delete (asio::strand<asio::io_context::executor_type>*)strand_;
    }

    bool MobDebugServer::start(uint16_t port)
    {
        if (running_) return false;
        listen_port_ = port;

        try
        {
            acceptor_ = std::make_unique<TcpAcceptor>(*io_ctx_, port);
        }
        catch (const std::exception& e)
        {
            if (cb_error_) cb_error_(std::string("监听失败: ") + e.what());
            return false;
        }

        running_ = true;
        do_accept();
        network_thread_ = std::thread(&MobDebugServer::network_thread_func, this);
        return true;
    }

    void MobDebugServer::stop()
    {
        if (!running_) return;
        running_ = false;

        io_ctx_->stop();

        if (session_)
        {
            session_->stop();
            session_.reset();
        }

        if (network_thread_.joinable())
            network_thread_.join();

        io_ctx_->restart();
        state_.store(DebuggerState::Disconnected);
    }

    void MobDebugServer::network_thread_func()
    {
        try { io_ctx_->run(); }
        catch (const std::exception& e)
        {
            push_event({ MobDebugServer::Event::Type::Error, {}, e.what() });
        }
    }

    void MobDebugServer::do_accept()
    {
        acceptor_->acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket)
            {
                if (!running_) return;
                if (!ec)
                {
                    if (session_)
                    {
                        session_->stop();
                        session_.reset();
                    }
                    session_ = std::make_shared<MobDebugSession>(
                        std::move(socket), *this);
                    session_->start();

                    state_.store(DebuggerState::Connected);
                    push_event({ Event::Type::Connected });
                }
                do_accept();
            });
    }

    // ── poll ─────────────────────────────────────────────────────────────────
    void MobDebugServer::poll()
    {
        std::queue<Event> local;
        {
            std::lock_guard<std::mutex> lk(event_mutex_);
            std::swap(local, event_queue_);
        }

        while (!local.empty())
        {
            Event& ev = local.front();
            switch (ev.type)
            {
            case Event::Type::Connected:
                if (cb_connected_) cb_connected_();
                break;
            case Event::Type::Disconnected:
                state_.store(DebuggerState::Disconnected);
                session_.reset();
                if (cb_disconnected_) cb_disconnected_();
                break;
            case Event::Type::Paused:
                if (cb_paused_) cb_paused_(ev.pause_event);
                break;
            case Event::Type::Resumed:
                if (cb_resumed_) cb_resumed_();
                break;
            case Event::Type::Output:
                if (cb_output_) cb_output_(ev.message);
                break;
            case Event::Type::Error:
                if (cb_error_) cb_error_(ev.message);
                break;
            case Event::Type::EvalResult:
                if (ev.eval_cb) ev.eval_cb(ev.eval_ok, ev.eval_result);
                break;
            case Event::Type::StackResult:
                if (ev.stack_cb) ev.stack_cb(ev.stack_ok, ev.stack_result);
                break;
            }
            local.pop();
        }
    }

    void MobDebugServer::push_event(Event ev)
    {
        std::lock_guard<std::mutex> lk(event_mutex_);
        event_queue_.push(std::move(ev));
    }

    // ── 断点管理 ─────────────────────────────────────────────────────────────
    void MobDebugServer::set_breakpoints(const std::string& file,
                                         const std::unordered_set<int>& lines)
    {
        std::unordered_set<int> old_lines;
        {
            std::lock_guard<std::mutex> lk(bp_mutex_);
            old_lines = breakpoints_[file];
            breakpoints_[file] = lines;
        }
        if (!session_) return;

        // SETB/DELB 都是 Fire 类型，不需要等待响应
        for (int ln : old_lines)
        {
            if (lines.find(ln) == lines.end())
            {
                CommandEntry e;
                e.kind = CommandEntry::Kind::Fire;
                e.raw_cmd = "DELB " + file + " " + std::to_string(ln);
                session_->enqueue(std::move(e));
            }
        }
        for (int ln : lines)
        {
            if (old_lines.find(ln) == old_lines.end())
            {
                CommandEntry e;
                e.kind = CommandEntry::Kind::Fire;
                e.raw_cmd = "SETB " + file + " " + std::to_string(ln);
                session_->enqueue(std::move(e));
            }
        }
    }

    void MobDebugServer::clear_breakpoints(const std::string& file)
    {
        std::unordered_set<int> old_lines;
        {
            std::lock_guard<std::mutex> lk(bp_mutex_);
            auto it = breakpoints_.find(file);
            if (it != breakpoints_.end())
            {
                old_lines = it->second;
                breakpoints_.erase(it);
            }
        }
        if (!session_) return;
        for (int ln : old_lines)
        {
            CommandEntry e;
            e.kind = CommandEntry::Kind::Fire;
            e.raw_cmd = "DELB " + file + " " + std::to_string(ln);
            session_->enqueue(std::move(e));
        }
    }

    void MobDebugServer::clear_all_breakpoints()
    {
        std::unordered_map<std::string, std::unordered_set<int>> old;
        {
            std::lock_guard<std::mutex> lk(bp_mutex_);
            std::swap(old, breakpoints_);
        }
        if (!session_) return;
        for (auto& [file, lines] : old)
        {
            for (int ln : lines)
            {
                CommandEntry e;
                e.kind = CommandEntry::Kind::Fire;
                e.raw_cmd = "DELB " + file + " " + std::to_string(ln);
                session_->enqueue(std::move(e));
            }
        }
    }

    void MobDebugServer::sync_breakpoints(
        const std::unordered_map<std::string, std::unordered_set<int>>& all)
    {
        for (auto& [file, lines] : all)
            set_breakpoints(file, lines);
    }

    // ── 执行控制 ─────────────────────────────────────────────────────────────
    // 辅助：投递一条 Control 类指令
    static void enqueue_control(MobDebugSession* s, const std::string& cmd)
    {
        if (!s) return;
        CommandEntry e;
        e.kind = CommandEntry::Kind::Control;
        e.raw_cmd = cmd;
        s->enqueue(std::move(e));
    }

    void MobDebugServer::cmd_run()
    {
        if (!session_) return;
        CommandEntry e;
        e.kind = CommandEntry::Kind::Fire;
        e.raw_cmd = "RUN";
        session_->enqueue(std::move(e));
    }
    void MobDebugServer::cmd_step_over() { enqueue_control(session_.get(), "OVER"); }
    void MobDebugServer::cmd_step_into() { enqueue_control(session_.get(), "STEP"); }
    void MobDebugServer::cmd_step_out() { enqueue_control(session_.get(), "OUT"); }
    void MobDebugServer::cmd_pause() { enqueue_control(session_.get(), "SUSPEND"); }

    void MobDebugServer::cmd_exit()
    {
        if (!session_) return;
        CommandEntry e;
        e.kind = CommandEntry::Kind::Fire; // EXIT 不期待响应
        e.raw_cmd = "EXIT";
        session_->enqueue(std::move(e));
    }

    // ── eval ─────────────────────────────────────────────────────────────────
    void MobDebugServer::eval(const std::string& expr, EvalCallback cb)
    {
        if (!session_ || state_ != DebuggerState::Paused)
        {
            if (cb) cb(false, "not paused");
            return;
        }
        CommandEntry e;
        e.kind = CommandEntry::Kind::Eval;
        e.raw_cmd = "EXEC return(" + expr + ")";
        e.eval_cb = std::move(cb);
        session_->enqueue(std::move(e));
    }

    // ── exec ─────────────────────────────────────────────────────────────────
    void MobDebugServer::exec(const std::string& expr, EvalCallback cb)
    {
        if (!session_ || state_ != DebuggerState::Paused)
        {
            if (cb) cb(false, "not paused");
            return;
        }
        CommandEntry e;
        e.kind = CommandEntry::Kind::Exec;
        e.raw_cmd = "EXEC " + expr;
        e.eval_cb = std::move(cb);
        session_->enqueue(std::move(e));
    }

    // ── get_stack ─────────────────────────────────────────────────────────────
    void MobDebugServer::get_stack(StackCallback cb)
    {
        if (!session_ || state_ != DebuggerState::Paused)
        {
            if (cb) cb(false, "not paused");
            return;
        }
        CommandEntry e;
        e.kind = CommandEntry::Kind::Stack;
        e.raw_cmd = "STACK";
        e.stack_cb = std::move(cb);
        session_->enqueue(std::move(e));
    }

    // ── 状态查询 ─────────────────────────────────────────────────────────────
    DebuggerState MobDebugServer::state() const { return state_.load(); }
    bool MobDebugServer::is_connected() const { return state_.load() != DebuggerState::Disconnected; }
    bool MobDebugServer::is_paused() const { return state_.load() == DebuggerState::Paused; }
} // namespace MobDebugCpp
