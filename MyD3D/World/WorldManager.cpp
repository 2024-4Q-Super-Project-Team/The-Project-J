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

void WorldManager::Tick()
{
	UpdateWorld();
	if (mCurrActiveWorld) {
		mCurrActiveWorld->OnTick();
		UPDATE_ENTITY(mCurrActiveWorld, Tick())
	}
}

void WorldManager::FixedUpdate()
{
	if (mCurrActiveWorld) {
		mCurrActiveWorld->OnFixedUpdate();
        UPDATE_ENTITY(mCurrActiveWorld, FixedUpdate());
	}
}

void WorldManager::PreUpdate()
{
	if (mCurrActiveWorld) {
		mCurrActiveWorld->OnPreUpdate();
		UPDATE_ENTITY(mCurrActiveWorld, PreUpdate())
	}
}

void WorldManager::Update()
{
	if (mCurrActiveWorld) {
		mCurrActiveWorld->OnUpdate();
		UPDATE_ENTITY(mCurrActiveWorld, Update())
	}
}

void WorldManager::PostUpdate()
{
	if (mCurrActiveWorld) {
		mCurrActiveWorld->OnPostUpdate();
		UPDATE_ENTITY(mCurrActiveWorld, PostUpdate())
	}
}

void WorldManager::PreRender()
{
	if (mCurrActiveWorld) {
		mCurrActiveWorld->OnPreRender();
		UPDATE_ENTITY(mCurrActiveWorld, PreRender())
	}
}

void WorldManager::Render()
{
	if (mCurrActiveWorld) {
		mCurrActiveWorld->OnRender();
		UPDATE_ENTITY(mCurrActiveWorld, Render())
	}
}

void WorldManager::PostRender()
{
	if (mCurrActiveWorld) {
		mCurrActiveWorld->OnPostRender();
		UPDATE_ENTITY(mCurrActiveWorld, PostRender())
	}
}

void WorldManager::UpdateWorld()
{
	for (auto itr = mWorlds.begin(); itr != mWorlds.end();)
	{
		// ����
		if (ENTITY_IS_DESTROY((*itr).second))
		{
			itr = mWorlds.erase(itr);
			continue;
		}
		// ����
		if (ENTITY_IS_CREATE((*itr).second))
		{
			(*itr).second->SetActive(true);
		}
		++itr;
	}

	{ // ��Ƽ�� �� �ٲٱ�
		if (nullptr != mNextActiveWorld)
		{
            if(mCurrActiveWorld)
                mCurrActiveWorld->OnDisable();
			mCurrActiveWorld = mNextActiveWorld;
			mCurrActiveWorld->OnEnable();
			mNextActiveWorld = nullptr;
		}
	}
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
		// �����ϴ� ���尡 �����ؾ��ϴ� ����� nullptr�� �ٲ��ش�.
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

void WorldManager::SaveWorld(std::wstring worldName)
{
	World* world;
	if (worldName == L"default")
	{
		if (mCurrActiveWorld == nullptr)
			return;
		world = mCurrActiveWorld;
	}	
	else
		world = mWorlds[worldName];


	//(1) ���� - ������Ʈ  �׷� - ������Ʈ -������Ʈ id ���� ����ȭ
	json data = world->Serialize();
	std::wstring name = world->GetName();
	std::ofstream file(name + L".json");
	if (file.is_open())
		file << data.dump(4);
	file.close();


	//(2) �� ������Ʈ ����ȭ 
	json cmpData = json::array();
	for (const auto& group : world->GetObjectGroups())
		for (const auto& object : group->GetObjects())
			cmpData.push_back(object->SerializeComponents());

	std::ofstream file2(name + L"Cmps.json");
	if (file2.is_open())
		file2 << cmpData.dump(4);
	file2.close();
	
}

void WorldManager::LoadWorld(std::wstring worldName)
{
	World* world;
	if (worldName == L"default")
	{
		if (mCurrActiveWorld == nullptr)
			return;
		world = mCurrActiveWorld;
	}
	else
		world = mWorlds[worldName];

	std::wstring name = world->GetName();
	std::ifstream file(name + L".json");
	json j;
	file >> j;

	world->Deserialize(j);

	file.close();
}
