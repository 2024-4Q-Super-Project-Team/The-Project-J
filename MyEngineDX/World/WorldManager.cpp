#include "pch.h"
#include "WorldManager.h"

WorldManager::WorldManager()
	: mCurrActiveWorld(nullptr), mNextActiveWorld(nullptr)
{
}

WorldManager::~WorldManager()
{
	for (auto& [first, second] : mWorlds)
	{
		SAFE_DELETE(second)
	}
	for (auto& [first, second] : DXWorld::mPersistanceObjectGroups)
	{
		SAFE_DELETE(second)
	}
	mWorlds.clear();
	DXWorld::mPersistanceObjectGroups.clear();
}

void WorldManager::FixedUpdate()
{
	if(mCurrActiveWorld){
		mCurrActiveWorld->OnFixedUpdate();
		UPDATE_ENTITY(mCurrActiveWorld, FixedUpdate())
	}
}

void WorldManager::PreUpdate()
{
	if (mCurrActiveWorld){
		mCurrActiveWorld->OnPreUpdate();
		UPDATE_ENTITY(mCurrActiveWorld, PreUpdate())
	}
}

void WorldManager::Update()
{
	if (mCurrActiveWorld){
		mCurrActiveWorld->OnUpdate();
		UPDATE_ENTITY(mCurrActiveWorld, Update())
	}
}

void WorldManager::PostUpdate()
{
	if (mCurrActiveWorld){
		mCurrActiveWorld->OnPostUpdate();
		UPDATE_ENTITY(mCurrActiveWorld, PostUpdate())
	}
}

void WorldManager::PreRender()
{
	if (mCurrActiveWorld){
		mCurrActiveWorld->OnPreRender();
		UPDATE_ENTITY(mCurrActiveWorld, PreRender())
	}
}

void WorldManager::Render(GraphicsManager* _graphicsManager)
{
	if (mCurrActiveWorld){
		mCurrActiveWorld->OnRender();
		UPDATE_ENTITY(mCurrActiveWorld, Render(_graphicsManager))
	}
}

void WorldManager::PostRender()
{
	if (mCurrActiveWorld){
		mCurrActiveWorld->OnPostRender();
		UPDATE_ENTITY(mCurrActiveWorld, PostRender())
	}

	for (auto itr = mWorlds.begin();
		itr != mWorlds.end(); )
	{
		DXWorld* pworld = (*itr).second;
		if (pworld->GetState() == EntityState::Destroy)
		{
			mDestroyQueue.push(pworld);
			itr = mWorlds.erase(itr);
			continue;
		}
		else ++itr;
	}

	{ // 생성 주기
		while (!mCreateQueue.empty())
		{
			DXWorld* instance = mCreateQueue.front();
			mWorlds[instance->GetName()] = instance;
			instance->SetActive(true);
			mCreateQueue.pop();
		}
	}

	{ // 삭제 주기
		while (!mDestroyQueue.empty())
		{
			SAFE_DELETE(mDestroyQueue.front())
			mDestroyQueue.pop();
		}
	}

	{ // 액티브 씬 바꾸기
		if (nullptr != mNextActiveWorld)
		{
			mCurrActiveWorld->OnUnLoad();
			mCurrActiveWorld = mNextActiveWorld;
			mCurrActiveWorld->OnLoad();
			mNextActiveWorld = nullptr;
		}
	}
}

BOOL WorldManager::SetActiveWorld(std::wstring_view _name)
{
	auto itr = mWorlds.find(_name.data());
	if (itr == mWorlds.end())
	{
		return FALSE;
	}
	else
	{
		mNextActiveWorld = (*itr).second;
	}
	return TRUE;
}
