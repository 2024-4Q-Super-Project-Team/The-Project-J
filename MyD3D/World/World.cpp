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

    pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND; // �浹 ���� �̺�Ʈ
    pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;  // �浹 ���� �̺�Ʈ
    pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS; // ���� ���� ���� ��û

    // PVD���� ������� ���� �߰� �÷���
    pairFlags |= PxPairFlag::eDETECT_DISCRETE_CONTACT; // �̻� �浹 �����
    pairFlags |= PxPairFlag::eDETECT_CCD_CONTACT;     // ���� �浹 �����
    pairFlags |= PxPairFlag::eNOTIFY_THRESHOLD_FORCE_FOUND; // �� �Ӱ�ġ �ʰ� �̺�Ʈ
    pairFlags |= PxPairFlag::eNOTIFY_THRESHOLD_FORCE_PERSISTS; // �� ���� �̺�Ʈ
    pairFlags |= PxPairFlag::eNOTIFY_THRESHOLD_FORCE_LOST; // �� ��� �̺�Ʈ

    return PxFilterFlag::eDEFAULT;
}


World::World(ViewportScene* _pViewport, std::wstring_view _name, std::wstring_view _tag, bool isEmpty)
    : Entity(_name, _tag)
    , mOwnerScene(_pViewport)
    , mLightSystem(new LightSystem)
{
    if (!isEmpty)
    {
        mPickingRay = new PickingRay;

        PxSceneDesc sceneDesc(GameManager::GetPhysicsManager()->GetPhysics()->getTolerancesScale());
        sceneDesc.gravity = PxVec3(0.f, -0.f, 0.f);
        sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(2);
        sceneDesc.filterShader = CustomFilterShader;
        //sceneDesc.simulationEventCallback = mEventCallback;

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
}

void World::Start()
{
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, Start())
}

void World::Tick()
{
    UpdateGroup();
    OnTick();
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, Tick())
}

void World::FixedUpdate()
{
    OnFixedUpdate();
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, FixedUpdate())
}

void World::PreUpdate()
{
    OnPreUpdate();
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, PreUpdate())
}

void World::Update()
{
    OnUpdate();
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, Update())
}

void World::PostUpdate()
{
    OnPostUpdate();
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, PostUpdate())


    if (mPxScene)
    {
        mPxScene->simulate(Time::GetUnScaledDeltaTime());
        mPxScene->fetchResults(true);
    }
}

void World::PreRender()
{
    OnPreRender();
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, PreRender())
}

void World::Render()
{
    OnRender();
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, Render())
}

void World::Draw(Camera* _camera)
{
    FOR_LOOP_ARRAY_ENTITY(mObjectGroups, Draw(_camera));
    // ī�޶� ��� �ִ� �׸��� �۾������ ����
    _camera->ExcuteDrawList();
}

void World::PostRender()
{
    OnPostRender();
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
    }
    return instance;
}

void World::ReceiveObjectGroup(ObjectGroup* _recvGroup)
{
    if (_recvGroup == nullptr)
        return;
    ObjectGroup* instance = GetObjectGroup(_recvGroup->GetName());
    // ���� �׷��� ���� �����̳ʿ� ���ٸ� �״�� �Űܿ´�.
    if (instance == nullptr)
    {
        // ���� �׷��� ���� ������ �׷������̳ʿ��� �ش� �׷��� ����� ����� �ű��.
        auto itr = FIND_CONTAINER(_recvGroup->GetWorld()->mObjectGroups,
            [&_recvGroup](ObjectGroup* _group) {
                return (_recvGroup == _group);
            });
        if (FIND_SUCCESS(itr, mObjectGroups))
        {
            _recvGroup->GetWorld()->mObjectGroups.erase(itr);
            mObjectGroups.push_back(_recvGroup);
            // Create���·� �ʱ�ȭ��Ų��.
            _recvGroup->SetCreate();
        }
    }
    // �̹� ������ ������Ʈ���� �����ͼ� �ڷ� ���δ�.
    // �ٵ� ���⼭ �ǹ��� ������Ʈ���� ��Ȱ��ȭ�ؼ� �����;��ϳ�?
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

void World::InitWorldObject()
{
    // ���� �ʱ� ������ �⺻������ �����ϴ� �׷�� ������Ʈ�� ������ش�.
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
    // ���� �� ���� ó��
    for (auto itr = mObjectGroups.begin(); itr != mObjectGroups.end();)
    {

        // ����
        if ((*itr)->GetState() == EntityState::Destroy)
        {
            itr = mObjectGroups.erase(itr);
            continue;
        }
        // ����
        if ((*itr)->GetState() == EntityState::Create)
        {
            (*itr)->SetActive(true);
        }
        ++itr;
    }
    // ���� ó��
    std::sort(mObjectGroups.begin(), mObjectGroups.end(),
        [](const ObjectGroup* left, const ObjectGroup* right) {
            return left->GetOrder() < right->GetOrder(); // ��������
        }
    );
}
