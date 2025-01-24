#include "pch.h"
#include "ObjectGroup.h"
#include "Object/Object.h"
#include "World/World.h"

ObjectGroup::ObjectGroup(std::wstring_view _name, std::wstring_view _tag)
	: Entity(_name, _tag)
    , mOwnerWorld(nullptr)
	, mGroupOrder(0)
{
}

ObjectGroup::~ObjectGroup()
{
}

void ObjectGroup::Start()
{
    for (auto& object : mObjects)
    {
        object->Start();
    }
}

void ObjectGroup::Tick()
{
    UpdateObject();
    for (Object* object : mObjects)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->Tick();
        }
    }
}

void ObjectGroup::FixedUpdate()
{
    for (Object* object : mObjects)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->FixedUpdate();
        }
    }
}

void ObjectGroup::PreUpdate()
{
    for (Object* object : mObjects)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->PreUpdate();
        }
    }
}

void ObjectGroup::Update()
{
    for (Object* object : mObjects)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->Update();
        }
    }
}

void ObjectGroup::PostUpdate()
{
    for (Object* object : mObjects)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->PostUpdate();
        }
    }
}

void ObjectGroup::PreRender()
{
    for (Object* object : mObjects)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->PreRender();
        }
    }
}

void ObjectGroup::Render()
{
    for (Object* object : mObjects)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->Render();
        }
    }
}

void ObjectGroup::Draw(Camera* _camera)
{
    for (Object* object : mObjects)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->Draw(_camera);
        }
    }
}

void ObjectGroup::PostRender()
{
    for (Object* object : mObjects)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->PostRender();
        }
    }
}

void ObjectGroup::EditorUpdate()
{
    UpdateObject();
    for (Object* object : mObjects)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->EditorUpdate();
        }
    }
}

void ObjectGroup::EditorRender()
{
    for (Object* object : mObjects)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->EditorRender();
        }
    }
}

void _CALLBACK ObjectGroup::OnEnable()
{
    for (auto& object : mObjects)
    {
        object->OnEnable();
    }
    return void _CALLBACK();
}

void _CALLBACK ObjectGroup::OnDisable()
{
    for (auto& object : mObjects)
    {
        object->OnDisable();
    }
    return void _CALLBACK();
}

Object* ObjectGroup::CreateObject(std::wstring_view _name, std::wstring_view _tag)
{
	Object* instance = new Object(_name, _tag);
    instance->SetOwnerGroup(this);
    mObjects.push_back(instance);
    instance->SetCreate();
	return instance;
}

void ObjectGroup::UpdateObject()
{
    // 삭제 및 생성 처리
    for (auto itr = mObjects.begin(); itr != mObjects.end();)
    {
        // 삭제
        if ((*itr)->GetState() == EntityState::Destroy)
        {
            if (EditorManager::mInspectorViewer &&
                EditorManager::mInspectorViewer->GetFocusObject() == *itr) {
                EditorManager::mInspectorViewer->SetFocusObject(nullptr);
            }
            SAFE_DELETE(*itr);
            itr = mObjects.erase(itr);
            continue;
        }
        // 생성
        if ((*itr)->GetState() == EntityState::Create)
        {
            (*itr)->SetActive(true);
        }
        ++itr;
    }
}

void ObjectGroup::SetListSize(UINT _size)
{
	if (mObjects.size() > _size)
		return;
	mObjects.resize(_size);
}

void ObjectGroup::SetWorld(World* _world)
{
	if (mOwnerWorld == _world)
		return;
    
    mOwnerWorld = _world;
    
    //_world->ReceiveObjectGroup(this); //?무슨 코드인지 모르겠음. 일단 호출하는데 없어서 주석처리했습니다
}

Object* ObjectGroup::GetObject(std::wstring name) const
{
    auto found = std::find_if(mObjects.begin(), mObjects.end(), [&name](Object* obj) {return obj->GetName() == name;});

    if (found != mObjects.end())
        return *found;
    return nullptr;
}

json ObjectGroup::Serialize()
{
    json ret;
    ret["id"] = GetId();
    ret["name"] = Helper::ToString(mName);

    json objs = json::array();
    for (auto obj : mObjects)
    {
        json objectJson;
        objectJson["id"] = obj->GiveId();
        objectJson["name"] = Helper::ToString(obj->GetName());
        objs += objectJson;
    }
    ret["objects"] = objs;
    return ret;
}

void ObjectGroup::Deserialize(json& j)
{
    for (auto& objectJson : j["objects"])
    {
        std::wstring name = Helper::ToWString(objectJson["name"].get<std::string>());
        Object* object = CreateObject(name);
        object->SetId(objectJson["id"].get<unsigned int>());
    }
}
