#include <any>
#include <iostream>
#include <atomic>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <mobdebug_server.h>

using namespace std;

void show_options();

int main()
{
    show_options();

    MobDebugCpp::MobDebugServer server;
    server.on_connected([&]
    {
        cout << "Debugger connected...\n";
        server.cmd_run();
    });
    server.on_disconnected([]
    {
        cout << "Debugger disconnected...\n";
    });
    server.on_output([](const string& msg)
    {
        cout << "[Lua] " << msg << "\n";
    });
    server.on_paused([](const MobDebugCpp::PauseEvent& pauseEvent)
    {
        cout << "[Lua] Paused at " << pauseEvent.file << ":" << pauseEvent.line << "\n";
        string reason = "Unknown";
        switch (pauseEvent.reason)
        {
        case MobDebugCpp::PauseReason::Breakpoint:
            reason = "Breakpoint";
            break;
        case MobDebugCpp::PauseReason::Exception:
            reason = "Exception";
            break;
        case MobDebugCpp::PauseReason::Step:
            reason = "Step";
            break;
        default:
            break;
        }
        cout << "[Lua] Pause Reason: " << reason << "\n";
    });
    server.on_resumed([] {
        cout << "[Lua] Resumed...\n";
    });
    server.start();

    std::queue<std::any> cmd_queue;
    std::mutex mtx_queue;
    std::atomic_bool running = true;
    std::thread([&]()
    {
        unordered_map<string, unordered_set<int>> breakpoints;
        while (running)
        {
            {
                std::lock_guard lock(mtx_queue);
                while (!cmd_queue.empty())
                {
                    const int cmd = std::any_cast<int>(cmd_queue.front());
                    cmd_queue.pop();
                    switch (cmd)
                    {
                    case 1:
                        server.cmd_run();
                        break;
                    case 2:
                        server.cmd_step_over();
                        break;
                    case 3:
                        server.cmd_step_into();
                        break;
                    case 4:
                        server.cmd_step_out();
                        break;
                    case 5:
                        server.cmd_pause();
                        break;
                    case 6:
                        {
                            string file = any_cast<string>(cmd_queue.front());
                            cmd_queue.pop();
                            int line = any_cast<int>(cmd_queue.front());
                            cmd_queue.pop();
                            auto found_file = breakpoints.find(file);
                            if (found_file == breakpoints.end())
                            {
                                found_file = breakpoints.insert({ file, {} }).first;
                            }
                            found_file->second.insert(line);
                            server.set_breakpoints(file, found_file->second);
                        }
                        break;
                    case 7:
                        {
                            string file = any_cast<string>(cmd_queue.front());
                            cmd_queue.pop();
                            int line = any_cast<int>(cmd_queue.front());
                            cmd_queue.pop();
                            auto found_file = breakpoints.find(file);
                            if (found_file == breakpoints.end())
                            {
                                break;
                            }
                            auto& lines = found_file->second;
                            auto found_line = lines.find(line);
                            if (found_line != lines.end())
                            {
                                lines.erase(found_line);
                            }
                            server.clear_breakpoints(file);
                            server.set_breakpoints(file, lines);
                        }
                        break;
                    case 8:
                        {
                            string eval = any_cast<string>(cmd_queue.front());
                            cmd_queue.pop();
                            server.eval(eval, [](bool ok, const string& msg)
                            {
                                cout << "[Lua Eval] ok = " << (ok ? "true" : "false") << "\n"
                                     << "[Lua Eval] msg: " << msg << "\n";
                            });
                        }
                        break;
                    case 9:
                        server.get_stack([](bool ok, const string& msg)
                        {
                            cout << "[Lua Stack] ok = " << (ok ? "true" : "false") << "\n"
                                 << "[Lua Stack] stack info write into `stack.lua`.\n";

                            FILE* fp = fopen("stack.lua", "wb");
                            fwrite(msg.c_str(), 1, msg.size(), fp);
                            fclose(fp);
                        });
                        break;
                    case 10:
                        {
                            const MobDebugCpp::DebuggerState state = server.state();
                            cout << "Debugger state: ";
                            switch (state)
                            {
                            case MobDebugCpp::DebuggerState::Connected:
                                cout << "Connected";
                                break;
                            case MobDebugCpp::DebuggerState::Disconnected:
                                cout << "Disconnected";
                                break;
                            case MobDebugCpp::DebuggerState::Paused:
                                cout << "Paused";
                                break;
                            case MobDebugCpp::DebuggerState::Running:
                                cout << "Running";
                                break;
                            }
                            cout << "\n";
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
            server.poll();
        }
    }).detach();

    while (true)
    {
        int cmd;
        cout << ">> ";
        cin >> cmd;
        if (cmd == 0)
        {
            running = false;
            break;
        }
        if (cmd == 11)
        {
            show_options();
        }
        {
            std::lock_guard lock(mtx_queue);
            cmd_queue.emplace(cmd);
            if (cmd == 6)
            {
                string file;
                int line = 0;
                cin >> file >> line;
                cmd_queue.emplace(file);
                cmd_queue.emplace(line);
            }
            else if (cmd == 7)
            {
                string file;
                int line = 0;
                cin >> file >> line;
                cmd_queue.emplace(file);
                cmd_queue.emplace(line);
            }
            else if (cmd == 8)
            {
                static char buffer[2048]{};
                cin.getline(buffer, 2048);
                cmd_queue.emplace(string(buffer));
            }
        }
    }
}

void show_options()
{
    cout << "Options:\n"
         << " 1 - run\n"
         << " 2 - step over\n"
         << " 3 - step into\n"
         << " 4 - step out\n"
         << " 5 - pause\n"
         << " 6 - add breakpoint\n"
         << " 7 - del breakpoint\n"
         << " 8 - eval\n"
         << " 9 - stack\n"
         << "10 - debugger state\n"
         << "11 - show this message\n";
}
