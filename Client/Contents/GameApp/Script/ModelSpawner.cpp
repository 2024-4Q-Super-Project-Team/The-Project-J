#include "pch.h"
#include "ModelSpawner.h"
#include "Contents/EditorApp/Script/EditorManager.h"

ModelSpawner::~ModelSpawner()
{
}

void ModelSpawner::Start()
{
    {   // Plain
        auto Instance = CreateObject(L"Plain", L"Default");
        Instance->transform->position = Vector3(0, -10, 0);
        Instance->transform->scale = Vector3(200, 200, 1);
        Instance->transform->SetEulerAnglesFromDegree(Vector3(90, 0, 0));
        MeshRenderer* pRenderer = Instance->AddComponent<MeshRenderer>();
        pRenderer->SetMesh(MeshResource::PlainMesh);
        pRenderer->SetMaterial(MaterialResource::DefaultMaterial);
    }

    {   // Gun
        auto ModelResource = ResourceManager::AddResource<FBXModelResource>(L"resource/fbx/Gun/gun.fbx");
        auto ModelPrefab = ResourceManager::AddResource<Prefab>(L"resource/fbx/gun.fbx", ModelResource);
        ModelPrefab->SetGroupName(L"Model_Group1");
        mFBXModelResources.push_back(ModelResource);
        mModelPrefabs.push_back(ModelPrefab);
        
        auto Instance = Instantiate(ModelPrefab.get());
        Instance->transform->scale = Vector3(0.2, 0.2, 0.2);
        Instance->transform->position = Vector3(0, 0, 0);

        MeshRenderer* pRenderer = FindObjectWithName(L"Cerberus00_Fixed")->GetComponent<MeshRenderer>();
        EditorManager::mDebugEditor->AddRenderModel(pRenderer);
    }
    {   // Sphere
        auto ModelResource = ResourceManager::AddResource<FBXModelResource>(L"resource/fbx/Sphere/sphere.fbx");
        auto ModelPrefab = ResourceManager::AddResource<Prefab>(L"resource/fbx/sphere.fbx", ModelResource);
        ModelPrefab->SetGroupName(L"Model_Group2");
        mFBXModelResources.push_back(ModelResource);
        mModelPrefabs.push_back(ModelPrefab);

        auto Instance = Instantiate(ModelPrefab.get());
        Instance->transform->scale = Vector3(0.2, 0.2, 0.2);
        Instance->transform->position = Vector3(25, 0, 0);

        MeshRenderer* pRenderer = FindObjectWithName(L"Sphere")->GetComponent<MeshRenderer>();
        EditorManager::mDebugEditor->AddRenderModel(pRenderer);
    }
    {   // Mixamo
        auto ModelResource = ResourceManager::AddResource<FBXModelResource>(L"resource/fbx/Mixamo/SkinningTest.fbx");
        auto ModelPrefab = ResourceManager::AddResource<Prefab>(L"resource/fbx/Mixamo/SkinningTest.fbx", ModelResource);
        ModelPrefab->SetGroupName(L"Model_Group3");
        mFBXModelResources.push_back(ModelResource);
        mModelPrefabs.push_back(ModelPrefab);

        auto Instance = Instantiate(ModelPrefab.get());
        Instance->transform->scale = Vector3(0.2, 0.2, 0.2);
        Instance->transform->position = Vector3(-25, 0, 0);

        Animator* pAnimator = Instance->AddComponent<Animator>();
        pAnimator->SetAnimation(ResourceManager::GetResource<AnimationResource>(L"mixamo.com"));
    }
   
    
    ObjectGroup* pGroup =  GameManager::GetCurrentWorld()->GetObjectGroup(L"Default");
    Object* dirLight;
    dirLight = pGroup->CreateObject(L"Direction_Light1", L"Default");
    Light* pDirLight1 = dirLight->AddComponent<Light>();
    pDirLight1->SetLightDirection(Vector4(0, 0, 1, 0));

    dirLight = pGroup->CreateObject(L"Direction_Light2", L"Default");
    Light* pDirLight2 = dirLight->AddComponent<Light>();
    pDirLight2->SetLightDirection(Vector4(0, 1, 0, 0));

    dirLight = pGroup->CreateObject(L"Direction_Light3", L"Default");
    Light* pDirLight3 = dirLight->AddComponent<Light>();
    pDirLight3->SetLightDirection(Vector4(1, 0, 0, 0));

    mMainCamera = FindObject(L"Main_Camera", L"Default")->GetComponent<Camera>();

    EditorManager::mDebugEditor->SetCamera(mMainCamera);
    EditorManager::mDebugEditor->AddLight(pDirLight1);
    EditorManager::mDebugEditor->AddLight(pDirLight2);
    EditorManager::mDebugEditor->AddLight(pDirLight3);

    //EditorManager::mDebugEditor->AddRenderModel(;
}

void ModelSpawner::Tick()
{
}

void ModelSpawner::FixedUpdate()
{
}

void ModelSpawner::PreUpdate()
{
}

void ModelSpawner::Update()
{
}

void ModelSpawner::PostUpdate()
{
}

void ModelSpawner::PreRender()
{
}

void ModelSpawner::Render()
{
}

void ModelSpawner::PostRender()
{
}
