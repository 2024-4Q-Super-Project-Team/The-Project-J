#include "pch.h"
#include "ResourceManager.h"
#include "Resource/FBXImporter/FBXImporter.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"
#include "Object/Object.h"

std::unordered_map<ResourceHandle, Resource*>       ResourceManager::mHandleFromResourceMappingTable[static_cast<UINT>(eResourceType::SIZE)] = {};
std::unordered_map<std::wstring, ResourceHandle>    ResourceManager::mHandleFromMainKeyMappingTable;
std::unordered_map<std::wstring, ResourceHandle>    ResourceManager::mHandleFromPathMappingTable;
std::vector<ResourceHandle>                         ResourceManager::mLoadResourceList;

BOOL ResourceManager::Initialize()
{
    FBXImporter::Initialize();

    // ���⼭ json�� �о� mLoadResourceList�� �ڵ������� ��´�.
    // �׸��� �ε��Ѵ�.
    Reload();

    return TRUE;
}

void ResourceManager::Finalization()
{
    FBXImporter::Finalizaiton();
}

void ResourceManager::Reset()
{
    for (auto& table : mHandleFromResourceMappingTable)
    {
        for (auto& [handle, resource] : table)
        {
            SAFE_DELETE(resource);
        }
        table.clear();
    }
}

void ResourceManager::Reload()
{
    // ���� -> json �ٽ� �ҷ����� -> ���� Alloc
    
    Reset();    // ����
    // JSON_TODO : ���⿡ json�ҷ����� �Լ� �־��ּ��� ��
    Alloc_All_Resource(); // ���� Alloc

    // ��ī�̹ڽ� �޽�
    MeshResource::InitSkyCubeMesh();
    PushResource<MeshResource>(MeshResource::SkyCubeMesh);
    // ť�� �޽�
    MeshResource::InitCubeMesh();
    PushResource<MeshResource>(MeshResource::CubeMesh);
    // �÷��� �޽�
    MeshResource::InitPlainMesh();
    PushResource<MeshResource>(MeshResource::PlainMesh);
    // �⺻ ��Ƽ����
    MaterialResource::InitDefaultMaterial();
    PushResource<MaterialResource>(MaterialResource::DefaultMaterial);

}

void ResourceManager::Alloc_All_Resource()
{
    for (auto& handle : mLoadResourceList)
    {
        LoadFileFromHandle(handle);
    }
}

BOOL ResourceManager::Free_Resource(ResourceHandle _handle)
{
    auto& table = GetResourceTable(_handle.GetResourceType());
    auto itr = table.find(_handle);
    if (FIND_SUCCESS(itr, table))
    {
        if (table[_handle] == nullptr)
        {
            return FALSE;
        }
        delete table[_handle];
        table[_handle] = nullptr;
        Display::Console::Log("Free_Resource -  MainKey : ", _handle.GetKey(), ", Path : ", _handle.GetPath(), '\n');
        return TRUE;
    }
    return FALSE;
}

void ResourceManager::Free_All_Resource()
{
    for (auto& table : mHandleFromResourceMappingTable)
    {
        for (auto& [handle, resource] : table)
        {
            SAFE_DELETE(resource);
        }
    }
}

BOOL ResourceManager::RegisterResourceHandle(ResourceHandle _handle)
{
    auto& table = GetResourceTable(_handle.GetResourceType());
    auto itr = table.find(_handle);
    if (FIND_FAILED(itr, table))
    {
        table[_handle] = nullptr;
        mHandleFromMainKeyMappingTable[_handle.GetKey()] = _handle;
        mHandleFromPathMappingTable[_handle.GetPath()] = _handle;
        return TRUE;
    }
    else
    {
        Display::Console::Log("=====================================================\n");
        Display::Console::Log("Warning - ResourceHandle is already used. \n");
        Display::Console::Log("MainKey : ", _handle.GetKey(), '\n');
        Display::Console::Log("SubKey : ", _handle.GetSubKey(), '\n');
        Display::Console::Log("FilePath : ", _handle.GetPath(), '\n');
        Display::Console::Log("=====================================================\n");
        return FALSE;
    }
}

BOOL ResourceManager::UnRegisterResourceHandle(ResourceHandle _handle)
{
    {
        auto& table = GetResourceTable(_handle.GetResourceType());
        auto itr = table.find(_handle);
        if (FIND_SUCCESS(itr, table))
        {
            SAFE_DELETE(itr->second);
            table.erase(itr);
        }
        else
        {
            return FALSE;
        }
    }
    {
        auto itr = mHandleFromMainKeyMappingTable.find(_handle.GetKey());
        if (FIND_SUCCESS(itr, mHandleFromMainKeyMappingTable))
        {
            mHandleFromMainKeyMappingTable.erase(itr);
        }
    }
    {
        auto itr = mHandleFromPathMappingTable.find(_handle.GetPath());
        if (FIND_SUCCESS(itr, mHandleFromPathMappingTable))
        {
            mHandleFromPathMappingTable.erase(itr);
        }
    }
    return TRUE;
}

std::unordered_map<ResourceHandle, Resource*>& ResourceManager::GetResourceTable(eResourceType _type)
{
    return mHandleFromResourceMappingTable[static_cast<UINT>(_type)];
}

std::pair<BOOL, ResourceHandle> ResourceManager::GetResourceHandleFromMainKey(const std::wstring _key)
{
    BOOL result = FALSE;
    ResourceHandle handle = {};
    auto itr = Helper::FindMap(_key, mHandleFromMainKeyMappingTable);
    if (itr)
    {
        result = TRUE;
        handle = *itr;
    }
    return std::make_pair(result, handle);
}

std::pair<BOOL, ResourceHandle> ResourceManager::GetResourceHandleFromPath(const std::wstring _key)
{
    BOOL result = FALSE;
    ResourceHandle handle = {};
    auto itr = Helper::FindMap(_key, mHandleFromPathMappingTable);
    if (itr)
    {
        result = TRUE;
        handle = *itr;
    }
    return std::make_pair(result, handle);
}

std::vector<ResourceHandle>& ResourceManager::GetLoadResourceList()
{
    return mLoadResourceList;
}

#define REGISTER_AND_ALLOC_RESOUCE_FROM_HANDLE(type) \
if(_handle.GetResourceType() == eResourceType::type)\
if(ResourceManager::RegisterResourceHandle(_handle)){\
ResourceManager::Alloc_Resource<type>(_handle);\
ResourceManager::GetLoadResourceList().push_back(_handle);\
return TRUE; \
} else { \
return FALSE;} \

BOOL ResourceManager::LoadFileFromHandle(const ResourceHandle& _handle)
{
    REGISTER_AND_ALLOC_RESOUCE_FROM_HANDLE(FBXModelResource);
    REGISTER_AND_ALLOC_RESOUCE_FROM_HANDLE(Texture2DResource);
    REGISTER_AND_ALLOC_RESOUCE_FROM_HANDLE(AudioResource);
    return FALSE;
}

BOOL ResourceManager::LoadFileFromPath(const std::wstring& _path)
{
    std::wstring fileExt;
    std::wstring fileName;
    Helper::GetExtFromFilePath(_path, fileExt);
    Helper::GetFileNameFromFilePath(_path, fileName);

    ResourceHandle handle = { eResourceType::SIZE , fileName, L"", _path };
    if (fileExt == L".fbx" || fileExt == L".FBX")
        handle.mResourceType = eResourceType::FBXModelResource;
    if (fileExt == L".png" || fileExt == L".jpg" || fileExt == L".dds" || fileExt == L".tga")
        handle.mResourceType = eResourceType::Texture2DResource;
    if (fileExt == L".ogg" || fileExt == L".mp3" || fileExt == L".wav")
        handle.mResourceType = eResourceType::AudioResource;

    if (handle.GetResourceType() == eResourceType::SIZE)
    {
        Display::Console::Log(L"This extension is not supported. �������� �ʴ� Ȯ���ڰų� ����Դϴ�. (", _path, L")");
        return FALSE;
    }
    else
    {
        return ResourceManager::LoadFileFromHandle(handle);
    }
}
