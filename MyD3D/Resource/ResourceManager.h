#pragma once
#include "Resource/Graphics/Texture/Texture.h"
#include "Resource/Graphics/FBXModel/FBXModel.h"
#include "Resource/Graphics/Mesh/Mesh.h"
#include "Resource/Graphics/Material/Material.h"
#include "Resource/Graphics/Animation/Animation.h"
#include "Resource/Prefab/Prefab.h"
#include "Resource/AudioResource/AudioResource.h"

class PrefabResource;

using ResourceTable = std::unordered_map<ResourceHandle, Resource*>;

// �޽� ��Ƽ���� ���� �Ҵ�� ������ fbxModel�� ����Ѵ�.

class ResourceManager
{
public:
    explicit ResourceManager() = default;
    virtual ~ResourceManager() = default;
public:
    static BOOL Initialize();
    static void Finalization();
public:
    template <typename TYPE>
    static TYPE*    GetResource(ResourceHandle _handle);
    // ���ҽ��� �Ҵ��մϴ�.
    template <typename TYPE>
    static TYPE*    Alloc_Resource(ResourceHandle _handle);
    // ���ҽ��� �����մϴ�.
    static BOOL     Free_Resource(ResourceHandle _handle);
    // ���ҽ� �ڵ��� ����մϴ�. �̹� ������ ���õ�.
    static void     RegisterResourceHandle(ResourceHandle _handle);
    // ���ҽ� �ڵ��� �����մϴ�.
    static void     UnRegisterResourceHandle(ResourceHandle _handle);
    // ���ҽ��� �ֽ��ϴ�. (�ڵ鸸���� �������� ���ϴ� ���ҽ��� ����)
    template <typename TYPE>
    static void     PushResource(TYPE* _pResource);
public:
    static ResourceTable& GetResourceTable(eResourceType _type) { return mResourceTables[static_cast<UINT>(_type)]; }
private:
    static ResourceTable mResourceTables[static_cast<UINT>(eResourceType::SIZE)];
public:
    
};

template<typename TYPE>
inline TYPE* ResourceManager::GetResource(ResourceHandle _handle)
{
    ResourceTable& table = GetResourceTable(_handle.GetResourceType());
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

template<typename TYPE>
inline TYPE* ResourceManager::Alloc_Resource(ResourceHandle _handle)
{
    ResourceTable& table = GetResourceTable(_handle.GetResourceType());
    auto itr = table.find(_handle);
    if (FIND_SUCCESS(itr, table))
    {
        TYPE* pResource = new TYPE(_handle);
        table[_handle] = pResource;
        return pResource;
    }
    return nullptr;
}

template<typename TYPE>
inline void ResourceManager::PushResource(TYPE* _pResource)
{
    if (_pResource)
    {
        ResourceHandle handle = _pResource->GetHandle();
        RegisterResourceHandle(handle);
        ResourceTable& table = GetResourceTable(handle.GetResourceType());
        table[handle] = _pResource;
    }
}