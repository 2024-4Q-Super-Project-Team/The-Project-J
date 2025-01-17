#include "pch.h"
#include "EditorCheckBox.h"

namespace Editor
{
	void CheckBox::Render()
	{
		std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
		if (mValue)
		{
			ImGui::Checkbox((mLabel + uid).c_str(), mValue);
		}
	}
}