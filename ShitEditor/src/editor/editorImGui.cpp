#include "editorImGui.h"

#include <filesystem>
#include <thread>

#include <Windows.h>

#include <mayEngine/tools/imgui/imgui.h>

#include <mobdebug_server.h>

#include "ext/TextEditor.h"
#include "editorScene.h"
#include "editorImGuiStaticInfo.h"

#include "../res.h"
#include "../exit.h"

NS_MAY_US;
using namespace ShitEditor;
namespace fs = std::filesystem;

EditorImGuiComponent* EditorImGuiComponent::create(Node* node_)
{
    EditorImGuiComponent* ret = maymnew(EditorImGuiComponent);
    if (!ret)
    {
        return nullptr;
    }
    ret->node = node_;
    if (!ret->init())
    {
        ret->destroy();
        maymdel(ret);
        return nullptr;
    }
    return ret;
}

EditorImGuiComponent::~EditorImGuiComponent()
{
    if (editor)
    {
        maymdel(editor);
        editor = nullptr;
    }
    if (debug_server_started)
    {
        debug_server->stop();
    }
}

bool EditorImGuiComponent::init()
{
    ImGuiComponent::init();
    applyCtx();

    const float dpi = ScreenManager::getDisplayDpi(0);
    scale = dpi / 96.0f;
    mLog("DPI缩放比例：%d%%\n", M_SC(int, scale * 100.0f));
    // ImGui::SetWindowFontScale(scale);
    const float font_size = 20.0f * scale;

    const ImGuiIO& io = ImGui::GetIO();

    const ResourceManager* man = res::get();

    size_t f_size = man->getResourceSize("/font/HarmonyOS_Sans.ttf");
    char* f_data = new char[f_size];
    man->getResource("/font/HarmonyOS_Sans.ttf", f_data, f_size);

    static const ImWchar font_ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x2000, 0x206F, // General Punctuation
        0x2190, 0x21FF, // Arrows
        0x2500, 0x257F, // Tables
        0x2580, 0x259F, // Blocks
        0x25A0, 0x25FF, // Shapes
        0x27F0, 0x27FF, // More Arrows A
        0x2900, 0x297F, // More Arrows B
        0x2B00, 0x2BFF, // Addition Shapes
        0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
        0x31F0, 0x31FF, // Katakana Phonetic Extensions
        0xFF00, 0xFFEF, // Half-width characters
        0xFFFD, 0xFFFD, // Invalid
        0x4e00, 0x9FAF, // CJK Ideograms
        0,
    };

    ImFontConfig cfg = ImFontConfig();
    cfg.FontDataOwnedByAtlas = false;
    strcpy(cfg.Name, "HarmonyOS Sans SC");
    io.Fonts->AddFontFromMemoryTTF(f_data, f_size, font_size, &cfg, &font_ranges[0]);
    delete[] f_data;

    f_size = man->getResourceSize("/font/MapleMonoNL-NF-CN-Regular.ttf");
    f_data = new char[f_size];
    man->getResource("/font/MapleMonoNL-NF-CN-Regular.ttf", f_data, f_size);

    cfg = ImFontConfig();
    cfg.FontDataOwnedByAtlas = false;
    strcpy(cfg.Name, "MapleMono NL NF CN");

    editor_font = io.Fonts->
                     AddFontFromMemoryTTF(f_data, f_size, font_size, &cfg, &font_ranges[0]);
    delete[] f_data;

    editor = maymnew(TextEditor);
    editor->SetReadOnly(true);

    apply_theme();
    flush_explorer();
    init_debug_server();

    return true;
}

void EditorImGuiComponent::apply_theme() const
{
    ImVec4* colors = ImGui::GetStyle().Colors;

    // Text
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.90f, 0.80f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.55f, 0.50f, 0.45f, 1.00f);

    // Keep your original backgrounds
    colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.85f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.09f, 0.06f, 0.95f);

    // Borders
    colors[ImGuiCol_Border] = ImVec4(0.45f, 0.32f, 0.18f, 0.70f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    // Frames
    colors[ImGuiCol_FrameBg] = ImVec4(0.32f, 0.25f, 0.17f, 0.70f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.45f, 0.33f, 0.20f, 0.85f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.55f, 0.40f, 0.22f, 1.00f);

    // Title bars
    colors[ImGuiCol_TitleBg] = ImVec4(0.25f, 0.18f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.40f, 0.28f, 0.16f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.15f, 0.10f, 0.70f);

    // Menubar
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.18f, 0.12f, 0.08f, 1.00f);

    // Scrollbars
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.15f, 0.10f, 0.06f, 0.60f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.40f, 0.20f, 0.50f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.65f, 0.48f, 0.25f, 0.70f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.75f, 0.55f, 0.28f, 0.85f);

    // Checks & sliders
    colors[ImGuiCol_CheckMark] = ImVec4(0.95f, 0.85f, 0.40f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.90f, 0.75f, 0.30f, 0.60f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.95f, 0.80f, 0.33f, 1.00f);

    // Buttons
    colors[ImGuiCol_Button] = ImVec4(0.45f, 0.34f, 0.22f, 0.70f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.60f, 0.45f, 0.25f, 0.85f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.75f, 0.55f, 0.30f, 1.00f);

    // Headers
    colors[ImGuiCol_Header] = ImVec4(0.55f, 0.40f, 0.20f, 0.50f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.65f, 0.45f, 0.25f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.75f, 0.55f, 0.30f, 1.00f);

    // Separators
    colors[ImGuiCol_Separator] = ImVec4(0.40f, 0.30f, 0.20f, 0.70f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.55f, 0.40f, 0.25f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.55f, 0.30f, 1.00f);

    // Resize grip
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.90f, 0.75f, 0.30f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.95f, 0.80f, 0.35f, 0.70f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.85f, 0.40f, 1.00f);

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4(0.32f, 0.25f, 0.15f, 0.85f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.55f, 0.40f, 0.20f, 0.90f);
    colors[ImGuiCol_TabSelected] = ImVec4(0.65f, 0.48f, 0.25f, 1.00f);
    colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.90f, 0.75f, 0.30f, 1.00f);
    colors[ImGuiCol_TabDimmed] = ImVec4(0.22f, 0.18f, 0.12f, 0.70f);
    colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.45f, 0.33f, 0.20f, 0.90f);

    // Docking
    colors[ImGuiCol_DockingPreview] = ImVec4(0.90f, 0.75f, 0.30f, 0.35f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.18f, 0.12f, 1.00f);

    // Plots
    colors[ImGuiCol_PlotLines] = ImVec4(0.90f, 0.80f, 0.40f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.85f, 0.50f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.95f, 0.80f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.90f, 0.40f, 1.00f);

    // Table
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.25f, 0.18f, 0.10f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.35f, 0.25f, 0.15f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.25f, 0.20f, 0.15f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);

    // Text selection
    colors[ImGuiCol_TextLink] = ImVec4(0.90f, 0.75f, 0.30f, 0.85f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.90f, 0.75f, 0.30f, 0.35f);

    // Misc
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 0.85f, 0.10f, 0.90f);
    colors[ImGuiCol_NavCursor] = ImVec4(0.95f, 0.80f, 0.35f, 0.70f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.90f, 0.50f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.50f, 0.40f, 0.20f, 0.30f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.15f, 0.10f, 0.40f);

    static constexpr TextEditor::Palette p = {
        {
            0xffa0c8d8, // Default
            0xff60b0e0, // Keyword
            0xff60d0a8, // Number
            0xff60b0d8, // String
            0xff70c0e0, // Char literal
            0xffffffff, // Punctuation
            0xff5090a0, // Preprocessor
            0xffb0c8d0, // Identifier
            0xff80e0c8, // Known identifier
            0xffc088e0, // Preproc identifier
            0xff406070, // Comment (single line)
            0xff507080, // Comment (multi line)
            0xA00f1820, // Background
            0xff90d0e8, // Cursor
            0x804070a0, // Selection
            0x80202040, // ErrorMarker
            0x4040c0f0, // Breakpoint
            0xff3090c0, // Line number
            0x40102030, // Current line fill
            0x40404050, // Current line fill (inactive)
            0x40406060, // Current line edge
            0x402cc59c, // debug line
        }
    };
    editor->SetPalette(p);
}

void EditorImGuiComponent::main_update(float dt)
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    M_SC(EditorScene*, node->getParent())->loadFinish();

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    update_debugger();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    if (ImGui::Begin(
        "ShitEditor", nullptr,
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar))
    {
        ImGui::PopStyleVar();
        if (ImGui::BeginMenuBar())
        {
            render_menu();
            ImGui::EndMenuBar();
        }
        if (ImGui::BeginChild("FileExplorer", ImVec2{ 200 * scale, 0 },
                              ImGuiChildFlags_ResizeX | ImGuiChildFlags_Borders))
        {
            if (require_flush_explorer)
            {
                require_flush_explorer = false;
                flush_explorer();
            }
            render_explorer();
        }
        ImGui::EndChild();

        if (ImGui::BeginPopupContextItem(nullptr,
                                         ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_NoOpenOverExistingPopup |
                                         ImGuiPopupFlags_MouseButtonRight))
        {
            if (ImGui::MenuItem("刷新"))
            {
                require_flush_explorer = true;
            }
            ImGui::Separator();
            if (ImGui::MenuItem("创建 文件/目录"))
            {
                explorer_popup_file_path = "Package";
                explorer_open_create_popup = true;
            }
            ImGui::EndPopup();
        }

        ImGui::SameLine();

        if (ImGui::BeginChild("##TextEditorFrame", ImVec2{ -FLT_MIN, 0 },
                              ImGuiChildFlags_AutoResizeX, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
        {
            ImGuiChildFlags child_flag = ImGuiChildFlags_None;
            const ImVec2 frame_size = ImGui::GetWindowSize();
            if (show_debug_window)
            {
                child_flag |= ImGuiChildFlags_ResizeY;
                float maxy = frame_size.y - 168.0f * scale;
                if (maxy < 0)
                {
                    maxy = 0;
                }
                ImGui::SetNextWindowSizeConstraints({ 0, 0 }, { FLT_MAX, maxy });
            }
            if (size_updated && show_debug_window)
            {
                ImGui::SetNextWindowSize({ -FLT_MAX, frame_size.y - debug_window_height });
            }
            if (ImGui::BeginChild("##TextEditor", ImVec2{ -FLT_MIN, 0 }, child_flag))
            {
                if (show_debug_window)
                {
                    debug_window_height = frame_size.y - ImGui::GetWindowHeight();
                }
                if (ImGui::IsKeyDown(ImGuiKey_ModCtrl))
                {
                    ImGuiIO& io = ImGui::GetIO();
                    if (io.MouseWheel != 0.0f)
                    {
                        editor_scale += io.MouseWheel * 0.1f;
                        if (editor_scale < 0.2f)
                        {
                            editor_scale = 0.2f;
                        }
                        if (editor_scale > 5.0f)
                        {
                            editor_scale = 5.0f;
                        }
                        io.MouseWheel = 0;
                    }
                }
                if (just_paused)
                {
                    if (fs::exists(paused_file))
                    {
                        explorer_select_path = paused_file;
                        update_editor_content();
                        editor->SetCursorPosition(TextEditor::Coordinates(paused_line - 1, 0));
                    }
                    just_paused = false;
                }
                if (debugger_paused)
                {
                    editor->SetDebugLine(paused_line);
                }
                else
                {
                    editor->SetDebugLine(-1);
                }
                ImGui::SetWindowFontScale(editor_scale);
                ImGui::PushFont(M_SC(ImFont*, editor_font));
                editor->Render("##TextEditorInner", ImVec2{ 0, 0 }, false);
                ImGui::PopFont();
                ImGui::SetWindowFontScale(1.0f);
            }
            ImGui::EndChild();
            render_debugger();
        }
        ImGui::EndChild();

        if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_S, ImGuiInputFlags_RouteGlobal) && editor_text_changed)
        {
            save_current_file();
        }

        explorer_popups();
        editor_popups();

        if (ImGui::Shortcut(ImGuiKey_F8))
        {
            op_pack = true;
            op_run = true;
        }
        if (ImGui::Shortcut(ImGuiKey_F7))
        {
            op_run = true;
        }
        if (ImGui::Shortcut(ImGuiKey_F6))
        {
            op_pack = true;
        }
        if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_F8))
        {
            set_breakpoint();
        }

        process_op();
    }
    else
    {
        ImGui::PopStyleVar();
    }
    ImGui::End();
    // ImGui::ShowDemoWindow();
    if (show_style_editor)
    {
        ImGui::SetNextWindowFocus();
        if (ImGui::Begin("Style Editor", &show_style_editor))
        {
            ImGui::ShowStyleEditor();
        }
        ImGui::End();
    }
    update_editor_title();
    if (size_updated)
    {
        size_updated = false;
    }
}

void EditorImGuiComponent::update_editor_content()
{
    static const mmap<mstring, std::function<const TextEditor::LanguageDefinition&()>> ext_file_type_mapping = {
        { ".lua", &TextEditor::LanguageDefinition::Lua },
        { ".vert", &TextEditor::LanguageDefinition::GLSL },
        { ".frag", &TextEditor::LanguageDefinition::GLSL },
        { ".comp", &TextEditor::LanguageDefinition::GLSL },
        { ".glsl", &TextEditor::LanguageDefinition::GLSL },
        { ".json", &TextEditor::LanguageDefinition::Lua },
        { ".shit", &TextEditor::LanguageDefinition::ShitLang },
    };
    static TextEditor::LanguageDefinition defaultDef{};

    editor_text_changed = false;
    editor->SetReadOnly(explorer_select_path.empty());

    if (explorer_select_path.empty())
    {
        editor->SetText("");
        editor->SetLanguageDefinition(defaultDef);
        editor_opened_path.clear();
        return;
    }

    if (editor_opened_path == explorer_select_path)
    {
        return;
    }

    editor_opened_path = explorer_select_path;

    const auto language = ext_file_type_mapping.find(
        mstring(fs::path(explorer_select_path).extension().generic_string()));
    if (language != ext_file_type_mapping.end())
    {
        editor->SetLanguageDefinition(language->second());
    }
    else
    {
        editor->SetLanguageDefinition(defaultDef);
    }

    FILE* fp = std::fopen(explorer_select_path.c_str(), "rb");
    std::fseek(fp, 0, SEEK_END);
    const long size = std::ftell(fp);
    std::fseek(fp, 0, SEEK_SET);

    char* text = new char[size + 1];
    std::fread(text, 1, size, fp);
    text[size] = 0;

    editor->SetText(text);

    std::fclose(fp);

    // Breakpoints
    const auto found_file = breakpoints.find(explorer_select_path);
    if (found_file != breakpoints.end())
    {
        editor->SetBreakpoints(found_file->second);
    }
    else
    {
        editor->SetBreakpoints({});
    }
}

void EditorImGuiComponent::render_menu()
{
    if (ImGui::BeginMenu("文件"))
    {
        if (ImGui::MenuItem("保存", "Ctrl+S") && editor_text_changed)
        {
            save_current_file();
        }

        if (ImGui::MenuItem("关闭文件"))
        {
            if (!explorer_select_path.empty())
            {
                if (editor_text_changed)
                {
                    save_current_file();
                }
                explorer_select_path.clear();
            }
            update_editor_content();
        }

        ImGui::Separator();
        if (ImGui::MenuItem("退出", "Alt+F4"))
        {
            editor_exit_popup = true;
        }

        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("编辑"))
    {
        if (ImGui::MenuItem("撤回", "Ctrl+Z"))
        {
            if (editor->CanUndo())
            {
                editor->Undo();
                editor_text_changed = true;
            }
        }
        if (ImGui::MenuItem("重做", "Ctrl+Y"))
        {
            if (editor->CanRedo())
            {
                editor->Redo();
                editor_text_changed = true;
            }
        }
        ImGui::Separator();
        if (ImGui::MenuItem("复制", "Ctrl+C"))
        {
            editor->Copy();
        }
        if (ImGui::MenuItem("剪切", "Ctrl+X"))
        {
            if (editor->HasSelection())
            {
                editor->Cut();
                editor_text_changed = true;
            }
        }
        if (ImGui::MenuItem("粘贴", "Ctrl+V"))
        {
            editor->Paste();
            editor_text_changed = true;
        }
        ImGui::Separator();
        if (ImGui::MenuItem("全选", "Ctrl+A"))
        {
            editor->SelectAll();
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("运行"))
    {
        if (ImGui::MenuItem("打包并运行", "F8"))
        {
            op_pack = true;
            op_run = true;
        }

        if (ImGui::MenuItem("打包", "F6"))
        {
            op_pack = true;
        }

        if (ImGui::MenuItem("运行", "F7"))
        {
            op_run = true;
        }

        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("调试"))
    {
        ImGui::Checkbox("显示调试窗口", &this->show_debug_window);

        // Debug server
        static bool run_dbg = false;
        if (ImGui::Checkbox("启动调试服务器", &run_dbg))
        {
            if (run_dbg != debug_server_started)
            {
                if (run_dbg)
                {
                    debug_server->start();
                    mLog("调试服务器已启动\n");
                }
                else
                {
                    debug_server->stop();
                    mLog("调试服务器已关闭\n");
                }
                debug_server_started = run_dbg;
            }
        }

        ImGui::Separator();

        ImGui::Checkbox("启用初始断点", &debugger_enable_init_breakpoint);

        if (ImGui::MenuItem("设置断点", "Ctrl+F8"))
        {
            set_breakpoint();
        }

        if (ImGui::MenuItem("清除断点"))
        {
            clear_breakpoints();
        }

        ImGui::Separator();
        if (ImGui::MenuItem("添加监视"))
        {
            editor_add_watch_popup = true;
        }

        if (ImGui::MenuItem("清空监视"))
        {
            clear_watch();
        }

        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("工具"))
    {
        render_menu_tools();
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("设置"))
    {
        ImGui::Text("缩放");
        ImGui::SameLine(0, 10.0f);
        if (ImGui::SmallButton("重置"))
        {
            editor_scale = 1.0f;
        }
        ImGui::SliderFloat("##Scale", &editor_scale, 0.2f, 5.0f, "");
        ImGui::SameLine();
        ImGui::Text("%d%%", (int)(editor_scale * 100.0f));

        ImGui::Separator();

        ImGui::Checkbox("显示样式编辑器", &this->show_style_editor);

        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("关于"))
    {
        if (ImGui::MenuItem("关于 ShitEditor"))
        {
            editor_about_popup = true;
        }
        ImGui::EndMenu();
    }
}

void EditorImGuiComponent::editor_popups()
{
    const ImGuiIO& io = ImGui::GetIO();

    if (editor_exit_popup)
    {
        ImGui::OpenPopup("退出");
        editor_exit_popup = false;
    }
    ImGui::SetNextWindowPos(
        ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
        ImGuiCond_Always,
        ImVec2(0.5f, 0.5f)
    );
    if (ImGui::BeginPopupModal("退出", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        ImGui::Text("确定退出？");
        if (editor_text_changed)
        {
            ImGui::SetWindowFontScale(0.6f);
            ImGui::TextColored(ImVec4{ 1.0f, 0.7529f, 0.0902f, 1.0 }, "您似乎还有文件未保存");
            ImGui::SetWindowFontScale(1.0f);
        }
        ImGui::Separator();
        if (ImGui::Button("取消", ImVec2{ 100, 0 }))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("确定", ImVec2{ 100, 0 }))
        {
            Exit::set_accept_exit(true);
            Engine::getInstance()->exit();
        }
        ImGui::EndPopup();
    }

    // About
    if (editor_about_popup)
    {
        ImGui::OpenPopup("关于 ShitEditor");
        editor_about_popup = false;
    }
    ImGui::SetNextWindowPos(
        ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
        ImGuiCond_Always,
        ImVec2(0.5f, 0.5f)
    );
    if (ImGui::BeginPopupModal("关于 ShitEditor", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        ImGui::Text("ShitEditor v1.0.0");
        ImGui::Text("Copyright (c) 2026 Majjcom");
        ImGui::Separator();
        if (ImGui::CollapsingHeader("LICENSE"))
        {
            ImGui::PushFont((ImFont*)editor_font);
            ImGui::InputTextMultiline(
                "##LICENSE", (char*)LICENSE, sizeof(LICENSE),
                ImVec2{ 800 * scale, 450 * scale },
                ImGuiInputTextFlags_ReadOnly);
            ImGui::PopFont();
        }
        if (ImGui::CollapsingHeader("OPEN SOURCE"))
        {
            ImGui::PushFont((ImFont*)editor_font);
            ImGui::InputTextMultiline(
                "##OPENSOURCE", (char*)OPENSOURCE_PROJECTS, sizeof(OPENSOURCE_PROJECTS),
                ImVec2{ 800 * scale, 500 * scale },
                ImGuiInputTextFlags_ReadOnly);
            ImGui::PopFont();
        }
        ImGui::Separator();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0f - 50.0f);
        if (ImGui::Button("关闭", ImVec2{ 100, 0 }))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if (editor_add_watch_popup)
    {
        ImGui::OpenPopup("添加监视");
        editor_add_watch_popup = false;
    }
    ImGui::SetNextWindowPos(
        ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
        ImGuiCond_Always,
        ImVec2(0.5f, 0.5f)
    );
    if (ImGui::BeginPopupModal("添加监视", nullptr,
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        ImGui::PushFont((ImFont*)editor_font);
        static char buffer[2048]{};
        ImGui::InputText("##Expr", buffer, 2048);
        ImGui::PopFont();
        ImGui::Separator();
        if (ImGui::Button("取消", { 100, 0 }))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("添加", { 100, 0 }))
        {
            if (buffer[0] != '\0')
            {
                add_watch(buffer);
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    if (editor_packaging_popup)
    {
        ImGui::OpenPopup("正在打包...");
    }
    ImGui::SetNextWindowPos(
        ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
        ImGuiCond_Always,
        ImVec2(0.5f, 0.5f)
    );
    if (ImGui::BeginPopupModal("正在打包...", nullptr,
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        ImGui::ProgressBar(-(float)ImGui::GetTime(), { 200 * scale , 0 });
        if (!editor_packaging_popup)
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void EditorImGuiComponent::save_current_file()
{
    if (explorer_select_path.empty())
    {
        return;
    }
    const mstring u8path(fs::path(explorer_select_path).generic_u8string());
    mLog("保存文件：%s\n", u8path.c_str());
    FILE* fp = std::fopen(explorer_select_path.c_str(), "wb");
    mstring text(editor->GetText());
    text = text.substr(0, text.size() - 1);
    std::fwrite(text.c_str(), 1, text.size(), fp);
    std::fclose(fp);
    editor_text_changed = false;
}

void EditorImGuiComponent::update_editor_title()
{
    const ScreenManager* sm = ScreenManager::getInstance();
    mstring title = "ShitEditor";
    if (!explorer_select_path.empty())
    {
        title += " - ";

        if (editor->IsTextChanged())
        {
            editor_text_changed = true;
        }

        if (editor_text_changed)
        {
            title += '*';
        }

        const mstring u8path(fs::path(explorer_select_path).generic_u8string());
        title += u8path;
    }

    if (title != last_window_title)
    {
        sm->setWindowTitle(title);
        last_window_title = title;
    }
}

void EditorImGuiComponent::process_op()
{
    if (op_pack)
    {
        op_pack = false;
        if (!editor_packaging_popup)
        {
            editor_packaging_popup = true;
            std::thread([this]
            {
                system("createPackage.bat");
                editor_packaging_popup = false;
            }).detach();
        }
    }
    if (op_run)
    {
        op_run = false;
        if (process_running)
        {
            return;
        }
        auto process = std::thread([this](std::atomic_bool* monitor)
        {
            *monitor = true;

            while (editor_packaging_popup)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            STARTUPINFO si{};
            PROCESS_INFORMATION pi{};
            si.cb = sizeof(si);

            if (!CreateProcess(
                nullptr, (char*)"ShitGame.exe",
                nullptr, nullptr, false,
                0, nullptr, nullptr,
                &si, &pi
            ))
            {
                mLoge("Create Process Failed...\n");
                *monitor = false;
                return;
            }

            WaitForSingleObject(pi.hProcess, INFINITE);

            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

            *monitor = false;
        }, &process_running);
        process.detach();
    }
}

void EditorImGuiComponent::open_exit_popup()
{
    editor_exit_popup = true;
}

void EditorImGuiComponent::update_size(const Vec3&)
{
    size_updated = true;
}
