#include "editorImGui.h"

#include <mayEngine/tools/imgui/imgui.h>

#include "ext/TextEditor.h"
#include "keyTable.h"

using namespace ShitEditor;
NS_MAY_US;

void EditorImGuiComponent::render_menu_tools()
{
    if (ImGui::BeginMenu("插入颜色"))
    {
        static float color[3];
        ImGui::ColorPicker3("##ColorPicker", color, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB);
        if (ImGui::Button("插入", ImVec2{ 120, 0 }) && !editor->IsReadOnly())
        {
            const int len = snprintf(nullptr, 0, "me.v3(%f, %f, %f)", color[0], color[1], color[2]) + 1;
            char* buffer = new char[len];
            snprintf(buffer, len, "me.v3(%f, %f, %f)", color[0], color[1], color[2]);
            mLog("插入颜色: %s\n", buffer);
            editor->InsertText(buffer);
            editor_text_changed = true;
            delete[] buffer;
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("插入按键数字"))
    {
        static char buffer[50]{};
        ImGui::InputTextWithHint("##Filter", "过滤器", buffer, sizeof(buffer), ImGuiInputTextFlags_CharsUppercase);
        ImGui::Separator();
        for (auto& data : keyTable)
        {
            if (!strstr(data.first + 4, buffer))
            {
                continue;
            }
            if (ImGui::MenuItem(data.first))
            {

                if (!editor->IsReadOnly())
                {
                    char buffer3[10];
                    snprintf(buffer3, 10, "%d", data.second);
                    editor->InsertText(buffer3);
                    editor_text_changed = true;
                }
            }
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNone | ImGuiHoveredFlags_NoSharedDelay))
            {
                ImGui::SetItemTooltip("%s = %d", data.first, data.second);
            }
        }
        ImGui::EndMenu();
    }
}

