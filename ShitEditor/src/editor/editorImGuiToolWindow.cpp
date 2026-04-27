#include "editorImGui.h"

#include <mayEngine/tools/imgui/imgui.h>

using namespace ShitEditor;

void EditorImGuiComponent::render_tool_window()
{
    show_tools_window = show_debug_window || show_find_window;
    if (!show_tools_window)
    {
        return;
    }

    if (ImGui::BeginChild("##ToolsWindow", { -FLT_MIN, -FLT_MIN },
                          ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeY))
    {
        if (ImGui::BeginTabBar("##ToolsTabBar", ImGuiTabBarFlags_AutoSelectNewTabs))
        {
            if (ImGui::BeginTabItem("调试器", &show_debug_window))
            {
                render_debugger();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("查找", &show_find_window))
            {
                render_find_window();

                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }

    ImGui::EndChild();
}
