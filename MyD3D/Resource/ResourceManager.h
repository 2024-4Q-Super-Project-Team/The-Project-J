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

// 메쉬 머티리얼 등의 할당과 해제는 fbxModel이 담당한다.

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
    // 리소스를 할당합니다.
    template <typename TYPE>
    static TYPE*    Alloc_Resource(ResourceHandle _handle);
    // 리소스를 해제합니다.
    static BOOL     Free_Resource(ResourceHandle _handle);
    // 리소스 핸들을 등록합니다. 이미 있으면 무시됨.
    static void     RegisterResourceHandle(ResourceHandle _handle);
    // 리소스 핸들을 삭제합니다.
    static void     UnRegisterResourceHandle(ResourceHandle _handle);
    // 리소스를 넣습니다. (핸들만으로 생성하지 못하는 리소스를 위해)
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