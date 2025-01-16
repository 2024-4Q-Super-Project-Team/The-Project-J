#include "pch.h"
#include "World.h"
#include "World/Light/LightSystem.h"
#include "Manager/GameManager.h"
#include "Physics/PhysicsManager.h"
#include "ViewportScene/ViewportScene.h"
#include "ObjectGroup/ObjectGroup.h"
#include "Object/Object.h"

World::World(ViewportScene* _pViewport, std::wstring_view _name, std::wstring_view _tag, bool isEmpty)
    : Entity(_name, _tag)
    , mOwnerScene(_pViewport)
    , mLightSystem(new LightSystem)
{
    if (!isEmpty)
    {
        ObjectGroup* defaultGroup = CreateObjectGroup(L"Default", L"Default");
        Object* mainCamera = defaultGroup->CreateObject(L"Main_Camera", L"Default");
        mainCamera->AddComponent<Camera>();
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
    // ����Ʈ�� Draw�� Camera�� �޾� �����ϱ� ������ Draw�� �� ���� �� LightSystem�� Bind�� ȣ���Ѵ�.
    mLightSystem->Bind();
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
        groupJson["name"] = group->GetName();
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
