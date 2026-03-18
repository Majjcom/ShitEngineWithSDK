#include "editorImGui.h"

#include <filesystem>

#include <mayEngine/system/IO/mayClipboard.h>
#include <mayEngine/tools/imgui/imgui.h>

using namespace ShitEditor;
namespace fs = std::filesystem;

void EditorImGuiComponent::draw_explorer_node(const ExplorerNode* node)
{
    if (!node) return;

    const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
        | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth
        | (node->type == ExplorerNode::File ? ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen : 0)
        | (explorer_select_path == node->path ? ImGuiTreeNodeFlags_Selected : 0);

    bool opened = false;
    const mstring show_name(fs::path(node->path).filename().generic_u8string());

    if (node->type == ExplorerNode::Directory)
    {
        opened = ImGui::TreeNodeEx(
            node->path.c_str(),
            flags,
            "%s", show_name.c_str()
        );
    }
    else
    {
        // 文件使用 TreeNodeEx，但设置 Leaf 标记
        ImGui::TreeNodeEx(
            node->path.c_str(),
            flags,
            "%s", show_name.c_str()
        );
    }

    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay))
    {
        const mstring p(fs::path(node->path).generic_u8string());
        ImGui::SetTooltip("%s", p.c_str());
    }

    // 点击选择
    if (ImGui::IsItemClicked() && node->type == ExplorerNode::File)
    {
        if (explorer_select_path != node->path)
        {
            if (editor_text_changed)
            {
                save_current_file();
            }
            explorer_select_path = node->path;
            update_editor_content();
        }
    }

    // 右键菜单
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("复制路径"))
        {
            mstring u8path(fs::path(node->path).generic_u8string());
            if (u8path.rfind("Package/res", 0) == 0)
            {
                u8path = u8path.substr(strlen("Package/res"));
            }
            else if (u8path.rfind("Package/shit", 0) == 0)
            {
                u8path = u8path.substr(strlen("Package/shit"));
            }
            IO::Clipboard::setClipboardText(u8path);
        }
        ImGui::Separator();
        if (ImGui::MenuItem("重命名"))
        {
            explorer_popup_file_path = node->path;
            explorer_open_rename_popup = true;
        }
        if (ImGui::MenuItem("删除"))
        {
            explorer_popup_file_path = node->path;
            explorer_open_delete_popup = true;
        }

        if (node->type == ExplorerNode::Directory)
        {
            ImGui::Separator();
            if (ImGui::MenuItem("创建 文件/目录"))
            {
                explorer_popup_file_path = node->path;
                explorer_open_create_popup = true;
            }
        }

        ImGui::EndPopup();
    }

    if (node->type == ExplorerNode::File)
    {
        return;
    }

    // 如果展开，绘制子节点
    if (opened)
    {
        for (auto& child : node->files)
        {
            draw_explorer_node(child.get());
        }
        ImGui::TreePop();
    }
}

void EditorImGuiComponent::render_explorer()
{
    for (auto& node : explorerNodes)
    {
        draw_explorer_node(node.get());
    }
}

void EditorImGuiComponent::flush_explorer()
{
    explorerNodes.clear();
    namespace fs = std::filesystem;
    auto root = fs::directory_entry("Package");
    mstack<fs::directory_entry> entry_stack;
    mstack<mvector<std::unique_ptr<ExplorerNode>>*> node_stack;
    node_stack.emplace(&explorerNodes);
    entry_stack.emplace(root);
    while (!entry_stack.empty())
    {
        auto dir = entry_stack.top();
        entry_stack.pop();
        auto& node_list = *node_stack.top();
        node_stack.pop();
        for (const auto& entry : fs::directory_iterator(dir))
        {
            // Directory
            if (entry.is_directory())
            {
                entry_stack.emplace(entry);
                node_list.emplace_back(
                    std::make_unique<ExplorerNode>(ExplorerNode{
                        mstring(entry.path().generic_string()),
                        {},
                        ExplorerNode::Directory
                    })
                );
                node_stack.emplace(&node_list[node_list.size() - 1]->files);
                continue;
            }

            constexpr const char* ext_list[] = {
                ".shit",
                ".json",
                ".txt",
                ".key",
                ".vert",
                ".frag",
                ".comp",
                ".glsl",
                ".lua",
                ".luac"
            };

            // File
            bool filterd = true;
            for (auto ext : ext_list)
            {
                if (entry.path().extension() == ext)
                {
                    filterd = false;
                    break;
                }
            }

            if (!filterd)
            {
                node_list.emplace_back(
                    std::make_unique<ExplorerNode>(ExplorerNode{
                        mstring(entry.path().generic_string()),
                        {},
                        ExplorerNode::File
                    })
                );
            }
        }
    }
}

void EditorImGuiComponent::explorer_popups()
{
    const ImGuiIO& io = ImGui::GetIO();

    // Rename
    if (explorer_open_rename_popup)
    {
        ImGui::OpenPopup("重命名");
        explorer_open_rename_popup = false;
    }
    ImGui::SetNextWindowPos(
        ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
        ImGuiCond_Always,
        ImVec2(0.5f, 0.5f)
    );
    if (ImGui::BeginPopupModal("重命名", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        static char text_buffer[256]{};
        if (text_buffer[0] == '\0')
        {
            const fs::path old_path(explorer_popup_file_path);
            const mstring old_u8path(old_path.filename().generic_u8string());
            strcpy(text_buffer, old_u8path.c_str());
        }
        ImGui::InputText("##RenameText", text_buffer, std::size(text_buffer));
        ImGui::Separator();
        if (ImGui::Button("取消", ImVec2{ 100, 0 }))
        {
            text_buffer[0] = '\0';
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("确认", ImVec2{ 100, 0 }))
        {
            if (strlen(text_buffer) != 0)
            {
                fs::path new_path(explorer_popup_file_path);
                new_path.replace_filename(fs::u8path(text_buffer));
                fs::rename(explorer_popup_file_path, new_path);
                const auto found_file = breakpoints.find(explorer_popup_file_path);
                if (found_file != breakpoints.end())
                {
                    std::unordered_set<int> tmp;
                    found_file->second.swap(tmp);
                    breakpoints.erase(found_file);
                    breakpoints.insert({ mstring(new_path.generic_string()), tmp });
                }
                require_flush_explorer = true;
            }
            text_buffer[0] = '\0';
            ImGui::CloseCurrentPopup();
            require_flush_explorer = true;
        }
        ImGui::EndPopup();
    }

    // New
    if (explorer_open_create_popup)
    {
        ImGui::OpenPopup("新建 文件/目录");
        explorer_open_create_popup = false;
    }
    ImGui::SetNextWindowPos(
        ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
        ImGuiCond_Always,
        ImVec2(0.5f, 0.5f)
    );
    if (ImGui::BeginPopupModal("新建 文件/目录", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        static char text_buffer[256]{};
        static const char* fail_reason;
        ImGui::InputText("##Name", text_buffer, std::size(text_buffer));
        ImGui::Separator();
        if (ImGui::Button("取消"))
        {
            text_buffer[0] = '\0';
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("创建文件"))
        {
            if (strlen(text_buffer) != 0)
            {
                fs::path path(explorer_popup_file_path);
                path /= fs::u8path(text_buffer);
                if (!fs::exists(path))
                {
                    FILE* fp = std::fopen(path.generic_string().c_str(), "wb");
                    std::fclose(fp);
                    text_buffer[0] = '\0';
                    ImGui::CloseCurrentPopup();
                    require_flush_explorer = true;
                }
                else
                {
                    fail_reason = "文件已存在";
                    ImGui::OpenPopup("创建失败");
                }
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("创建目录"))
        {
            if (strlen(text_buffer) != 0)
            {
                fs::path path(explorer_popup_file_path);
                path /= fs::u8path(text_buffer);
                if (!fs::exists(path))
                {
                    fs::create_directory(path);
                    text_buffer[0] = '\0';
                    ImGui::CloseCurrentPopup();
                    require_flush_explorer = true;
                }
                else
                {
                    fail_reason = "文件已存在";
                    ImGui::OpenPopup("创建失败");
                }
            }
        }

        ImGui::SetNextWindowPos(
            ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
            ImGuiCond_Always,
            ImVec2(0.5f, 0.5f)
        );
        if (ImGui::BeginPopupModal("创建失败", nullptr,
                                   ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
        {
            ImGui::Text("%s", fail_reason);
            ImGui::Separator();
            if (ImGui::Button("关闭", ImVec2{ 120, 0 }))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        ImGui::EndPopup();
    }

    // Delete
    if (explorer_open_delete_popup)
    {
        ImGui::OpenPopup("删除");
        explorer_open_delete_popup = false;
    }
    ImGui::SetNextWindowPos(
        ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
        ImGuiCond_Always,
        ImVec2(0.5f, 0.5f)
    );
    if (ImGui::BeginPopupModal("删除", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        ImGui::Text("确认删除？");
        ImGui::Separator();
        if (ImGui::Button("取消", ImVec2{ 100, 0 }))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("确认", ImVec2{ 100, 0 }))
        {
            if (fs::exists(explorer_popup_file_path))
            {
                fs::remove_all(explorer_popup_file_path);
                const auto file_found = breakpoints.find(explorer_popup_file_path);
                if (file_found != breakpoints.end())
                {
                    breakpoints.erase(file_found);
                }
                if (explorer_popup_file_path == explorer_select_path)
                {
                    explorer_select_path.clear();
                    update_editor_content();
                }
            }
            require_flush_explorer = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}
