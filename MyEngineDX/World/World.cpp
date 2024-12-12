#include "pch.h"
#include "World.h"

std::unordered_map<std::wstring, ObjectGroup*> DXWorld::mPersistanceObjectGroups {};

DXWorld::DXWorld(WorldManager* _wrdMng, std::wstring_view _name, std::wstring_view _tag)
	: Entity(_name, _tag), mWorldTransform(new Transform3D()), mWorldManager(_wrdMng)
{
}

DXWorld::~DXWorld()
{
	for (auto itr : mObjectGroups)
	{
		delete itr.second;
	}
	mObjectGroups.clear();
	SAFE_DELETE(mWorldTransform)
}

void DXWorld::FixedUpdate()
{
	FOR_LOOP_ENTITY(mObjectGroups, FixedUpdate());
}

void DXWorld::PreUpdate()
{
	FOR_LOOP_ENTITY(mObjectGroups, PreUpdate());
}

void DXWorld::Update()
{
	FOR_LOOP_ENTITY(mObjectGroups, Update());
	for (auto& a : mObjectGroups)
	{
		a.second->Update();
	}
}

void DXWorld::PostUpdate()
{
	FOR_LOOP_ENTITY(mObjectGroups, PostUpdate());
}

void DXWorld::PreRender()
{
	FOR_LOOP_ENTITY(mObjectGroups, PreRender());
}

void DXWorld::Render(GraphicsManager* _graphicsManager)
{
	if (mWorldTransform)
	{
		mWorldTransform->CalculateMatrix();
	}
	FOR_LOOP_ENTITY(mObjectGroups, Render(_graphicsManager));
}

void DXWorld::PostRender()
{
	for (auto itr = mObjectGroups.begin();
		itr != mObjectGroups.end(); )
	{
		ObjectGroup* group = (*itr).second;
		if (group->GetState() == EntityState::Active)
		{
			group->PostRender();
			++itr;
			continue;
		}
		else if (group->GetState() == EntityState::Destroy)
		{
			mDestroyQueue.push(group);
			itr = mObjectGroups.erase(itr);
			continue;
		}
	}

	{ // 积己 林扁
		while (!mCreateQueue.empty())
		{
			ObjectGroup* instance = mCreateQueue.front();
			mObjectGroups[instance->GetName()] = instance;
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

ObjectGroup* DXWorld::CreateObjectGroup(std::wstring_view _name, std::wstring_view _tag)
{
	auto itr = mObjectGroups.find(_name.data());
	if (itr != mObjectGroups.end()) 
		return itr->second;
	ObjectGroup* instance = new ObjectGroup(_name, _tag);
	mCreateQueue.push(instance);
	return instance;
}

ObjectGroup* DXWorld::GetObjectGroup(std::wstring_view _name)
{
	auto itr = mObjectGroups.find(_name.data());
	if (itr != mObjectGroups.end())
		return itr->second;

	itr = mPersistanceObjectGroups.find(_name.data());
	if (itr != mPersistanceObjectGroups.end())
		return itr->second;

	return nullptr;
}
