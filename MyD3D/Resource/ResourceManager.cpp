#include "pch.h"
#include "ResourceManager.h"
#include "Resource/FBXImporter/FBXImporter.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"
#include "Object/Object.h"

ResourceTable ResourceManager::mResourceTables[static_cast<UINT>(eResourceType::SIZE)] = {};

BOOL ResourceManager::Initialize()
{
    // 스카이박스 메쉬
    MeshResource::InitSkyCubeMesh();
    PushResource<MeshResource>(MeshResource::SkyCubeMesh);
    // 큐브 메쉬
    MeshResource::InitCubeMesh();
    PushResource<MeshResource>(MeshResource::CubeMesh);
    // 플레인 메쉬
    MeshResource::InitPlainMesh();
    PushResource<MeshResource>(MeshResource::PlainMesh);
    // 기본 머티리얼
    MaterialResource::InitDefaultMaterial();
    PushResource<MaterialResource>(MaterialResource::DefaultMaterial);
    // 기본 스카이박스
    SkyBox::InitSkyBoxResource();
    PushResource<SkyBox>(SkyBox::DefaultSkyBox);
    return FBXImporter::Initialize();
}

void ResourceManager::Finalization()
{
    FBXImporter::Finalizaiton();
}

void ResourceManager::RegisterResourceHandle(ResourceHandle _handle)
{
    UINT slot = static_cast<UINT>(_handle.GetResourceType());
    auto itr = mResourceTables[slot].find(_handle);
    if (FIND_FAILED(itr, mResourceTables[slot]))
    {
        mResourceTables[slot][_handle] = std::make_shared<Resource>();
    }
}

void ResourceManager::UnregisterResourceHandle(ResourceHandle _handle)
{
}

