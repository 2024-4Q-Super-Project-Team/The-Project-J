#pragma once
//ImGuiSelectableFlags_AllowDoubleClick
namespace Editor
{
	bool BeginResourceBar(const char* _label, ImGuiSelectableFlags _flags)
	{
		bool isOpen = ImGui::Selectable(_label, &isOpen, _flags);
		if (isOpen)
		{
			ImGui::Indent();
			return true;
		}
		return false;
	};
	void EndResourceBar()
	{
		ImGui::Unindent();
	}
}


