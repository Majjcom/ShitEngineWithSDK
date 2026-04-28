#include "editorImGui.h"
#include "ext/TextEditor.h"
#include "mayEngine/tools/imgui/imgui.h"
#include "mayEngine/tools/imgui/imgui_stdlib.h"

using namespace ShitEditor;

void EditorImGuiComponent::render_find_window()
{
    if (!show_find_window)
    {
        return;
    }

    if (find_last_find)
    {
        find_last_find = false;
        ImGui::SetNextWindowFocus();
    }

    if (!ImGui::BeginChild("##FindWindow", { -FLT_MIN, -FLT_MIN }))
    {
        ImGui::EndChild();
        return;
    }

    ImGui::Text("查找内容: ");

    bool do_find = false;

    static mstring input_text;
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::PushFont((ImFont*)editor_font);
    ImGui::InputText("##TextInput", &input_text);
    ImGui::PopFont();

    do_find = ImGui::Shortcut(ImGuiKey_Enter, ImGuiInputFlags_RouteOverActive | ImGuiInputFlags_Repeat);
    do_find = ImGui::Button("查找", { 80 * scale, 0 }) || do_find;

    if (do_find && !input_text.empty())
    {
        find_last_find = true;
        TextEditor::Coordinates find_pos;
        if (editor->HasSelection())
        {
            find_pos = editor->GetSelection().second;
        }
        else
        {
            find_pos = editor->GetCursorPosition();
        }
        const auto found = editor->FindTextFrom(find_pos, input_text);
        if (found.has_value())
        {
            editor->SetSelection(found.value().first, found.value().second);
            editor->SetCursorPosition(found.value().first);
        }
        else
        {
            editor->SetCursorPosition({});
            editor->SetSelection({}, {});
        }
    }

    ImGui::EndChild();
}
