#include "pch.h"
#include "ResourceManager.h"
#include "Resource/FBXImporter/FBXImporter.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"
#include "Object/Object.h"

ResourceTable ResourceManager::mResourceTables[static_cast<UINT>(eResourceType::SIZE)] = {};

BOOL ResourceManager::Initialize()
{
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
    // �⺻ ��ī�̹ڽ�
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

