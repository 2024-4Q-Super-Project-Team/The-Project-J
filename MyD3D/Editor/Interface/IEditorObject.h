#pragma once

class IEditorObject
{
public:
	virtual void EditorRendering() = 0;
};

namespace Editor
{
    class IWidget
    {
    public:
        virtual void Render() = 0;
    protected:
    };
}

#define EDITOR_FLAG_MAIN ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen
#define EDITOR_FLAG_RESOURCE  ImGuiTreeNodeFlags_Selected

#define EDITOR_COLOR_EXTRA		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
#define EDITOR_COLOR_RESOURCE	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.0f, 0.4f, 0.2f, 1.0f));
#define EDITOR_COLOR_POP(num)	ImGui::PopStyleColor(num);

