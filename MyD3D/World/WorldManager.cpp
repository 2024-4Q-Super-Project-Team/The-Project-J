#include "pch.h"
#include "WorldManager.h"
#include "World/World.h"
#include "ViewportScene/ViewportScene.h"
#include "Object/Object.h"

WorldManager::WorldManager(ViewportScene* _pViewport)
	: mOwnerScene(_pViewport), mCurrActiveWorld(nullptr), mNextActiveWorld(nullptr)
{
}

WorldManager::~WorldManager()
{
    SAFE_DELETE_VECTOR(mWorldArray);
	mCurrActiveWorld = nullptr;
	mNextActiveWorld = nullptr;
}

void WorldManager::Start()
{
	if (mCurrActiveWorld) {
		mCurrActiveWorld->Start();
	}
}

void WorldManager::Reset()
{
	SAFE_DELETE_VECTOR(mWorldArray);
	mCurrActiveWorld = nullptr;
	mNextActiveWorld = nullptr;
}

void WorldManager::Tick()
{
	UpdateWorld();
	for (auto& world : mWorldArray)
	{
		if (world == mCurrActiveWorld || world->IsPersistance())
		{
			UPDATE_ENTITY(world, Tick());
		}
	}
}

void WorldManager::FixedUpdate()
{
	for (auto& world : mWorldArray)
	{
		if (world == mCurrActiveWorld || world->IsPersistance())
		{
			UPDATE_ENTITY(world, FixedUpdate());
		}
	}
}

void WorldManager::PreUpdate()
{
	for (auto& world : mWorldArray)
	{
		if (world == mCurrActiveWorld || world->IsPersistance())
		{
			UPDATE_ENTITY(world, PreUpdate());
		}
	}
}

void WorldManager::Update()
{
	for (auto& world : mWorldArray)
	{
		if (world == mCurrActiveWorld || world->IsPersistance())
		{
			UPDATE_ENTITY(world, Update());
		}
	}
}

void WorldManager::PostUpdate()
{
	for (auto& world : mWorldArray)
	{
		if (world == mCurrActiveWorld || world->IsPersistance())
		{
			UPDATE_ENTITY(world, PostUpdate());
		}
	}
}

void WorldManager::PreRender()
{
	for (auto& world : mWorldArray)
	{
		if (world == mCurrActiveWorld || world->IsPersistance())
		{
			UPDATE_ENTITY(world, PreRender());
		}
	}
}

void WorldManager::Render()
{
	for (auto& world : mWorldArray)
	{
		if (world == mCurrActiveWorld || world->IsPersistance())
		{
			UPDATE_ENTITY(world, Render());
		}
	}
}

void WorldManager::PostRender()
{
	for (auto& world : mWorldArray)
	{
		if (world == mCurrActiveWorld || world->IsPersistance())
		{
			UPDATE_ENTITY(world, PostRender());
		}
	}
}

void WorldManager::EditorUpdate()
{
	UpdateWorld();
	for (auto& world : mWorldArray)
	{
		if (world == mCurrActiveWorld || world->IsPersistance())
		{
			UPDATE_ENTITY(world, EditorUpdate());
		}
	}
}

void WorldManager::EditorRender()
{
	for (auto& world : mWorldArray)
	{
		if (world == mCurrActiveWorld || world->IsPersistance())
		{
			UPDATE_ENTITY(world, EditorRender());
		}
	}
}

void WorldManager::UpdateWorld()
{
	for (auto itr = mWorldArray.begin(); itr != mWorldArray.end();)
	{
		World* pWorld = *itr;
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
			itr = mWorldArray.erase(itr);
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
	auto world = FindWorld(_name);
	if (world != nullptr)
	{
		return world;
	}
	else
	{
		World* instance = new World(mOwnerScene, _name, _tag, isEmpty);
		mWorldArray.push_back(instance);
		instance->OnCreate();
		if (isEmpty == false)
		{
			instance->InitWorldObject();
		}
		return instance;
	}
}

BOOL WorldManager::DestroyWorld(const std::wstring& _name)
{
	auto world = FindWorld(_name);
	if (world != nullptr)
	{
		world->SetDestroy();
		// 삭제하는 월드가 변경해야하는 월드면 nullptr로 바꿔준다.
		if (world == mNextActiveWorld)
		{
			mNextActiveWorld = nullptr;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL WorldManager::SetActiveWorld(const std::wstring& _name)
{
	auto world = FindWorld(_name);
    if (world != nullptr)
    {
        return SetActiveWorld(world);
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

World* WorldManager::FindWorld(const std::wstring _name)
{
	auto itr = FIND_CONTAINER(mWorldArray,
		[&_name](World* _world) {
			return (_world->GetName() == _name);
		});
	return (FIND_SUCCESS(itr, mWorldArray)) ? (*itr) : nullptr;
}

