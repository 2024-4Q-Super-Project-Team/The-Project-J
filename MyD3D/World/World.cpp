#include "pch.h"
#include "World.h"
#include "World/SkyBox/SkyBox.h"
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
    , mSkyBox(new SkyBox(this))
{
    if (!isEmpty)
    {
        ObjectGroup* defaultGroup = CreateObjectGroup(L"Default", L"Default");
        Object* mainCamera = defaultGroup->CreateObject(L"Main_Camera", L"Default");
        POINT windowSize = _pViewport->GetIWindow()->GetSize();
        mainCamera->AddComponent<Camera>(Vector2(windowSize.x, windowSize.y));
    }
}

World::~World()
{
    SAFE_DELETE_VECTOR(mObjectGroups);
    SAFE_DELETE(mLightSystem);
    SAFE_DELETE(mSkyBox);
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
    // 라이트는 Draw중 Camera를 받아 연산하기 때문에 Draw가 다 끝난 후 LightSystem의 Bind를 호출한다.
    mLightSystem->Bind();
    // 카메라가 담고 있는 그리기 작업목록을 수행
    _camera->ExcuteDrawList();
    // 스카이박스 렌더
    mSkyBox->Draw(_camera);
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
    for (auto itr = mObjectGroups.begin(); itr != mObjectGroups.end(); ++itr)
    {
        ret += (*itr)->Serialize();
    }
    return ret;
}

void World::UpdateGroup()
{
    // 삭제 및 생성 처리
    for (auto itr = mObjectGroups.begin(); itr != mObjectGroups.end();)
    {
        // 삭제
        if ((*itr)->GetState() == EntityState::Destroy)
        {
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
