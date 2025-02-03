#include "pch.h"
#include "Texture.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"

Texture2DResource::Texture2DResource(ResourceHandle _handle)
	: Resource(_handle)
	, Texture(new D3DGraphicsImg(_handle.GetPath()))
{
	SetEID("Texture : " + Helper::ToString(_handle.GetKey()));
	Display::Console::Log("create\n");
}

Texture2DResource::~Texture2DResource()
{
	Texture->Reset();
	SAFE_RELEASE(Texture);
}

json Texture2DResource::Serialize()
{
	json ret;
	ret["id"] = GiveId();

	return json();
}

void Texture2DResource::Deserialize(json& j)
{
	if (j.contains("id"))
		SetId(j["id"].get<unsigned int>());
}

void Texture2DResource::EditorRendering(EditorViewerType _viewerType)
{
	//std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
	//std::string name = Helper::ToString(GetKey());
	switch (_viewerType)
	{
	case EditorViewerType::DEFAULT:
	{
		std::string name = Helper::ToString(GetKey());
		ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_RESOURCE);
		auto flags = ImGuiSelectableFlags_AllowDoubleClick;
		if (ImGui::Selectable(GetEID(), false, flags))
		{
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				EditorManager::GetInspectorViewer()->SetFocusObject(this);
			}
		}
		EditorItemState state;
		state.mResourcePtr = this;
		state.mName = Helper::ToString(mHandle.GetKey());
		EditorDragNDrop::SendDragAndDropData(GetEID(), state);
		EDITOR_COLOR_POP(1);
		break;
	}
	case EditorViewerType::HIERARCHY:
		break;
	case EditorViewerType::INSPECTOR:
	{
		Resource::EditorRendering(_viewerType);

		ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_RESOURCE);
		ImGui::Image((ImTextureID)Texture->mSRV, ImVec2(150, 150));
		EDITOR_COLOR_POP(1);
		break;
	}
	default:
		break;
	}
}
