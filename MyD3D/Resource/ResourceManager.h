#pragma once
#include "Resource/Graphics/Texture/Texture.h"
#include "Resource/Graphics/FBXModel/FBXModel.h"
#include "Resource/Graphics/Mesh/Mesh.h"
#include "Resource/Graphics/Material/Material.h"
#include "Resource/Graphics/Bone/Bone.h"
#include "Resource/Graphics/Animation/Animation.h"
#include "Resource/Graphics/SkyBox/SkyBox.h"
#include "Resource/Prefab/Prefab.h"
#include "Resource/AudioResource/AudioResource.h"

class IInputLayout;
class IVertexShader;
class IPixelShader;
class ITexture;
class ITexture2D;
class ITexture3D;

class Prefab;

using ResourceTable = std::unordered_map<ResourceHandle, std::weak_ptr<Resource>>;

class ResourceManager
{
public:
    explicit ResourceManager() = default;
    virtual ~ResourceManager() = default;
public:
    static BOOL Initialize();
    static void Finalization();
public:
    // 리소스를 요청합니다. 참조카운트가 증가.
    template <typename TYPE>
    static std::shared_ptr<TYPE>    RequestResource(ResourceHandle _handle);
    // 리소스 핸들을 등록합니다.
    static void                     RegisterResourceHandle(ResourceHandle _handle);
    // 리소스 핸들을 삭제합니다.
    static void                     UnregisterResourceHandle(ResourceHandle _handle);
    // 리소스를 핸들 등록과 함께 생성해서 반환한다.
    template <typename TYPE, typename ...Args>
    static std::shared_ptr<TYPE>    RegisterResource(ResourceHandle _handle, Args ..._args);
    // 리소스를 넣는다. (shader_ptr로 넣으므로 이미 누군가 리소스를 소유중일 때 사용해야한다.)
    template <typename TYPE>
    static BOOL                     PushResource(std::shared_ptr<TYPE> _pResource);
private:
    static ResourceTable            mResourceTables[static_cast<UINT>(eResourceType::SIZE)];
public:
    inline static ResourceTable&    GetResourceTable(eResourceType _type) { return mResourceTables[static_cast<UINT>(_type)]; }
};

template<typename TYPE>
inline std::shared_ptr<TYPE> ResourceManager::RequestResource(ResourceHandle _handle)
{
    std::shared_ptr<TYPE> pResource;
    UINT slot = static_cast<UINT>(_handle.GetResourceType());
    auto itr = mResourceTables[slot].find(_handle);
    if (FIND_SUCCESS(itr, mResourceTables[slot]))
    {
        pResource = std::dynamic_pointer_cast<TYPE>(itr->second.lock());
        if (pResource == nullptr)
        {
            pResource = std::make_shared<TYPE>(_handle);
            mResourceTables[slot][_handle] = pResource;
        }
        return pResource;
    }
    return std::shared_ptr<TYPE>();
}

template<typename TYPE, typename ...Args>
inline std::shared_ptr<TYPE> ResourceManager::RegisterResource(ResourceHandle _handle, Args ..._args)
{
    std::shared_ptr<TYPE> pResource;
    UINT slot = static_cast<UINT>(TYPE::GetType());
    auto itr = mResourceTables[slot].find(_handle);
    if (FIND_SUCCESS(itr, mResourceTables[slot]))
    {
        if (false == itr->second.expired())
        {
            pResource = std::dynamic_pointer_cast<TYPE>(itr->second.lock());
            if (pResource != nullptr)
            {
                return pResource;
            }
        }
        return std::shared_ptr<TYPE>();
    }
    else
    {
        pResource = std::make_shared<TYPE>(_handle, std::forward<Args>(_args)...);
        mResourceTables[slot][_handle] = pResource;
        return pResource;
    }
}

template<typename TYPE>
inline BOOL ResourceManager::PushResource(std::shared_ptr<TYPE> _pResource)
{
    UINT slot = static_cast<UINT>(TYPE::GetType());
    // 검색해서 이미 있으면 곤란하다
    auto itr = mResourceTables[slot].find(_pResource->GetHandle());
    if (FIND_SUCCESS(itr, mResourceTables[slot]))
    {
        return FALSE;
    }
    mResourceTables[slot][_pResource->GetHandle()] = _pResource;
    return TRUE;
}    