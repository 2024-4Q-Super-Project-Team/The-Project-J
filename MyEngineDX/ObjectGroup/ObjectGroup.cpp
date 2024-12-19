#include "pch.h"
#include "ObjectGroup.h"
#include "Object/Object.h"
#include "World/World.h"

ObjectGroup::ObjectGroup() 
	: mOwnerWorld(nullptr)
	, mGroupOrder(0)
{
}

ObjectGroup::ObjectGroup(std::wstring_view _name, std::wstring_view _tag)
	: Entity(_name, _tag), mOwnerWorld(nullptr)
{
}

ObjectGroup::~ObjectGroup()
{
}

void ObjectGroup::Tick()
{
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

void ObjectGroup::Render(GraphicsManager* _graphicsManager)
{
	FOR_LOOP_ARRAY_ENTITY(mObjects, Render(_graphicsManager))
}

void ObjectGroup::PostRender()
{
	for (auto itr = mObjects.begin();
		itr != mObjects.end(); )
	{
		GameObject* pObject = (*itr);
		if (pObject->GetState() == EntityState::Active)
		{
			pObject->PostRender();
			++itr;
			continue;
		}
		else if (pObject->GetState() == EntityState::Destroy)
		{
			mDestroyQueue.push(pObject);
			itr = mObjects.erase(itr);
			continue;
		}
	}

	{ // 积己 林扁
		while (!mCreateQueue.empty())
		{
			GameObject* instance = mCreateQueue.front();
			mObjects.push_back(instance);
			instance->SetActive(true);
			mCreateQueue.pop();
		}
	}

	{ // 昏力 林扁
		while (!mDestroyQueue.empty())
		{
			SAFE_DELETE(mDestroyQueue.front())
				mDestroyQueue.pop();
		}
	}
}

GameObject* ObjectGroup::CreateObject(std::wstring_view _name, std::wstring_view _tag)
{
	GameObject* instance = new GameObject(this, _name, _tag);
	mCreateQueue.push(instance);
	return instance;
}

void ObjectGroup::UpdateObject()
{
}

void ObjectGroup::SetListSize(UINT _size)
{
	if (mObjects.size() > _size)
		return;
	mObjects.resize(_size);
}

void ObjectGroup::SetWorld(DXWorld* _world)
{
	if (mOwnerWorld == _world)
		return;
	_world->CreateObjectGroup(this);
}

