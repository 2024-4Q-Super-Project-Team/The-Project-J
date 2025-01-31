#pragma once
#include "Resource/Resource.h"

class FontResource
	: public Resource
{
public:
	RESOURCE_TYPE(FontResource);
	explicit FontResource(ResourceHandle _handle);
	virtual ~FontResource();
public:
	virtual json Serialize();
	virtual void Deserialize(json& j);
public:
	virtual void EditorRendering(EditorViewerType _viewerType) override;
};

