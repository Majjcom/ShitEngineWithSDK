#include <mobdebug_server.h>

#include "ext/TextEditor.h"
#include "editorImGui.h"
#include <mayEngine/tools/imgui/imgui.h>
#include <mayEngine/tools/imgui/imgui_stdlib.h>

#include <filesystem>
#include <charconv>

#include <lua/lua.hpp>

using namespace ShitEditor;
namespace fs = std::filesystem;

namespace
{
    mstring lua_run_result(lua_State* L, const char* code)
    {
        luaL_loadstring(L, code);
        if (lua_pcall(L, 0, 1, 0))
        {
            const char* msg = lua_tostring(L, -1);
            lua_pop(L, -1);
            return mstring("[error] ") + msg;
        }
        lua_rawgeti(L, -1, 1);
        if (lua_isnil(L, -1))
        {
            lua_pop(L, -1);
            return "nil";
        }
        const char* res = lua_tostring(L, -1);
        lua_pop(L, -1);
        return res;
    }

    void lua_store_stack(lua_State* L, const char* code)
    {
        luaL_loadstring(L, code);
        lua_pcall(L, 0, 1, 0);
        lua_setglobal(L, "stack_info");
        lua_pop(L, -1);
    }

    /**
     * 消耗栈顶任意类型数据
     * @param L
     * @param name
     */
    void lua_render_imgui_object(lua_State* L, const char* name)
    {
        constexpr ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
            ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        mstring show_value("[unkown]");
        switch (lua_type(L, -1))
        {
        case LUA_TNIL:
            show_value = "[nil]";
            break;
        case LUA_TBOOLEAN:
            show_value = lua_toboolean(L, -1) ? "true" : "false";
            break;
        case LUA_TLIGHTUSERDATA:
            show_value = "[lightuserdata]";
            break;
        case LUA_TNUMBER:
            if (lua_isinteger(L, -1))
            {
                const lua_Integer d = lua_tointeger(L, -1);
                mstringstream ss;
                ss << d;
                ss >> show_value;
            }
            else
            {
                const lua_Number r = lua_tonumber(L, -1);
                mstringstream ss;
                ss << r;
                ss >> show_value;
            }
            break;
        case LUA_TSTRING:
            show_value = lua_tostring(L, -1);
            break;
        case LUA_TFUNCTION:
            show_value = "[function]";
            break;
        case LUA_TUSERDATA:
            show_value = "[userdata]";
            break;
        case LUA_TTHREAD:
            show_value = "[thread]";
            break;
        default:
            break;
        }
        mstring show(name);
        show += " = ";
        show += show_value;

        ImGui::TreeNodeEx(name, flags, "%s", show.c_str());

        lua_pop(L, 1);
    }

    /**
     * 消耗一个栈顶的table
     * @param L
     * @param name
     */
    void lua_render_imgui_table(lua_State* L, const char* name)
    {
        constexpr ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
            ImGuiTreeNodeFlags_SpanAvailWidth;
        if (ImGui::TreeNodeEx(name, flags, "%s", name))
        {
            lua_pushnil(L); // index
            while (lua_next(L, -2))
            {
                lua_pushvalue(L, -2);
                const char* key_name = lua_tostring(L, -1); // key
                lua_pop(L, 1);
                const char* type_name = lua_typename(L, lua_type(L, -1)); // type name
                mstring show_name(key_name);
                show_name += ": ";
                show_name += type_name;
                switch (lua_type(L, -1))
                {
                case LUA_TTABLE:
                    // pop table
                    lua_render_imgui_table(L, show_name.c_str());
                    break;
                default:
                    // pop obj
                    lua_render_imgui_object(L, show_name.c_str());
                    break;
                }
            }
            ImGui::TreePop();
        }
        lua_pop(L, 1);
    }

    void lua_render_imgui_local_global_value(lua_State* L, const char* name)
    {
        constexpr ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
            ImGuiTreeNodeFlags_SpanAvailWidth;
        mstring id_name = name;
        id_name = id_name.substr(0, id_name.rfind(':'));
        if (ImGui::TreeNodeEx(id_name.c_str(), flags, "%s", name))
        {
            const int idx_l = lua_gettop(L) - 2 + 1;
            const int idx_g = lua_gettop(L) - 1 + 1;

            // locals
            lua_pushnil(L); // index
            while (lua_next(L, idx_l))
            {
                lua_pushvalue(L, -2);
                const char* key_name = lua_tostring(L, -1); // key
                lua_pop(L, 1);
                lua_rawgeti(L, -1, 1);
                const char* type_name = lua_typename(L, lua_type(L, -1)); // type name
                mstring show_name(key_name);
                show_name += ": ";
                show_name += type_name;
                switch (lua_type(L, -1))
                {
                case LUA_TTABLE:
                    // pop table
                    lua_render_imgui_table(L, show_name.c_str());
                    break;
                default:
                    // pop obj
                    lua_render_imgui_object(L, show_name.c_str());
                    break;
                }
                lua_pop(L, 1);
            }

            // globals
            lua_pushnil(L); // index
            while (lua_next(L, idx_g))
            {
                lua_pushvalue(L, -2);
                const char* key_name = lua_tostring(L, -1); // key
                lua_pop(L, 1);
                lua_rawgeti(L, -1, 1);
                const char* type_name = lua_typename(L, lua_type(L, -1)); // type name
                mstring show_name(key_name);
                show_name += ": ";
                show_name += type_name;
                switch (lua_type(L, -1))
                {
                case LUA_TTABLE:
                    // pop table
                    lua_render_imgui_table(L, show_name.c_str());
                    break;
                default:
                    // pop obj
                    lua_render_imgui_object(L, show_name.c_str());
                    break;
                }
                lua_pop(L, 1);
            }
            ImGui::TreePop();
        }
        lua_pop(L, 2);
    }

    /**
     * 显示栈树
     * @param L
     */
    void lua_render_stack_tree(lua_State* L)
    {
        lua_pushnil(L);
        while (lua_next(L, -2)) // for stacks
        {
            lua_rawgeti(L, -1, 1); // stack info

            lua_rawgeti(L, -1, 1); // stack function name
            mstring func_name("[unknown]");
            if (lua_isstring(L, -1))
            {
                func_name = lua_tostring(L, -1);
            }
            lua_pop(L, 1);

            lua_rawgeti(L, -1, 2); // stack code file name
            mstring file{};
            if (lua_isstring(L, -1))
            {
                file = lua_tostring(L, -1);
            }
            lua_pop(L, 1);

            lua_rawgeti(L, -1, 3);
            const lua_Integer func_line = lua_tointeger(L, -1);
            mstring func_line_str;
            {
                mstringstream ss;
                ss << func_line;
                ss >> func_line_str;
            }
            lua_pop(L, 1);

            lua_rawgeti(L, -1, 4); // stack code line
            const lua_Integer line = lua_tointeger(L, -1);
            mstring line_str;
            {
                mstringstream ss;
                ss << line;
                ss >> line_str;
            }
            lua_pop(L, 1);

            lua_pop(L, 1); // pop stack info

            // stack info name
            mstring stack_name(file);
            stack_name += ':';
            stack_name += func_line_str;
            stack_name += ':';
            stack_name += func_name;
            stack_name += ':';
            stack_name += line_str;

            // render stack locals
            lua_rawgeti(L, -1, 2); // stack locals table
            lua_rawgeti(L, -2, 3); // stack globals table
            lua_render_imgui_local_global_value(L, stack_name.c_str());

            lua_pop(L, 1); // pop one stack
        }
    }
}

#define L (this->debugger_lua_state)

void EditorImGuiComponent::init_debug_server()
{
    // Create lua state
    debugger_lua_state = luaL_newstate();
    luaL_openlibs(L);

    // Create debug server
    debug_server = me_make_unique<MobDebugCpp::MobDebugServer>();
    debug_server->on_output([this](const std::string& msg)
    {
        debugger_messages.emplace_back("[Message] " + msg);
        debugger_message_changed = true;
    });
    debug_server->on_connected([this]
    {
        debugger_messages.clear();
        debugger_messages.emplace_back("[Debugger] Connected...");
        debugger_message_changed = true;
        debugger_connected = true;
        show_debug_window = true;
        if (!debugger_enable_init_breakpoint)
        {
            debug_server->cmd_run();
        }
    });
    debug_server->on_disconnected([this]
    {
        debugger_messages.emplace_back("[Debugger] Disconnected...");
        debugger_message_changed = true;
        debugger_connected = false;
    });
    debug_server->on_error([this](const std::string& msg)
    {
        debugger_messages.emplace_back("[Error] " + msg);
        debugger_message_changed = true;
    });
    debug_server->on_paused([this](const MobDebugCpp::PauseEvent& ev)
    {
        debugger_paused = true;
        paused_line = ev.line;
        paused_file = "Package/shit/" + fs::u8path(ev.file).generic_string();
        just_paused = true;

        load_watches();
        flush_stack_info();

        // Message
        char line_buffer[10]{};
        std::to_chars(line_buffer, line_buffer + 9, ev.line);
        debugger_messages.emplace_back("[Debugger] Pause at `" + ev.file + ':' + line_buffer + '`');
        debugger_message_changed = true;
    });
    debug_server->on_resumed([this]
    {
        debugger_paused = false;
        paused_line = -1;
        paused_file.clear();
    });
}

void EditorImGuiComponent::render_debugger()
{
    if (!this->show_debug_window) return;
    ImGui::PushFont(M_SC(ImFont*, editor_font));
    if (!ImGui::BeginChild("调试器", ImVec2{ -FLT_MIN, -FLT_MIN },
                           ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize))
    {
        ImGui::EndChild();
        return;
    }

    ImGui::BeginDisabled(!debugger_connected);
    if (ImGui::BeginChild("##Tools", { 0, -FLT_MIN },
                          ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY |
                          ImGuiChildFlags_AlwaysAutoResize,
                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        if (ImGui::Button("▶"))
        {
            debug_server->cmd_run();
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip | ImGuiHoveredFlags_DelayNormal))
        {
            ImGui::SetTooltip("继续");
        }

        if (ImGui::Button("■"))
        {
            debug_server->cmd_exit();
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip | ImGuiHoveredFlags_DelayNormal))
        {
            ImGui::SetTooltip("停止");
        }

        if (ImGui::Button("→") ||
            (debugger_connected && ImGui::Shortcut(ImGuiKey_F9,
                                                   ImGuiInputFlags_RouteGlobal | ImGuiInputFlags_Repeat)))
        {
            debug_server->cmd_step_over();
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip | ImGuiHoveredFlags_DelayNormal))
        {
            ImGui::SetTooltip("单步执行(F9)");
        }

        if (ImGui::Button("↧") ||
            (debugger_connected && ImGui::Shortcut(ImGuiKey_F10,
                                                   ImGuiInputFlags_RouteGlobal | ImGuiInputFlags_Repeat)))
        {
            debug_server->cmd_step_into();
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip | ImGuiHoveredFlags_DelayNormal))
        {
            ImGui::SetTooltip("单步进入(F10)");
        }

        if (ImGui::Button("↥") ||
            (debugger_connected && ImGui::Shortcut(ImGuiMod_Shift | ImGuiKey_F10,
                                                   ImGuiInputFlags_RouteGlobal | ImGuiInputFlags_Repeat)))
        {
            debug_server->cmd_step_out();
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip | ImGuiHoveredFlags_DelayNormal))
        {
            ImGui::SetTooltip("单步跳出(Shift+F10)");
        }
    }
    ImGui::EndChild();
    ImGui::EndDisabled();

    ImGui::SameLine();
    if (ImGui::BeginChild("##Panel", { -FLT_MIN, -FLT_MIN }))
    {
        if (ImGui::BeginTabBar("##Tabs"))
        {
            if (ImGui::BeginTabItem("消息"))
            {
                ImGui::BeginDisabled(!debugger_connected);
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyle().Colors[ImGuiCol_FrameBg]);
                if (ImGui::BeginChild("##Logs", { -FLT_MIN, -FLT_MIN },
                                      ImGuiChildFlags_Borders))
                {
                    for (auto& line : debugger_messages)
                    {
                        ImGui::TextWrapped("%s", line.c_str());
                    }
                    ImGui::TextUnformatted("");
                    if (debugger_message_changed)
                    {
                        ImGui::SetScrollY(ImGui::GetScrollMaxY());
                        debugger_message_changed = false;
                    }
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
                ImGui::EndDisabled();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("监视"))
            {
                ImGui::BeginDisabled(!debugger_connected);
                if (ImGui::SmallButton("刷新") ||
                    ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_F5, ImGuiInputFlags_RouteGlobal))
                {
                    load_watches();
                }
                if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip | ImGuiHoveredFlags_DelayNormal))
                {
                    ImGui::SetTooltip("刷新 (Ctrl+F5)");
                }
                ImGui::SameLine();
                if (ImGui::SmallButton("添加"))
                {
                    editor_add_watch_popup = true;
                }
                ImGui::SameLine();
                if (ImGui::SmallButton("清空"))
                {
                    clear_watch();
                }
                if (ImGui::BeginChild("##WatchPanel", { -FLT_MIN, -FLT_MIN }))
                {
                    if (ImGui::BeginTable("##Watches", 2,
                                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                                          ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit))
                    {
                        ImGui::TableSetupColumn("表达式", ImGuiTableColumnFlags_WidthFixed);
                        ImGui::TableSetupColumn("值", ImGuiTableColumnFlags_WidthStretch);
                        ImGui::TableHeadersRow();

                        int delete_idx = -1;

                        for (int i = 0; i < debugger_watch_list.size(); i++)
                        {
                            ImGui::TableNextRow();

                            // Watch
                            ImGui::TableSetColumnIndex(0);
                            ImGui::TextUnformatted(debugger_watch_list[i].first.c_str());
                            char id_buffer[10]{};
                            std::to_chars(id_buffer, id_buffer + 9, i);
                            mstring id = mstring("##watch") + id_buffer;
                            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
                            {
                                ImGui::OpenPopup(id.c_str());
                            }
                            if (ImGui::BeginPopup(id.c_str()))
                            {
                                if (ImGui::MenuItem("删除"))
                                {
                                    delete_idx = i;
                                }
                                ImGui::EndPopup();
                            }

                            // Value
                            ImGui::TableSetColumnIndex(1);
                            ImGui::TextUnformatted(debugger_watch_list[i].second.c_str());
                            id = mstring("##value") + id_buffer;
                            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                            {
                                ImGui::OpenPopup(id.c_str());
                                ImGui::SetNextWindowSizeConstraints({ 500, 0 }, { FLT_MAX, FLT_MAX });
                            }
                            if (ImGui::BeginPopup(id.c_str()))
                            {
                                ImGui::TextWrapped("%s", debugger_watch_list[i].second.c_str());
                                ImGui::EndPopup();
                            }
                        }
                        if (delete_idx != -1)
                        {
                            debugger_watch_list.erase(debugger_watch_list.begin() + delete_idx);
                        }
                        ImGui::EndTable();
                    }
                }
                ImGui::EndChild();
                ImGui::EndDisabled();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("栈"))
            {
                ImGui::BeginDisabled(!debugger_connected);
                if (ImGui::SmallButton("刷新"))
                {
                    flush_stack_info();
                }

                if (ImGui::BeginChild("##StackInfo", { -FLT_MIN, -FLT_MIN },
                                      ImGuiChildFlags_Borders))
                {
                    lua_getglobal(L, "stack_info");
                    if (!lua_isnil(L, -1))
                    {
                        lua_render_stack_tree(L);
                    }
                    lua_pop(L, -1);
                }
                ImGui::EndChild();
                ImGui::EndDisabled();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("注入"))
            {
                ImGui::BeginDisabled(!debugger_connected);
                static mstring buffer;
                ImGui::Text("输入MELua代码表达式进行代码注入：");
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::InputTextWithHint("##expr", "表达式", &buffer);
                ImGui::Separator();
                if (ImGui::SmallButton("Eval"))
                {
                    if (buffer[0] != '\0')
                    {
                        debug_server->eval(std::string(buffer), [this](const bool ok, const std::string& result)
                        {
                            if (!ok)
                            {
                                debugger_code_inject_result.assign("[error] " + mstring(result));
                                return;
                            }

                            debugger_code_inject_result.assign(lua_run_result(L, result.c_str()));
                        });
                    }
                }
                ImGui::SameLine();
                if (ImGui::SmallButton("Exec"))
                {
                    if (buffer[0] != '\0')
                    {
                        debug_server->exec(std::string(buffer), [this](const bool ok, const std::string& result)
                        {
                            if (!ok)
                            {
                                debugger_code_inject_result.assign("[error] " + mstring(result));
                                return;
                            }
                            debugger_code_inject_result.assign(lua_run_result(L, result.c_str()));
                        });
                    }
                }
                ImGui::Separator();
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyle().Colors[ImGuiCol_FrameBg]);
                if (ImGui::BeginChild("##Result", { -FLT_MIN, -FLT_MIN },
                                      ImGuiChildFlags_Borders))
                {
                    ImGui::TextWrapped("%s", debugger_code_inject_result.c_str());
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
                ImGui::EndDisabled();
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }
    ImGui::EndChild();

    ImGui::PopFont();
    ImGui::EndChild();
}

void EditorImGuiComponent::add_watch(const mstring& expr)
{
    auto& ref = debugger_watch_list.emplace_back(expr, mstring()).second;
    if (debugger_paused)
    {
        debug_server->eval(std::string(expr), [this, &ref](const bool ok, const std::string& result)
        {
            if (!ok)
            {
                ref.assign("[error] " + mstring(result));
                return;
            }
            ref.assign(lua_run_result(L, result.c_str()));
        });
    }
}

void EditorImGuiComponent::clear_watch()
{
    debugger_watch_list.clear();
}

void EditorImGuiComponent::load_watches()
{
    for (auto& watch : debugger_watch_list)
    {
        debug_server->eval(
            std::string(watch.first),
            [this, &ref = watch.second](const bool ok, const std::string& result)
            {
                if (!ok)
                {
                    ref.assign("[error] " + mstring(result));
                    return;
                }
                ref.assign(lua_run_result(L, result.c_str()));
            });
    }
}

void EditorImGuiComponent::update_debugger() const
{
    debug_server->poll();
}

void EditorImGuiComponent::set_breakpoint()
{
    if (!explorer_select_path.empty())
    {
        if (explorer_select_path.find("Package/shit/") != 0)
        {
            return;
        }
        const int line = editor->GetCursorPosition().mLine + 1;
        auto found_file = breakpoints.find(explorer_select_path);
        if (found_file == breakpoints.end())
        {
            found_file = breakpoints.insert({ explorer_select_path, {} }).first;
        }
        auto& filebp = found_file->second;
        const auto found = filebp.find(line);
        if (found != filebp.end())
        {
            filebp.erase(found);
        }
        else
        {
            filebp.insert(line);
        }
        editor->SetBreakpoints(filebp);
        const std::string path = fs::path(explorer_select_path).generic_u8string().substr(strlen("Package/shit/"));
        debug_server->set_breakpoints(path, filebp);
    }
}

void EditorImGuiComponent::clear_breakpoints()
{
    for (auto& [file, lines] : breakpoints)
    {
        lines.clear();
        std::string path = fs::path(file).generic_u8string().substr(strlen("Package/shit/"));
        debug_server->set_breakpoints(path, lines);
        if (file == explorer_select_path)
        {
            editor->SetBreakpoints(lines);
        }
    }
}

void EditorImGuiComponent::flush_stack_info() const
{
    debug_server->get_stack([this](const bool ok, const std::string& result)
    {
        if (!ok)
        {
            mLogw("Get stack failed: %s.\n", result.c_str());
            lua_pushnil(L);
            lua_setglobal(L, "stack_info");
            lua_pop(L, -1);
            return;
        }
        lua_store_stack(L, result.c_str());
    });
}
