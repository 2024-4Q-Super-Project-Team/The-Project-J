#include "pch.h"
#include "Resource.h"

Resource::Resource(ResourceHandle _handle)
    : mHandle(_handle)
{
}

void Resource::EditorRendering(EditorViewerType _viewerType)
{
	ImGui::Text(("Main Key : " + Helper::ToString(mHandle.GetKey())).c_str());
	if (mHandle.GetSubKey().empty() == false)
	{
		ImGui::Text(("Sub Key : " + Helper::ToString(mHandle.GetSubKey())).c_str());
	}
	if (mHandle.GetPath().empty() == false)
	{
		ImGui::Text(("Path : " + Helper::ToString(mHandle.GetPath())).c_str());
	}
	ImGui::Separator();
}

const char* ResourceTypeToStr(eResourceType _type)
{
	switch (_type)
	{
		RESOURCE_TYPE_TO_STR_DEF(FBXModelResource);
		RESOURCE_TYPE_TO_STR_DEF(MeshResource);
		RESOURCE_TYPE_TO_STR_DEF(MaterialResource);
		RESOURCE_TYPE_TO_STR_DEF(Texture2DResource);
		RESOURCE_TYPE_TO_STR_DEF(AnimationResource);
		RESOURCE_TYPE_TO_STR_DEF(AudioResource);
		RESOURCE_TYPE_TO_STR_DEF(FontResource);
		RESOURCE_TYPE_TO_STR_DEF(PrefabResource);
	default:
		break;
	}
}

json ResourceHandle::Serialize()
{
	// JSON_TODO :
	// Type(int), MainKey, SubKey, Path
	return json();
}

void ResourceHandle::Deserialize(json& j)
{
}
