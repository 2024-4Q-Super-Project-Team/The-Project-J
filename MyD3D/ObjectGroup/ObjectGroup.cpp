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

void ObjectGroup::Tick()
{
    UpdateObject();
	FOR_LOOP_ARRAY_ENTITY(mObjects, Tick())
}

void ObjectGroup::FixedUpdate()
{
	FOR_LOOP_ARRAY_ENTITY(mObjects, FixedUpdate())
}

void ObjectGroup::PreUpdate()
{
	FOR_LOOP_ARRAY_ENTITY(mObjects, PreUpdate())
}

void ObjectGroup::Update()
{
	FOR_LOOP_ARRAY_ENTITY(mObjects, Update())
}

void ObjectGroup::PostUpdate()
{
	FOR_LOOP_ARRAY_ENTITY(mObjects, PostUpdate())
}

void ObjectGroup::PreRender()
{
	FOR_LOOP_ARRAY_ENTITY(mObjects, PreRender())
}

void ObjectGroup::Render()
{
	FOR_LOOP_ARRAY_ENTITY(mObjects, Render())
}

void ObjectGroup::Draw(Camera* _camera)
{
    FOR_LOOP_ARRAY_ENTITY(mObjects, Draw(_camera))
}

void ObjectGroup::PostRender()
{
    FOR_LOOP_ARRAY_ENTITY(mObjects, PostRender())
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
    // 昏力 棺 积己 贸府
    for (auto itr = mObjects.begin(); itr != mObjects.end();)
    {
        // 昏力
        if ((*itr)->GetState() == EntityState::Destroy)
        {
            itr = mObjects.erase(itr);
            continue;
        }
        // 积己
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
    _world->ReceiveObjectGroup(this);
}

json ObjectGroup::Serialize()
{
    json ret;
    ret["id"] = GetId();
    ret["name"] = Helper::to_utf8(mName);

    json objs = json::array();
    for (auto obj : mObjects)
    {
        json objectJson;
        objectJson["id"] = obj->GetId();
        objectJson["name"] = obj->GetName();
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
        Object* group = CreateObject(name);
        group->SetId(objectJson["id"].get<unsigned int>());
    }
}
