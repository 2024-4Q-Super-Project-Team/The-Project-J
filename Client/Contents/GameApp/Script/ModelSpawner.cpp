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
        Instance->transform->position = Vector3(0, 0, 0);
        Instance->transform->scale = Vector3(1000, 1000, 1);
        Instance->transform->SetEulerAnglesFromDegree(Vector3(90, 0, 0));

        MeshRenderer* pRenderer = Instance->AddComponent<MeshRenderer>();
        pRenderer->SetMesh(MeshResource::PlainMesh);
        pRenderer->SetMaterial(MaterialResource::DefaultMaterial);
        pRenderer->GetMaterial()->mMatCBuffer.MatProp.RoughnessScale = 0.0f;
        pRenderer->GetMaterial()->mMatCBuffer.MatProp.MetallicScale = 0.5f;
        pRenderer->GetMaterial()->mMatCBuffer.MatProp.AmbienOcclusionScale = 0.1f;
    }

    {   // Gun
        auto ModelResource = ResourceManager::AddResource<FBXModelResource>(L"resource/fbx/Gun/gun.fbx");
        auto ModelPrefab = ResourceManager::AddResource<Prefab>(L"resource/fbx/gun.fbx", ModelResource);
        ModelPrefab->SetGroupName(L"Model_Group1");
        mFBXModelResources.push_back(ModelResource);
        mModelPrefabs.push_back(ModelPrefab);
        
        auto Instance = Instantiate(ModelPrefab.get());
        //Instance->transform->scale = Vector3(0.2, 0.2, 0.2);
        Instance->transform->position = Vector3(0, 45, 0);

        MeshRenderer* pRenderer = FindObjectWithName(L"Cerberus00_Fixed")->GetComponent<MeshRenderer>();
        if(EditorManager::mDebugEditor)
        EditorManager::mDebugEditor->AddRenderModel(pRenderer);
    }
    {   // Sphere
        auto ModelResource = ResourceManager::AddResource<FBXModelResource>(L"resource/fbx/Sphere/sphere.fbx");
        auto ModelPrefab = ResourceManager::AddResource<Prefab>(L"resource/fbx/sphere.fbx", ModelResource);
        ModelPrefab->SetGroupName(L"Model_Group2");
        mFBXModelResources.push_back(ModelResource);
        mModelPrefabs.push_back(ModelPrefab);

        auto Instance = Instantiate(ModelPrefab.get());
        //Instance->transform->scale = Vector3(0.2, 0.2, 0.2);
        Instance->transform->position = Vector3(100, 50, 50);

        MeshRenderer* pRenderer = FindObjectWithName(L"Sphere")->GetComponent<MeshRenderer>();
        pRenderer->GetMaterial()->mMatCBuffer.MatProp.RoughnessScale = 0.0f;
        pRenderer->GetMaterial()->mMatCBuffer.MatProp.MetallicScale = 1.0f;
        pRenderer->GetMaterial()->mMatCBuffer.MatProp.AmbienOcclusionScale = 1.0f;
        EditorManager::mDebugEditor->AddRenderModel(pRenderer);
    }
    {   // Mixamo
        auto ModelResource = ResourceManager::AddResource<FBXModelResource>(L"resource/fbx/Mixamo/SkinningTest.fbx");
        auto ModelPrefab = ResourceManager::AddResource<Prefab>(L"resource/fbx/Mixamo/SkinningTest.fbx", ModelResource);
        ModelPrefab->SetGroupName(L"Model_Group3");
        mFBXModelResources.push_back(ModelResource);
        mModelPrefabs.push_back(ModelPrefab);

        auto Instance = Instantiate(ModelPrefab.get());
        //Instance->transform->scale = Vector3(0.2, 0.2, 0.2);
        Instance->transform->position = Vector3(-100, 0, 50);

        Animator* pAnimator = Instance->AddComponent<Animator>();
        pAnimator->SetAnimation(ResourceManager::GetResource<AnimationResource>(L"mixamo.com"));
    }
    {   // Evironment
        auto ModelResource = ResourceManager::AddResource<FBXModelResource>(L"resource/fbx/Desert_Rock_007/Desert_Rock_007.FBX");
        if (ModelResource->mRootNode != nullptr)
        {
            auto ModelPrefab = ResourceManager::AddResource<Prefab>(L"resource/fbx/Desert_Rock_007/Desert_Rock_007.FBX", ModelResource);
            ModelPrefab->SetGroupName(L"Model_Group2");
            mFBXModelResources.push_back(ModelResource);
            mModelPrefabs.push_back(ModelPrefab);

            auto Instance = Instantiate(ModelPrefab.get());
            //Instance->transform->scale = Vector3(0.2, 0.2, 0.2);
            Instance->transform->position = Vector3(100, 50, 50);
        }
    }
    
    ObjectGroup* pGroup = GameManager::GetCurrentWorld()->GetObjectGroup(L"Default");
    Object* dirLight;
    dirLight = pGroup->CreateObject(L"Direction_Light1", L"Default");
    Light* pDirLight1 = dirLight->AddComponent<Light>();
    pDirLight1->SetLightDirection(Vector4(0, -1, 1, 0));

    dirLight = pGroup->CreateObject(L"Direction_Light2", L"Default");
    Light* pDirLight2 = dirLight->AddComponent<Light>();
    pDirLight2->SetLightDirection(Vector4(0.3, -1, 0.15, 0));
    dirLight->SetActive(false); // 초기 비활성화

    mMainCamera = FindObject(L"Main_Camera", L"Default")->GetComponent<Camera>();
    if (EditorManager::mDebugEditor)
    {
        EditorManager::mDebugEditor->SetCamera(mMainCamera);
        EditorManager::mDebugEditor->AddLight(pDirLight1);
        EditorManager::mDebugEditor->AddLight(pDirLight2);
    }
    
    //auto pAudioResource = ResourceManager::AddResource<AudioResource>(L"resource/sound/odesa_new.ogg");
    //auto pAudioSource = gameObject->AddComponent<AudioSource>();
    //pAudioResource->GetAudioClip()->SetSurround(true);
    //pAudioSource->SetSurround(true);
    //pAudioSource->AddAudio(L"test", pAudioResource);
    //pAudioSource->Play(L"test");
}

void ModelSpawner::Tick()
{
    if (Input::IsKeyHold(Key::SPACEBAR))
    {
        auto pViewport = ViewportManager::GetActiveViewport();
        POINT size = { 800, 800 };
        pViewport->GetIWindow()->SetSize(size);
    }
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

json ModelSpawner::Serialize()
{
    return json();
}

void ModelSpawner::Deserialize(json& j)
{
}
