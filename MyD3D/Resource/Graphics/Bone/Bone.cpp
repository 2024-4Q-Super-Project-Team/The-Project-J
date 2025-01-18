#include "pch.h"
#include "Bone.h"

Bone::Bone(std::wstring_view _name, Matrix _matrix)
    : Resource(_name)
    , mOffsetMatrix(_matrix)
{
}

Bone::~Bone()
{
}

void Bone::EditorRendering(EditorViewerType _viewerType)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
	std::string name = Helper::ToString(mName);
	EDITOR_COLOR_RESOURCE;
	if (ImGui::TreeNodeEx(("Bone : " + name + uid).c_str(), EDITOR_FLAG_RESOURCE))
	{
		ImGui::Text("Offset Matrix");
		ImGui::Text("X : %f, %f, %f, %f", mOffsetMatrix._11, mOffsetMatrix._12, mOffsetMatrix._13, mOffsetMatrix._14);
		ImGui::Text("Y : %f, %f, %f, %f", mOffsetMatrix._21, mOffsetMatrix._22, mOffsetMatrix._23, mOffsetMatrix._24);
		ImGui::Text("Z : %f, %f, %f, %f", mOffsetMatrix._31, mOffsetMatrix._32, mOffsetMatrix._33, mOffsetMatrix._34);
		ImGui::Text("W : %f, %f, %f, %f", mOffsetMatrix._41, mOffsetMatrix._42, mOffsetMatrix._43, mOffsetMatrix._44);
		ImGui::TreePop();
	}
	EDITOR_COLOR_POP(1);
}
