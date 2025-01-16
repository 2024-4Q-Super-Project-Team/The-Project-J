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

#define RESOURCE_TYPE(type)\
inline static const eResourceType GetType() { return eResourceType::type; }\
inline static const char* GetTypeName() { return "type"; }\

class Resource
    : public IEditorObject
{
public:
    explicit Resource(std::wstring_view _name);
    virtual ~Resource();
public:
    inline const std::wstring& GetName() { return mName; }
protected:
    std::wstring mName;
};

#define RESOURCE_TYPE_TO_STR_DEF(type) \
case eResourceType::type:\
return #type;\
break;\

const char* ResourceTypeToStr(eResourceType _type);