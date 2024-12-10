#include "pch.h"
#include "ObjectGroup.h"


ObjectGroup::ObjectGroup() 
	: mOwnerWorld(nullptr)
{
}

ObjectGroup::ObjectGroup(std::wstring_view _name, std::wstring_view _tag)
	: Entity(_name, _tag), mOwnerWorld(nullptr)
{
}

ObjectGroup::~ObjectGroup()
{
}

void ObjectGroup::FixedUpdate()
{
	for (GameObject*& obj : mObjects)
	{
		if (obj->GetState() == EntityState::Active)
			obj->FixedUpdate();
	}
}

void ObjectGroup::PreUpdate()
{
	for (GameObject*& obj : mObjects)
	{
		if (obj->GetState() == EntityState::Active)
			obj->PreUpdate();
	}
}

void ObjectGroup::Update()
{
	for (GameObject*& obj : mObjects)
	{
		if (obj->GetState() == EntityState::Active)
			obj->Update();
	}
}

void ObjectGroup::PostUpdate()
{
	for (GameObject*& obj : mObjects)
	{
		if (obj->GetState() == EntityState::Active)
			obj->PostUpdate();
	}
}

void ObjectGroup::PreRender()
{
	for (GameObject*& obj : mObjects)
	{
		if (obj->GetState() == EntityState::Active)
			obj->PreRender();
	}
}

void ObjectGroup::Render(GraphicsManager* _graphicsManager)
{
	for (GameObject*& obj : mObjects)
	{
		if (obj->GetState() == EntityState::Active)
			obj->Render(_graphicsManager);
	}
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

void ObjectGroup::SetListSize(UINT _size)
{
	if (mObjects.size() > _size)
		return;
	mObjects.resize(_size);
}
