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
    static void Reset();    // 런타임중엔 절!대! 실행하지 마세요
    static void Reload();   

    static void SaveResources();
    static void LoadResources();
public:
    template <typename TYPE>
    static TYPE*    GetResource(ResourceHandle _handle);
    // 리소스를 할당합니다.
    static void     Alloc_Resource(ResourceHandle _handle);
    static void     Alloc_All_Resource();
    // 리소스를 해제합니다.
    static BOOL     Free_Resource(ResourceHandle _handle);
    static void     Free_All_Resource();
    // 리소스 핸들을 등록합니다. 이미 있으면 무시됨.
    static BOOL     RegisterResourceHandle(ResourceHandle _handle);
    // 리소스 핸들을 삭제합니다.
    static BOOL     UnRegisterResourceHandle(ResourceHandle _handle);
    // 리소스를 넣습니다. (핸들만으로 생성하지 못하는 리소스를 위해)
    template <typename TYPE>
    static void     PushResource(TYPE* _pResource);
public:
    static std::unordered_map<ResourceHandle, Resource*>&   GetResourceTable(eResourceType _type);
    static std::pair<BOOL, ResourceHandle>                  GetResourceHandleFromMainKey(const std::wstring _key);
    static std::pair<BOOL, ResourceHandle>                  GetResourceHandleFromPath(const std::wstring _key);
    static std::unordered_set<ResourceHandle>&              GetLoadResourceList();
private:
    // 핸들->리소스(실제 사용 리소스)를 매핑한 테이블
    static std::unordered_map<ResourceHandle, Resource*> mHandleFromResourceMappingTable[static_cast<UINT>(eResourceType::SIZE)];
    // 메인 키->핸들 매핑테이블.
    static std::unordered_map<std::wstring, ResourceHandle> mHandleFromMainKeyMappingTable;
    // 경로->핸들 매핑테이블.
    static std::unordered_map<std::wstring, ResourceHandle> mHandleFromPathMappingTable;
    // 로드할 핸들을 모아둔 컨테이너(JSON_TODO : 직렬화로 저장, 불러오기 필요)
    static std::unordered_set<ResourceHandle> mLoadResourceList;    // 모든 핸들을 저장한게 아니라, 경로를 통해 직접 불러올 핸들만 모아둔 컨테이너 (ex. Mesh, Material은 fbx를 통해 로드하니까 여기에 포함되지 않음)
public:
    // 사용자가 준 핸들을 등록함과 동시에 Alloc, LoadList에넣어줍니다.
    static BOOL LoadFileFromHandle(const ResourceHandle& _handle);
    // Path를 통해 자동으로 핸들을 등록함과 동시에 Alloc, LoadList에넣어줍니다.
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

// 이미 만들어진 리소스를 넣기위해 만듬
// 핸들이 이미 있다면 덮어쓴다.
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