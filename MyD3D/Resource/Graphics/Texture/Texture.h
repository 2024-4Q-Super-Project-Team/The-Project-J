#pragma once
#include "Resource/Resource.h"

class D3DGraphicsImg;

class Texture2DResource
    : public Resource
{
public:
    RESOURCE_TYPE(Texture2DResource);
    explicit Texture2DResource(ResourceHandle _handle);
    virtual ~Texture2DResource();
public:
    D3DGraphicsImg* Texture;
public:
	virtual void EditorRendering(EditorViewerType _viewerType) override;
};