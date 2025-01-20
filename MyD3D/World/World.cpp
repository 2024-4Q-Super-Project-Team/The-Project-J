#include "pch.h"
#include "World.h"
#include "World/Light/LightSystem.h"
#include "Manager/GameManager.h"
#include "Physics/PhysicsManager.h"
#include "ViewportScene/ViewportScene.h"
#include "ObjectGroup/ObjectGroup.h"
#include "Object/Object.h"

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

    return PxFilterFlag::eDEFAULT;
}


World::World(ViewportScene* _pViewport, std::wstring_view _name, std::wstring_view _tag, bool isEmpty)
    : Entity(_name, _tag)
    , mOwnerScene(_pViewport)
    , mLightSystem(new LightSystem)
{
    if (!isEmpty)
    {
        PxSceneDesc sceneDesc(GameManager::GetPhysicsManager()->GetPhysics()->getTolerancesScale());
        sceneDesc.gravity = PxVec3(0.f, -9.8f, 0.f);
        sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(2);
        sceneDesc.filterShader = CustomFilterShader;
        //sceneDesc.simulationEventCallback = mEventCallback;
        // GPU ���� ���� (�ʼ�)
        mPxScene = GameManager::GetPhysicsManager()->GetPhysics()->createScene(sceneDesc);
    }
}

World::~World()
{
    SAFE_DELETE_VECTOR(mObjectGroups);
    SAFE_DELETE(mLightSystem);
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
    ret["id"] = GetId();
    ret["name"] = Helper::ToString(mName);

    json objGroups = json::array();
    for (auto group : mObjectGroups)
    {
        json groupJson;
        groupJson["id"] = group->GetId();
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
