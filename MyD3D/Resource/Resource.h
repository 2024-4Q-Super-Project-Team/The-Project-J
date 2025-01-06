#pragma once

enum class eResourceType
{
    VertexShader,
    PixelShader,
    InputLayout,
    SamplerState,
    FBXModel,
    Mesh,
    Material,
    Texture2D,
    Animation,
    Bone,
    Prefab,
    AudioClip,
    Font,
    SIZE,
};

#define RESOURCE_TYPE(type) inline static const eResourceType GetType() { return type; }

class Resource
{
public:
    explicit Resource(std::wstring_view _name);
    virtual ~Resource();
public:
    inline const std::wstring& GetName() { return mName; }
protected:
    std::wstring    mName;
};

