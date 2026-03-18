#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <atomic>
#include <mutex>
#include <queue>
#include <deque>
#include <thread>
#include <optional>
#include <condition_variable>
#include "mobdebug_types.h"

namespace boost::asio
{
    class io_context;
}

namespace MobDebugCpp
{
    struct Breakpoint;
    struct StackFrame;
    struct Variable;
    struct PauseEvent;
    enum class DebuggerState;

    using OnConnectedCallback    = std::function<void()>;
    using OnDisconnectedCallback = std::function<void()>;
    using OnPausedCallback       = std::function<void(const PauseEvent&)>;
    using OnResumedCallback      = std::function<void()>;
    using OnOutputCallback       = std::function<void(const std::string& msg)>;
    using OnErrorCallback        = std::function<void(const std::string& err)>;

    using EvalCallback  = std::function<void(bool ok, const std::string& result)>;
    using StackCallback = std::function<void(bool ok, const std::string& result)>;

    class MobDebugSession;

    class MobDebugServer
    {
    public:
        explicit MobDebugServer();
        ~MobDebugServer();

        MobDebugServer(const MobDebugServer&)            = delete;
        MobDebugServer& operator=(const MobDebugServer&) = delete;

        bool start(uint16_t port = 8172);
        void stop();
        void poll();

        void set_breakpoints(const std::string& file,
                             const std::unordered_set<int>& lines);
        void clear_breakpoints(const std::string& file);
        void clear_all_breakpoints();
        void sync_breakpoints(
            const std::unordered_map<std::string,
                                     std::unordered_set<int>>& all_breakpoints);

        void cmd_run();
        void cmd_step_over();
        void cmd_step_into();
        void cmd_step_out();
        void cmd_pause();
        void cmd_exit();

        void eval(const std::string& expr, EvalCallback cb);
        void exec(const std::string& expr, EvalCallback cb);
        void get_stack(StackCallback cb);

        DebuggerState state() const;
        bool is_connected() const;
        bool is_paused() const;

        void on_connected   (OnConnectedCallback    cb) { cb_connected_    = std::move(cb); }
        void on_disconnected(OnDisconnectedCallback cb) { cb_disconnected_ = std::move(cb); }
        void on_paused      (OnPausedCallback       cb) { cb_paused_       = std::move(cb); }
        void on_resumed     (OnResumedCallback      cb) { cb_resumed_      = std::move(cb); }
        void on_output      (OnOutputCallback       cb) { cb_output_       = std::move(cb); }
        void on_error       (OnErrorCallback        cb) { cb_error_        = std::move(cb); }

    private:
        struct Event
        {
            enum class Type
            {
                Connected,
                Disconnected,
                Paused,
                Resumed,
                Output,
                Error,
                EvalResult,
                StackResult,
            };
            Type type;

            PauseEvent pause_event;
            std::string message;

            bool eval_ok = false;
            std::string eval_result;
            EvalCallback eval_cb;

            bool stack_ok = false;
            std::string stack_result;
            StackCallback stack_cb;
        };

        void push_event(Event ev);

        void network_thread_func();
        void do_accept();

        std::unique_ptr<boost::asio::io_context> io_ctx_;
        std::unique_ptr<class TcpAcceptor>       acceptor_;
        std::shared_ptr<MobDebugSession>         session_;
        void*                                    strand_;

        std::thread network_thread_;
        std::atomic<DebuggerState> state_{ DebuggerState::Disconnected };

        std::mutex bp_mutex_;
        std::unordered_map<std::string, std::unordered_set<int>> breakpoints_;

        std::mutex event_mutex_;
        std::queue<Event> event_queue_;

        OnConnectedCallback    cb_connected_;
        OnDisconnectedCallback cb_disconnected_;
        OnPausedCallback       cb_paused_;
        OnResumedCallback      cb_resumed_;
        OnOutputCallback       cb_output_;
        OnErrorCallback        cb_error_;

        uint16_t listen_port_ = 8172;
        std::atomic_bool running_{ false };

        friend class MobDebugSession;
    };
}
