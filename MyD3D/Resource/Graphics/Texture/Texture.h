#pragma once
#include "Resource/Resource.h"

class D3DGraphicsImg;

class Texture2D
    : public Resource
	, public IEditorObject
{
public:
    RESOURCE_TYPE(eResourceType::Texture2D);
    explicit Texture2D(std::wstring_view _path);
    virtual ~Texture2D();
public:
    D3DGraphicsImg* Texture;
public:
	virtual void EditorRendering() override;
};