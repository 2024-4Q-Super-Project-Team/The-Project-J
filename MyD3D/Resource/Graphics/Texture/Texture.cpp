#include "pch.h"
#include "Texture.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"

Texture2DResource::Texture2DResource(ResourceHandle _handle)
    : Resource(_handle)
    , Texture(new D3DGraphicsImg(_handle.GetPath()))
{
}

Texture2DResource::~Texture2DResource()
{
    SAFE_RELEASE(Texture);
}

void Texture2DResource::EditorRendering(EditorViewerType _viewerType)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
	std::string name = Helper::ToString(GetKey());
	ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_RESOURCE);
	if (ImGui::TreeNodeEx(("Texture2DResource : " + name + uid).c_str(), EDITOR_FLAG_RESOURCE))
	{
		ImGui::Image((ImTextureID)Texture->mSRV, ImVec2(150, 150));
		ImGui::TreePop();
	}
	EDITOR_COLOR_POP(1);
}
