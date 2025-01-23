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
    ResourceHandle(const ResourceHandle&) = default;  // ���� ������
    ResourceHandle(ResourceHandle&&) noexcept = default; // �̵� ������
    ResourceHandle& operator=(const ResourceHandle&) = default;  // ���� ���� ������
    ResourceHandle& operator=(ResourceHandle&&) noexcept = default; // �̵� ���� ������
public:
    eResourceType   mResourceType = eResourceType::SIZE;    // ���ҽ� Ÿ��
    std::wstring    mMainKey = L"";		                    // ���ҽ� �Ŵ������� �� Ű ��.
    std::wstring    mSubKey = L"";		                    // �޽��� ���, fbxModel������ ����ϴ� key���� ���� �ҷ��;� �ϹǷ�, fbxModel������ ����ϴ� key���� �����Ѵ�.
    std::wstring    mPath = L"";			                // ���ҽ��� �ҷ����� ���� ���(FBX�𵨾ȿ� �ִ� �޽��� ���, �� ���� fbx�� ��δ�.)
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

/*���ҽ��� ���� ���....
���ҽ��� �����Ϳ��� ���ҽ��� ���� �̸� �ε��Ѵ�.
���ҽ��Ŵ����� Ű ���� ���� ���ҽ��� �ش�.
���ҽ��� Key�� Path, �׸��� �� �޸� ���ҽ��� ����ִ´�.
�׸��� �� �޸𸮸� ����ִ� �������� nullptr���θ� ���� �̿� ���� ���θ� Ȯ���� �� �ְ� �Ѵ�.(weak_ptr�� lock�� �����ϴ�)

���ҽ� �Ŵ����� ������ �ٲ�� �Ѵ�!
�㳪 �������� weak_ptr�� lock�� �Ұ��ϸ� ��������, ������ ���������� �����Ѵ�.
��� ���ҽ��� �� �޸𸮸� �����ְ� nullptr�� �����.
*/ 