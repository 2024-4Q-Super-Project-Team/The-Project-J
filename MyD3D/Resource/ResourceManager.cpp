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

    return FBXImporter::Initialize();
}

void ResourceManager::Finalization()
{
    FBXImporter::Finalizaiton();
}
