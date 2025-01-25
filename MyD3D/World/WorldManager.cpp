#include "pch.h"
#include "WorldManager.h"
#include "World/World.h"
#include "ViewportScene/ViewportScene.h"
#include "ObjectGroup/ObjectGroup.h"
#include "Object/Object.h"

#include <fstream>

WorldManager::WorldManager(ViewportScene* _pViewport)
	: mOwnerScene(_pViewport), mCurrActiveWorld(nullptr), mNextActiveWorld(nullptr)
{
}

WorldManager::~WorldManager()
{
    SAFE_DELETE_MAP(mWorlds);
}

void WorldManager::Start()
{
	if (mCurrActiveWorld) {
		mCurrActiveWorld->Start();
	}
}

void WorldManager::Tick()
{
	UpdateWorld();
	if (mCurrActiveWorld) {
		UPDATE_ENTITY(mCurrActiveWorld, Tick())
	}
}

void WorldManager::FixedUpdate()
{
	if (mCurrActiveWorld) {
        UPDATE_ENTITY(mCurrActiveWorld, FixedUpdate());
	}
}

void WorldManager::PreUpdate()
{
	if (mCurrActiveWorld) {
		UPDATE_ENTITY(mCurrActiveWorld, PreUpdate())
	}
}

void WorldManager::Update()
{
	if (mCurrActiveWorld) {
		UPDATE_ENTITY(mCurrActiveWorld, Update())
	}
}

void WorldManager::PostUpdate()
{
	if (mCurrActiveWorld) {
		UPDATE_ENTITY(mCurrActiveWorld, PostUpdate())
	}
}

void WorldManager::PreRender()
{
	if (mCurrActiveWorld) {
		UPDATE_ENTITY(mCurrActiveWorld, PreRender())
	}
}

void WorldManager::Render()
{
	if (mCurrActiveWorld) {
		UPDATE_ENTITY(mCurrActiveWorld, Render())
	}
}

void WorldManager::PostRender()
{
	if (mCurrActiveWorld) {
		UPDATE_ENTITY(mCurrActiveWorld, PostRender())
	}
}

void WorldManager::EditorUpdate()
{
	UpdateWorld();
	if (mCurrActiveWorld) {
		UPDATE_ENTITY(mCurrActiveWorld, EditorUpdate())
	}
}

void WorldManager::EditorRender()
{
	if (mCurrActiveWorld) {
		UPDATE_ENTITY(mCurrActiveWorld, EditorRender())
	}
}

void WorldManager::UpdateWorld()
{
	for (auto itr = mWorlds.begin(); itr != mWorlds.end();)
	{
		World* pWorld = (*itr).second;
		if (ENTITY_IS_DESTROY(pWorld))
		{
			if (mCurrActiveWorld == pWorld)
			{
				mCurrActiveWorld = nullptr;
			}
			if (mNextActiveWorld == pWorld)
			{
				mNextActiveWorld = nullptr;
			}
			// 인스펙터 포커스 확인
			if (EditorManager::mInspectorViewer &&
				EditorManager::mInspectorViewer->GetFocusObject() == pWorld) {
				EditorManager::mInspectorViewer->SetFocusObject(nullptr);
			}
			if(pWorld)
				delete pWorld;
			itr = mWorlds.erase(itr);
			continue;
		}
		// 생성
		if (ENTITY_IS_CREATE(pWorld))
		{
			if (pWorld)
				pWorld->SetActive(true);
			++itr;
			continue;
		}
		++itr;
	}

	{ // 액티브 씬 바꾸기
		if (nullptr != mNextActiveWorld)
		{
            if(mCurrActiveWorld)
                mCurrActiveWorld->OnDisable();
			mCurrActiveWorld = mNextActiveWorld;
			mCurrActiveWorld->OnEnable();
			mCurrActiveWorld->Start();
			mNextActiveWorld = nullptr;
		}
	}
}

World* WorldManager::CreateWorld(const std::wstring& _name, std::wstring_view _tag, bool isEmpty)
{
	auto itr = Helper::FindMap(_name, mWorlds);
	if (itr != nullptr)
	{
		return *itr;
	}
	else
	{
		World* instance = new World(mOwnerScene, _name, _tag, isEmpty);
		mWorlds[_name] = instance;
		instance->OnCreate();
		if (isEmpty == false)
		{
			instance->InitWorldObject();
		}
		return instance;
	}
}

void WorldManager::AddWorld(World* _world)
{
	mWorlds[_world->GetName()] = _world;
}

World* WorldManager::GetActiveWorld()
{
	return mCurrActiveWorld;
}

BOOL WorldManager::DestroyWorld(const std::wstring& _name)
{
	auto itr = Helper::FindMap(_name, mWorlds);
	if (itr != nullptr)
	{
		(*itr)->SetDestroy();
		// 삭제하는 월드가 변경해야하는 월드면 nullptr로 바꿔준다.
		if (*itr == mNextActiveWorld)
		{
			mNextActiveWorld = nullptr;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL WorldManager::SetActiveWorld(const std::wstring& _name)
{
	auto itr = Helper::FindMap(_name, mWorlds);
    if (itr != nullptr)
    {
        return SetActiveWorld(*itr);
    }
    return FALSE;
}

BOOL WorldManager::SetActiveWorld(World* _pWorld)
{
    if (_pWorld)
    {
        mNextActiveWorld = _pWorld;
        return TRUE;
    }
    return FALSE;
}

