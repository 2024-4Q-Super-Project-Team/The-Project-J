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
    // ���ҽ��� �����´�.
    template <typename TYPE>
    static std::shared_ptr<TYPE> GetResource(const std::wstring& _key);
    // ���ҽ��� �����.
    template <typename TYPE, typename... Args>
    static std::shared_ptr<TYPE> AddResource(const std::wstring& _key, Args... _args);
    // ���ҽ��� �ִ´�. (shader_ptr�� �����Ƿ� �̹� ������ ���ҽ��� �������� �� ����ؾ��Ѵ�.)
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
    return nullptr; // nullptr ��ȯ
}

template<typename TYPE, typename ...Args>
inline std::shared_ptr<TYPE> ResourceManager::AddResource(const std::wstring& _key, Args ..._args)
{
    std::shared_ptr<TYPE> pResource;
    UINT id = static_cast<UINT>(TYPE::GetType());
    auto itr = mResourceTables[id].find(_key);
    if (FIND_SUCCESS(itr, mResourceTables[id]))
    {
        // ���� ���ҽ��� �˻��� �Ǹ� ���� ī��Ʈ�� ����.
        // 1�̻��̸� �����ϰ� 0�̸� ����� �ٽ� �������Ѵ�.
        pResource = std::dynamic_pointer_cast<TYPE>(itr->second.lock());
        if (pResource)
            return pResource;  // �̹� �����ϸ� ��ȯ
        mResourceTables[id].erase(itr);  // ���� ī��Ʈ�� 0�̸� ����� �ٽ� ����
    }
    pResource = std::make_shared<TYPE>(_key, std::forward<Args>(_args)...);
    mResourceTables[id][_key] = pResource;
    return pResource;
}

template<typename TYPE>
inline BOOL ResourceManager::PushResource(std::shared_ptr<TYPE> _pResource)
{
    UINT id = static_cast<UINT>(TYPE::GetType());
    // �˻��ؼ� �̹� ������ ����ϴ�
    auto itr = mResourceTables[id].find(_pResource->GetName());
    if (FIND_SUCCESS(itr, mResourceTables[id]))
    {
        return FALSE;
    }
    mResourceTables[id][_pResource->GetName()] = _pResource;
    return TRUE;
}