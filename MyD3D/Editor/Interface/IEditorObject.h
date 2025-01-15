#pragma once

class IEditorObject
{
public:
	virtual void EditorRendering() = 0;
};

#define EDITOR_COLOR_EXTRA ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
#define EDITOR_COLOR_RESOURCE ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.0f, 0.4f, 0.2f, 1.0f));
#define EDITOR_COLOR_POP(num) ImGui::PopStyleColor(num);