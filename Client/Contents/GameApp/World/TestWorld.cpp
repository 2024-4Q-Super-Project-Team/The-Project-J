#include "pch.h"
#include "TestWorld.h"
#include "../Script/ModelSpawner.h"
#include "../Script/CameraController.h"


void _CALLBACK TestWorld::OnCreate()
{
    CreateObjectGroup(L"Default", L"");
    CreateObjectGroup(L"Test", L"");

    return void _CALLBACK();
}

void _CALLBACK TestWorld::OnDestroy()
{
    return void _CALLBACK();
}

void _CALLBACK TestWorld::OnEnable()
{
    auto pSkyBox = GetSkyBox();
    pSkyBox->SetEnvironmentTexture(ResourceManager::AddResource<Texture2D>(L"resource/texture/Skybox/DefaultSkyIBLMapEnvHDR.dds"));
    pSkyBox->SetDiffuseTexture(ResourceManager::AddResource<Texture2D>(L"resource/texture/Skybox/DefaultSkyIBLMapDiffuseHDR.dds"));
    pSkyBox->SetSpecularture(ResourceManager::AddResource<Texture2D>(L"resource/texture/Skybox/DefaultSkyIBLMapSpecularHDR.dds"));
    pSkyBox->SetLookUpTableTexture(ResourceManager::AddResource<Texture2D>(L"resource/texture/Skybox/DefaultSkyIBLMapBrdf.dds"));
    
    ResourceManager::AddResource<FBXModelResource>(L"resource/fbx/gun.fbx");
    ResourceManager::AddResource<FBXModelResource>(L"resource/fbx/Jinx Default.fbx");
    ResourceManager::AddResource<FBXModelResource>(L"resource/fbx/sphere.fbx");
    ResourceManager::AddResource<FBXModelResource>(L"resource/fbx/skyBox.obj");

    ObjectGroup* pGroup = GetObjectGroup(L"Test");
    if (pGroup)
    {
        Object* clone;
        clone = pGroup->CreateObject(L"ModelController", L"Default");
        clone->AddComponent<ModelSpawner>();

        clone = pGroup->CreateObject(L"CameraController", L"Default");
        clone->AddComponent<CameraController>();
    }
    return void _CALLBACK();
}
