#include "pch.h"
#include "ResourceManager.h"
#include "Resource/FBXImporter/FBXImporter.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"
#include "Object/Object.h"
#include "Component/Camera/SkyBox/SkyBox.h"

std::unordered_map<ResourceHandle, Resource*>       ResourceManager::mHandleFromResourceMappingTable[static_cast<UINT>(eResourceType::SIZE)] = {};
std::unordered_map<std::wstring, ResourceHandle>    ResourceManager::mHandleFromMainKeyMappingTable;
std::unordered_map<std::wstring, ResourceHandle>    ResourceManager::mHandleFromPathMappingTable;
std::unordered_set<ResourceHandle>                  ResourceManager::mLoadResourceList;
std::string                                         ResourceManager::mSaveFilePath = "../Save/";

BOOL ResourceManager::Initialize()
{
    FBXImporter::Initialize();

    // 여기서 json을 읽어 mLoadResourceList에 핸들정보를 담는다.
    // 그리고 로드한다.
    Reload();

    MeshResource::GetSkyCubeMesh();
    MeshResource::GetCubeMesh();
    MeshResource::GetPlainMesh();
    MaterialResource::GetDefaultMaterial();

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
    SkyBox::FreeDefaultSkyBox();
}

void ResourceManager::Reload()
{
    // 리셋 -> json 다시 불러오기 -> 전부 Alloc
    
    Reset();    // 리셋
    LoadResources();
    Alloc_All_Resource(); // 전부 Alloc

    std::ifstream audioFile(mSaveFilePath + "audios.json");
    json audioJson;
    if (audioFile.is_open())
    {
        audioFile >> audioJson;
        audioFile.close();
    }

    for (json& j : audioJson)
    {
        std::wstring mainKey = Helper::ToWString(audioJson["key"].get<std::string>());
        ResourceHandle handle = mHandleFromMainKeyMappingTable[mainKey];
        auto audio = GetResource<AudioResource>(handle);
        audio->Deserialize(j);
    }
}

void ResourceManager::SaveResources()
{
    json handleSaveJson;
    json audioSaveJson;
    for (ResourceHandle resourceHandle : mLoadResourceList)
    {
        handleSaveJson += resourceHandle.Serialize();
        auto fbx = GetResource<FBXModelResource>(resourceHandle);
        if(fbx) fbx->SaveJson();

        auto audio = GetResource<AudioResource>(resourceHandle);
        if(audio)  audioSaveJson += audio->Serialize();
    }

    std::ofstream file(mSaveFilePath + "resources.json");
    file << handleSaveJson.dump(4);
    file.close();

    std::ofstream file2(mSaveFilePath + "audios.json");
    file2 << audioSaveJson.dump(4);
    file2.close();
}

void ResourceManager::LoadResources()
{
    std::ifstream loadFile(mSaveFilePath + "resources.json");

    json loadJson;
    if (loadFile.is_open())
    {
        loadFile >> loadJson;
        loadFile.close();
    }

    for (json j : loadJson)
    {
        ResourceHandle handle;
        handle.Deserialize(j);
        mLoadResourceList.insert(handle);
    }

    SkyBox::GetDefaultSkyBox();
}

void ResourceManager::Alloc_All_Resource()
{
    for (ResourceHandle handle : mLoadResourceList)
    {
        LoadFileFromHandle(handle);

        auto fbx = GetResource<FBXModelResource>(handle);
        if(fbx) fbx->LoadJson();
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
        Display::Console::Log("Free_Resource - MainKey : ", _handle.GetKey(), ", Path : ", _handle.GetPath(), '\n');
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

std::unordered_set<ResourceHandle>& ResourceManager::GetLoadResourceList()
{
    return mLoadResourceList;
}

#define REGISTER_AND_ALLOC_RESOUCE_FROM_HANDLE(type) \
if(_handle.GetResourceType() == eResourceType::type)\
if(ResourceManager::RegisterResourceHandle(_handle)){\
ResourceManager::Alloc_Resource<type>(_handle);\
ResourceManager::GetLoadResourceList().insert(_handle);\
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
        Display::Console::Log(L"This extension is not supported. 지원하지 않는 확장자거나 경로입니다. (", _path, L")");
        return FALSE;
    }
    else
    {
        return ResourceManager::LoadFileFromHandle(handle);
    }
}
