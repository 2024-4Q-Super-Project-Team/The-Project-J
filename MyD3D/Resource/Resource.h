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
inline static const char* GetTypeName() { return #type; }\

class Resource
    : public IEditorObject
{
public:
    explicit Resource(std::wstring_view _name);
    virtual ~Resource();
public:
    inline const std::wstring& GetName() { return mName; }
protected:
    std::wstring    mName;
};

#define RESOURCE_TYPE_TO_STR_DEF(type) \
case eResourceType::type:\
return #type;\
break;\

const char* ResourceTypeToStr(eResourceType _type);

/*리소스에 대한 고민....
리소스는 에디터에서 리소스뷰어를 통해 미리 로드한다.
리소스매니저는 키 값을 통해 리소스를 준다.
리소스는 Key와 Path, 그리고 실 메모리 리소스를 들고있는다.
그리고 실 메모리를 들고있는 포인터의 nullptr여부를 통해 이용 가능 여부를 확인할 수 있게 한다.(weak_ptr의 lock과 유사하다)

리소스 매니저의 로직을 바꿔야 한다!
허나 기존에는 weak_ptr의 lock이 불가하면 지웠지만, 이제는 지우지말고 유지한다.
대신 리소스의 실 메모리를 지워주고 nullptr로 만든다.
*/ 