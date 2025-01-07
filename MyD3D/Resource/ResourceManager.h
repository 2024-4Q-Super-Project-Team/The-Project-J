#pragma once
#include "Resource/Graphics/Texture/Texture.h"
#include "Resource/Graphics/FBXModel/FBXModel.h"
#include "Resource/Graphics/Mesh/Mesh.h"
#include "Resource/Graphics/Material/Material.h"
#include "Resource/Graphics/Bone/Bone.h"
#include "Resource/Graphics/Animation/Animation.h"
#include "Resource/Prefab/Prefab.h"

class IInputLayout;
class IVertexShader;
class IPixelShader;
class ITexture;
class ITexture2D;
class ITexture3D;

class Prefab;

class ResourceManager
{
    using ResourceTable = std::unordered_map<std::wstring, std::weak_ptr<Resource>>;
public:
    explicit ResourceManager() = default;
    virtual ~ResourceManager() = default;
public:
    static BOOL Initialize();
    static void Finalization();
public:
    // 리소스를 가져온다.
    template <typename TYPE>
    static std::shared_ptr<TYPE> GetResource(const std::wstring& _key);
    // 리소스를 만든다.
    template <typename TYPE, typename... Args>
    static std::shared_ptr<TYPE> AddResource(const std::wstring& _key, Args... _args);
    // 리소스를 넣는다. (shader_ptr로 넣으므로 이미 누군가 리소스를 소유중일 때 사용해야한다.)
    template <typename TYPE>
    static BOOL                  PushResource(std::shared_ptr<TYPE> _pResource);
private:
    static ResourceTable         mResourceTables[static_cast<UINT>(eResourceType::SIZE)];
};

template<typename TYPE>
inline std::shared_ptr<TYPE> ResourceManager::GetResource(const std::wstring& _key)
{
    std::shared_ptr<TYPE> pResource;
    UINT id = static_cast<UINT>(TYPE::GetType());
    auto itr = mResourceTables[id].find(_key);
    if (FIND_SUCCESS(itr, mResourceTables[id]))
    {
        pResource = std::dynamic_pointer_cast<TYPE>(itr->second.lock());
        if (pResource)
            return pResource;
    }
    return nullptr; // nullptr 반환
}

template<typename TYPE, typename ...Args>
inline std::shared_ptr<TYPE> ResourceManager::AddResource(const std::wstring& _key, Args ..._args)
{
    std::shared_ptr<TYPE> pResource;
    UINT id = static_cast<UINT>(TYPE::GetType());
    auto itr = mResourceTables[id].find(_key);
    if (FIND_SUCCESS(itr, mResourceTables[id]))
    {
        // 만약 리소스가 검색이 되면 참조 카운트를 본다.
        // 1이상이면 리턴하고 0이면 지우고 다시 만들어야한다.
        pResource = std::dynamic_pointer_cast<TYPE>(itr->second.lock());
        if (pResource)
            return pResource;  // 이미 존재하면 반환
        mResourceTables[id].erase(itr);  // 참조 카운트가 0이면 지우고 다시 생성
    }
    pResource = std::make_shared<TYPE>(_key, std::forward<Args>(_args)...);
    mResourceTables[id][_key] = pResource;
    return pResource;
}

template<typename TYPE>
inline BOOL ResourceManager::PushResource(std::shared_ptr<TYPE> _pResource)
{
    UINT id = static_cast<UINT>(TYPE::GetType());
    // 검색해서 이미 있으면 곤란하다
    auto itr = mResourceTables[id].find(_pResource->GetName());
    if (FIND_SUCCESS(itr, mResourceTables[id]))
    {
        return FALSE;
    }
    mResourceTables[id][_pResource->GetName()] = _pResource;
    return TRUE;
}