#pragma once

#include <mayEngine/mayEngineAll.h>
#include <mayEngine/components/tools/imgui/MayImGuiComponent.h>
#include <unordered_map>
#include <unordered_set>
#include <atomic>

NS_MAY_US;

class TextEditor;
struct lua_State;

namespace MobDebugCpp
{
    class MobDebugServer;
}

namespace ShitEditor
{
    class EditorImGuiComponent : public ImGuiComponent
    {
        M_GET_CLASS_NAME(EditorImGuiComponent);
        M_NODE_DECL_CREATE(EditorImGuiComponent, Node*);

    public:
        ~EditorImGuiComponent() override;
        bool init() override;
        void main_update(float dt) override;
        void open_exit_popup();
        void update_size(const Vec3& size);

    private:
        TextEditor* editor = nullptr;
        struct ExplorerNode
        {
            enum NodeType
            {
                Directory = 0,
                File,
            };
            mstring path{};
            mvector<std::unique_ptr<ExplorerNode>> files;
            NodeType type = File;
        };
        void* editor_font = nullptr;
        float scale = 1.0f;
        float editor_scale = 1.0f;
        mvector<std::unique_ptr<ExplorerNode>> explorerNodes;
        mstring explorer_select_path;
        bool require_flush_explorer = false;
        mstring explorer_popup_file_path;
        bool explorer_open_rename_popup = false;
        bool explorer_open_create_popup = false;
        bool explorer_open_delete_popup = false;
        bool editor_exit_popup = false;
        bool editor_text_changed = false;
        bool editor_about_popup = false;
        bool editor_add_watch_popup = false;
        mstring editor_opened_path;
        float editor_font_scale = 1.0f;
        mstring last_window_title = "ShitEditor";
        bool op_run = false;
        bool op_pack = false;
        bool size_updated = false;
        std::atomic_bool process_running = false;
        bool show_style_editor = false;
        bool editor_packaging_popup = false;

        // Debugger
        std::unordered_map<mstring, std::unordered_set<int>> breakpoints;
        me_unique_ptr<MobDebugCpp::MobDebugServer> debug_server;
        bool debug_server_started = false;
        bool show_debug_window = false;
        float debug_window_height = 0;
        bool debugger_connected = false;
        bool debugger_paused = false;
        int paused_line = -1;
        mstring paused_file;
        bool just_paused = false;
        mvector<mstring> debugger_messages;
        mvector<std::pair<mstring, mstring>> debugger_watch_list;
        lua_State* debugger_lua_state = nullptr;
        bool debugger_message_changed = false;
        mstring debugger_code_inject_result;
        bool debugger_enable_init_breakpoint = false;
        void init_debug_server();
        void render_debugger();
        void update_debugger() const;
        void set_breakpoint();
        void clear_breakpoints();
        void load_watches();
        void add_watch(const mstring& expr);
        void clear_watch();
        void flush_stack_info() const;

        // Methods
        void apply_theme() const;
        void flush_explorer();
        void render_explorer();
        void draw_explorer_node(const ExplorerNode* node);
        void explorer_popups();
        void update_editor_content();
        void render_menu();
        void render_menu_tools();
        void save_current_file();
        void editor_popups();
        void update_editor_title();
        void process_op();

    };
}
