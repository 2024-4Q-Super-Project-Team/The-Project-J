#pragma once

enum class eResourceType
{
    FBXModel,
    Mesh,
    Material,
    Texture2D,
    Animation,
    Bone,
    SkyBox,
    Prefab,
    AudioResource,
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

