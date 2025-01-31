#pragma once
#include "Resource/Graphics/Texture/Texture.h"
#include "Resource/Graphics/FBXModel/FBXModel.h"
#include "Resource/Graphics/Mesh/Mesh.h"
#include "Resource/Graphics/Material/Material.h"
#include "Resource/Graphics/Animation/Animation.h"
#include "Resource/Prefab/Prefab.h"
#include "Resource/AudioResource/AudioResource.h"
#include "Resource/FontResource/FontResource.h"

class ResourceManager
{
public:
    explicit ResourceManager() = default;
    virtual ~ResourceManager() = default;
public:
    static BOOL Initialize();
    static void Finalization();
    static void Reset();    // ��Ÿ���߿� ��!��! �������� ������
    static void Reload();   

    static void SaveResources();
    static void LoadResources();
public:
    template <typename TYPE>
    static TYPE*    GetResource(ResourceHandle _handle);
    // ���ҽ��� �Ҵ��մϴ�.
    static void     Alloc_Resource(ResourceHandle _handle);
    static void     Alloc_All_Resource();
    // ���ҽ��� �����մϴ�.
    static BOOL     Free_Resource(ResourceHandle _handle);
    static void     Free_All_Resource();
    // ���ҽ� �ڵ��� ����մϴ�. �̹� ������ ���õ�.
    static BOOL     RegisterResourceHandle(ResourceHandle _handle);
    // ���ҽ� �ڵ��� �����մϴ�.
    static BOOL     UnRegisterResourceHandle(ResourceHandle _handle);
    // ���ҽ��� �ֽ��ϴ�. (�ڵ鸸���� �������� ���ϴ� ���ҽ��� ����)
    template <typename TYPE>
    static void     PushResource(TYPE* _pResource);
public:
    static std::unordered_map<ResourceHandle, Resource*>&   GetResourceTable(eResourceType _type);
    static std::pair<BOOL, ResourceHandle>                  GetResourceHandleFromMainKey(const std::wstring _key);
    static std::pair<BOOL, ResourceHandle>                  GetResourceHandleFromPath(const std::wstring _key);
    static std::unordered_set<ResourceHandle>&              GetLoadResourceList();
private:
    // �ڵ�->���ҽ�(���� ��� ���ҽ�)�� ������ ���̺�
    static std::unordered_map<ResourceHandle, Resource*> mHandleFromResourceMappingTable[static_cast<UINT>(eResourceType::SIZE)];
    // ���� Ű->�ڵ� �������̺�.
    static std::unordered_map<std::wstring, ResourceHandle> mHandleFromMainKeyMappingTable;
    // ���->�ڵ� �������̺�.
    static std::unordered_map<std::wstring, ResourceHandle> mHandleFromPathMappingTable;
    // �ε��� �ڵ��� ��Ƶ� �����̳�(JSON_TODO : ����ȭ�� ����, �ҷ����� �ʿ�)
    static std::unordered_set<ResourceHandle> mLoadResourceList;    // ��� �ڵ��� �����Ѱ� �ƴ϶�, ��θ� ���� ���� �ҷ��� �ڵ鸸 ��Ƶ� �����̳� (ex. Mesh, Material�� fbx�� ���� �ε��ϴϱ� ���⿡ ���Ե��� ����)
public:
    // ����ڰ� �� �ڵ��� ����԰� ���ÿ� Alloc, LoadList���־��ݴϴ�.
    static BOOL LoadFileFromHandle(const ResourceHandle& _handle);
    // Path�� ���� �ڵ����� �ڵ��� ����԰� ���ÿ� Alloc, LoadList���־��ݴϴ�.
    static BOOL LoadFileFromPath(const std::wstring& _path);

private:
    static std::string mSaveFilePath;
};

template<typename TYPE>
inline TYPE* ResourceManager::GetResource(ResourceHandle _handle)
{
    auto& table = GetResourceTable(_handle.GetResourceType());
    auto itr = table.find(_handle);
    if (FIND_SUCCESS(itr, table))
    {
        TYPE* pResource = dynamic_cast<TYPE*>(itr->second);
        if (pResource)
        {
            return pResource;
        }
        return nullptr;
    }
    return nullptr;
}

// �̹� ������� ���ҽ��� �ֱ����� ����
// �ڵ��� �̹� �ִٸ� �����.
template<typename TYPE>
inline void ResourceManager::PushResource(TYPE* _pResource)
{
    if (_pResource)
    {
        ResourceHandle handle = _pResource->GetHandle();
        RegisterResourceHandle(handle);
        auto& table = GetResourceTable(handle.GetResourceType());
        table[handle] = _pResource;
        Display::Console::Log("Alloc_Resource - MainKey : ", handle.GetKey(), ", Path : ", handle.GetPath(), '\n');
    }
}