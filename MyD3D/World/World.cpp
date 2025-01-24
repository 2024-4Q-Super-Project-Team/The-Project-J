#include "pch.h"
#include "World.h"
#include "World/Light/LightSystem.h"
#include "Manager/GameManager.h"
#include "Physics/PhysicsManager.h"
#include "ViewportScene/ViewportScene.h"
#include "ViewportScene/ViewportManager.h"
#include "ObjectGroup/ObjectGroup.h"
#include "Object/Object.h"
#include "Component/Camera/Camera.h"

PxFilterFlags CustomFilterShader(
    PxFilterObjectAttributes attributes0, PxFilterData filterData0,
    PxFilterObjectAttributes attributes1, PxFilterData filterData1,
    PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
    pairFlags = PxPairFlag::eCONTACT_DEFAULT;
    pairFlags |= PxPairFlag::eTRIGGER_DEFAULT;

    return PxFilterFlag::eDEFAULT;
}


World::World(ViewportScene* _pViewport, std::wstring_view _name, std::wstring_view _tag, bool isEmpty)
    : Entity(_name, _tag)
    , mOwnerScene(_pViewport)
    , mLightSystem(new LightSystem)
{
    SetEID(Helper::ToString(_name.data()));
    mNeedResourceHandleTable.reserve(30);
    if (!isEmpty)
    {
        mPickingRay = new PickingRay;

        PxSceneDesc sceneDesc(GameManager::GetPhysicsManager()->GetPhysics()->getTolerancesScale());
        sceneDesc.gravity = PxVec3(0.f, -9.8f, 0.f);
        sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(2);
        sceneDesc.filterShader = CustomFilterShader;
        //sceneDesc.simulationEventCallback = mEventCallback;
            // GPU 가속 설정 (필수)
        //sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
        //sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;
        //sceneDesc.cudaContextManager = GameManager::GetPhysicsManager()->GetCudaManager();


        mPxScene = GameManager::GetPhysicsManager()->GetPhysics()->createScene(sceneDesc);

        PxPvdSceneClient* pvdClient = mPxScene->getScenePvdClient();
        if (pvdClient) {
            pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
            pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
            pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
        }

    }
}

World::~World()
{
    SAFE_DELETE_VECTOR(mObjectGroups);
    SAFE_DELETE(mLightSystem);
    SAFE_DELETE(mPickingRay);
    mPxScene->release();
    
}

void World::Start()
{
    for (auto& group : mObjectGroups)
    {
        group->Start();
    }
}

void World::Tick()
{
    UpdateGroup();
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, Tick())
}

void World::FixedUpdate()
{
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, FixedUpdate())
}

void World::PreUpdate()
{
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, PreUpdate())
}

void World::Update()
{
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, Update())
}

void World::PostUpdate()
{
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, PostUpdate())


    if (mPxScene)
    {
        mPxScene->simulate(Time::GetUnScaledDeltaTime());
        mPxScene->fetchResults(true);
        mPxScene->fetchResultsParticleSystem();
    }
}

void World::PreRender()
{
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, PreRender())
}

void World::Render()
{
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, Render())
}

void World::Draw(Camera* _camera)
{
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, Draw(_camera));
}

void World::PostRender()
{
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, PostRender())
}

void World::EditorUpdate()
{
    UpdateGroup();
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, EditorUpdate())
}

void World::EditorRender()
{
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, EditorRender())
}

ObjectGroup* World::CreateObjectGroup(std::wstring_view _name, std::wstring_view _tag)
{
    ObjectGroup* instance = GetObjectGroup(_name);
    if (instance == nullptr)
    {
        instance = new ObjectGroup(_name, _tag);
        mObjectGroups.push_back(instance);
        instance->SetWorld(this);
    }
    return instance;
}

void World::ReceiveObjectGroup(ObjectGroup* _recvGroup)
{
    if (_recvGroup == nullptr)
        return;
    ObjectGroup* instance = GetObjectGroup(_recvGroup->GetName());
    // 받은 그룹이 현재 컨테이너에 없다면 그대로 옮겨온다.
    if (instance == nullptr)
    {
        // 받은 그룹이 속한 월드의 그룹컨테이너에서 해당 그룹을 지우고 여기로 옮긴다.
        auto itr = FIND_CONTAINER(_recvGroup->GetWorld()->mObjectGroups,
            [&_recvGroup](ObjectGroup* _group) {
                return (_recvGroup == _group);
            });
        if (FIND_SUCCESS(itr, mObjectGroups))
        {
            _recvGroup->GetWorld()->mObjectGroups.erase(itr);
            mObjectGroups.push_back(_recvGroup);
            // Create상태로 초기화시킨다.
            _recvGroup->SetCreate();
        }
    }
    // 이미 있으면 오브젝트들을 가져와서 뒤로 붙인다.
    // 근데 여기서 의문은 오브젝트들을 비활성화해서 가져와야하나?
    else
    {
        mObjectGroups.insert(mObjectGroups.end(), _recvGroup->GetWorld()->mObjectGroups.begin(), _recvGroup->GetWorld()->mObjectGroups.end());
    }
}

ObjectGroup* World::GetObjectGroup(std::wstring_view _name)
{
    auto itr = FIND_CONTAINER(mObjectGroups,
        [&_name](ObjectGroup* _group) {
            return (_group->GetName() == _name.data());
        });
    return (FIND_SUCCESS(itr, mObjectGroups)) ? (*itr) : nullptr;
}

json World::Serialize()
{
    json ret;
    ret["id"] = GiveId();
    ret["name"] = Helper::ToString(mName);

    json objGroups = json::array();
    for (auto group : mObjectGroups)
    {
        json groupJson;
        groupJson["id"] = group->GiveId();
        groupJson["name"] = Helper::ToString(group->GetName());
        objGroups += groupJson;
    }
    ret["object groups"] = objGroups;

    return ret;
}

void World::Deserialize(json& j)
{
    for (auto& groupJson : j["object groups"])
    {
        std::wstring name = Helper::ToWString(groupJson["name"].get<std::string>());
        ObjectGroup* group = CreateObjectGroup(name);
        group->SetId(groupJson["id"].get<unsigned int>());
    }
}

json World::SerializeDefault()
{
    json ret;
    ret["id"] = GetId();
    ret["name"] = Helper::ToString(GetName());


    ObjectGroup* defaultGroup = GetObjectGroup(L"Default");

    json groupJson;
    groupJson["id"] = defaultGroup->GiveId();
    groupJson["name"] = Helper::ToString(defaultGroup->GetName());
    
    json objectsJson;
    for (Object* object : defaultGroup->GetObjects())
    {
        json objectJson;
        objectJson["id"] = object->GiveId();
        objectJson["name"] = Helper::ToString(object->GetName());
        for (Component* component : object->GetAllComponents())
        {
            json compJson; 
            compJson = component->Serialize();
            objectJson["components"] += compJson;
        }

        objectsJson.push_back(objectJson);
    }
    groupJson["objects"] = objectsJson;
    ret["groups"] = groupJson;

    return ret;
}

void World::DeserializeDefault(json& j)
{
    ObjectGroup* defaultGroup = GetObjectGroup(L"Default");

    json groupJson = j["groups"];
    defaultGroup->SetId(groupJson["id"].get<unsigned int>());

    for (json objJson : groupJson["objects"])
    {
        Object* obj = defaultGroup->GetObject(Helper::ToWString(objJson["name"].get<std::string>()));
        obj->SetId(objJson["id"].get<unsigned int>());

        for (json compJson : objJson["components"])
        {
            if (compJson["name"] == L"Transform")
                obj->transform->Deserialize(compJson);

            else if (compJson["name"] == L"Camera")
            {
                obj->GetComponent<Camera>()->Deserialize(compJson);
            }
            else if (compJson["name"] == L"Light")
            {
                obj->GetComponent<Light>()->Deserialize(compJson);
            }
        }
    }
}

void _CALLBACK World::OnEnable()
{
    for (auto& group : mObjectGroups)
    {
        group->OnEnable();
    }
    return void _CALLBACK();
}

void _CALLBACK World::OnDisable()
{
    for (auto& group : mObjectGroups)
    {
        group->OnDisable();
    }
    return void _CALLBACK();
}

void World::InitWorldObject()
{
    // 월드 초기 생성시 기본적으로 제공하는 그룹과 오브젝트를 만들어준다.
    ObjectGroup* defaultGroup = CreateObjectGroup(L"Default", L"Default");
    {
        Object* mainCamera = defaultGroup->CreateObject(L"Main_Camera", L"Default");
        Camera* cameraComponent = mainCamera->AddComponent<Camera>();
    }
    {
        Object* mainLight = defaultGroup->CreateObject(L"Direction_Light", L"Default");
        Light* lightComponent = mainLight->AddComponent<Light>();
        Vector4 defaultDirection = Vector4(0.0f, 1.0f, 1.0f, 1.0f);
        defaultDirection.Normalize();
        lightComponent->GetProperty().Direction = defaultDirection;
    }
}

void World::UpdateGroup()
{
    mNeedResourceHandleTable.clear();
    // 삭제 및 생성 처리
    for (auto itr = mObjectGroups.begin(); itr != mObjectGroups.end();)
    {
        // 삭제
        if ((*itr)->GetState() == EntityState::Destroy)
        {
            SAFE_DELETE(*itr);
            itr = mObjectGroups.erase(itr);
            continue;
        }
        // 생성
        if ((*itr)->GetState() == EntityState::Create)
        {
            (*itr)->SetActive(true);
        }
        ++itr;
    }
    // 정렬 처리
    std::sort(mObjectGroups.begin(), mObjectGroups.end(),
        [](const ObjectGroup* left, const ObjectGroup* right) {
            return left->GetOrder() < right->GetOrder(); // 오름차순
        }
    );
}

void World::EditorRendering(EditorViewerType _viewerType)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

    switch (_viewerType)
    {
        case EditorViewerType::DEFAULT:
        {
            break;
        }
        case EditorViewerType::HIERARCHY:
        {
            break;
        }
        case EditorViewerType::INSPECTOR:
        {
            {
                ImGui::Text("World");
                static char buffer1[128] = "";
                strcpy_s(buffer1, Helper::ToString(GetName()).c_str());
                if (ImGui::InputText((uid + "InputName").c_str(), buffer1, IM_ARRAYSIZE(buffer1))) {
                    std::wstring newName = Helper::ToWString(std::string(buffer1));
                    SetName(newName);
                }
                static char buffer2[128] = "";
                strcpy_s(buffer2, Helper::ToString(GetTag()).c_str());
                ImGui::Text("Tag ");
                if (ImGui::InputText((uid + "Tag").c_str(), buffer2, IM_ARRAYSIZE(buffer2))) {
                    std::wstring newTag = Helper::ToWString(std::string(buffer2));
                    SetTag(newTag);
                }
            }
            ImGui::Separator();
            {
                ImGui::Text("Pre Load Resource List :");
                for (auto itr = mNeedResourceHandleTable.begin(); itr != mNeedResourceHandleTable.end(); ++itr)
                {
                    ImGui::Text(Helper::ToString((*itr).GetKey() + L" : " + (*itr).GetPath()).c_str());
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }
}