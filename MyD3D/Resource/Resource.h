#pragma once

enum class eResourceType
{
    FBXModelResource,
    MeshResource,
    MaterialResource,
    Texture2DResource,
    AnimationResource,
    AudioResource,
    FontResource,
    PrefabResource,
    SIZE,
};

#define RESOURCE_TYPE(type)\
inline static const eResourceType GetType() { return eResourceType::type; }\
inline static const char* GetTypeName() { return #type; }\

class ResourceHandle
    //: public Engine::SaveBase
{
public:
    ResourceHandle(eResourceType _type, const std::wstring& _mainKey, const std::wstring& _subKey, const std::wstring& _path)
        : mResourceType(_type), mMainKey(_mainKey), mSubKey(_subKey), mPath(_path) {}
    ResourceHandle() = default;
    ResourceHandle(const ResourceHandle&) = default;  // 복사 생성자
    ResourceHandle(ResourceHandle&&) noexcept = default; // 이동 생성자
    ResourceHandle& operator=(const ResourceHandle&) = default;  // 복사 대입 연산자
    ResourceHandle& operator=(ResourceHandle&&) noexcept = default; // 이동 대입 연산자
public:
    eResourceType   mResourceType = eResourceType::SIZE;    // 리소스 타입
    std::wstring    mMainKey = L"";		                    // 리소스 매니저에서 쓸 키 값.
    std::wstring    mSubKey = L"";		                    // 메쉬의 경우, fbxModel내에서 사용하는 key값을 통해 불러와야 하므로, fbxModel내에서 사용하는 key값을 저장한다.
    std::wstring    mPath = L"";			                // 리소스를 불러오기 위한 경로(FBX모델안에 있는 메쉬의 경우, 이 값은 fbx의 경로다.)
public:
    inline const auto& GetResourceType()    const { return mResourceType; }
    inline const auto& GetKey()             const { return mMainKey; }
    inline const auto& GetSubKey()          const { return mSubKey; }
    inline const auto& GetPath()            const { return mPath; }
    bool operator==(const ResourceHandle& other) const {
        return mMainKey == other.mMainKey && mPath == other.mPath;
    }
};
namespace std {
    template <>
    struct hash<ResourceHandle> {
        size_t operator()(const ResourceHandle& handle) const {
            return hash<std::wstring>()(handle.GetKey());
        }
    };
}

class Resource
    : public IEditorObject
{
public:
    explicit Resource(ResourceHandle _handle);
    explicit Resource() = default;
    virtual ~Resource() = default;
public:
    inline const auto& GetHandle()  { return mHandle; }
    inline const auto& GetKey()     { return mHandle.GetKey(); }
    inline const auto& GetSubKey()     { return mHandle.GetSubKey(); }
    inline const auto& GetPath()    { return mHandle.GetPath(); }
public:
    virtual void Create() {}
public:
    virtual void EditorRendering(EditorViewerType _viewerType) override;
protected:
    ResourceHandle mHandle;
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