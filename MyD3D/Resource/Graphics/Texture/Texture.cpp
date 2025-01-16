#include "pch.h"
#include "Texture.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"

Texture2D::Texture2D(std::wstring_view _path)
    : Resource(_path)
    , Texture(new D3DGraphicsImg(_path))
{
}

Texture2D::~Texture2D()
{
    SAFE_RELEASE(Texture);
}

void Texture2D::EditorRendering()
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
	std::string name = Helper::ToString(mName);
	EDITOR_COLOR_RESOURCE;
	if (ImGui::TreeNodeEx(("Texture2D : " + name + uid).c_str(), EDITOR_FLAG_RESOURCE))
	{
		ImGui::Image((ImTextureID)Texture->mSRV, ImVec2(150, 150));
		ImGui::TreePop();
	}
	EDITOR_COLOR_POP(1);
}
