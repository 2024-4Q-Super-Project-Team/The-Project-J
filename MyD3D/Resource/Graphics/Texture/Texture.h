#pragma once
#include "Resource/Resource.h"

class Texture2D
    : public Resource
{
public:
    RESOURCE_TYPE(eResourceType::Texture2D);
    explicit Texture2D(std::wstring_view _path);
    virtual ~Texture2D();
public:
    inline ITexture2D* GetTexture2D() { return mTexture2D; }
    void Bind();
    void Reset();
    Texture2D& SetShaderStage(eShaderStage _stage);
    Texture2D& SetSlot(UINT _slot);
private:
    IGraphicsRenderer* mRenderer;
    ITexture2D* mTexture2D;
};