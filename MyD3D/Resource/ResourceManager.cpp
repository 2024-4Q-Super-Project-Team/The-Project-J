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
    return FBXImporter::Initialize();
}

void ResourceManager::Finalization()
{
    FBXImporter::Finalizaiton();
}

BOOL ResourceManager::Free_Resource(ResourceHandle _handle)
{
    return 0;
}

void ResourceManager::RegisterResourceHandle(ResourceHandle _handle)
{
    ResourceTable& table = GetResourceTable(_handle.GetResourceType());
    auto itr = table.find(_handle);
    if (FIND_FAILED(itr, table))
    {
        table[_handle] = nullptr;
    }
}

void ResourceManager::UnRegisterResourceHandle(ResourceHandle _handle)
{
}

