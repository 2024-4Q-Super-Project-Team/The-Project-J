#include "pch.h"
#include "Resource.h"

Resource::Resource(ResourceHandle _handle)
    : mHandle(_handle)
{
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
