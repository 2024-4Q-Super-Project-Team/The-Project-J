#include "pch.h"
#include "Resource/FontResource/FontResource.h"
#include "Manager/GameManager.h"

FontResource::FontResource(ResourceHandle _handle)
	: Resource(_handle)
{
	SetEID("Font : " + Helper::ToString(_handle.GetKey()));
}

FontResource::~FontResource()
{
	//
}

json FontResource::Serialize()
{
	return json();
}

void FontResource::Deserialize(json& j)
{
}

void FontResource::EditorRendering(EditorViewerType _viewerType)
{
	switch (_viewerType)
	{
	case EditorViewerType::DEFAULT:
	{
		break;
	}
	case EditorViewerType::HIERARCHY:
	{
		break;
	}
	case EditorViewerType::INSPECTOR:
	{
		ImGui::Text("Font Resource : ");
		ImGui::SameLine();
		std::string path = Helper::ToString(GetPath());
		ImGui::Text(path.c_str());

		break;
	}
	default:
		break;
	}
}
