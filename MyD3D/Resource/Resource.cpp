#include "pch.h"
#include "Resource.h"

Resource::Resource(std::wstring_view _name)
    : mName(_name)
{
}

Resource::~Resource()
{
}

const char* ResourceTypeToStr(eResourceType _type)
{
	switch (_type)
	{
		RESOURCE_TYPE_TO_STR_DEF(FBXModel);
		RESOURCE_TYPE_TO_STR_DEF(Mesh);
		RESOURCE_TYPE_TO_STR_DEF(Material);
		RESOURCE_TYPE_TO_STR_DEF(Texture2D);
		RESOURCE_TYPE_TO_STR_DEF(Animation);
		RESOURCE_TYPE_TO_STR_DEF(Bone);
		RESOURCE_TYPE_TO_STR_DEF(SkyBox);
		RESOURCE_TYPE_TO_STR_DEF(Prefab);
		RESOURCE_TYPE_TO_STR_DEF(AudioResource);
		RESOURCE_TYPE_TO_STR_DEF(Font);
	default:
		break;
	}
}
