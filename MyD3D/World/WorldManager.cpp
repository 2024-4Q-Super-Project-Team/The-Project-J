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
		// 삭제
		if (ENTITY_IS_DESTROY((*itr).second))
		{
			itr = mWorlds.erase(itr);
			continue;
		}
		// 생성
		if (ENTITY_IS_CREATE((*itr).second))
		{
			(*itr).second->SetActive(true);
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

void WorldManager::SaveWorlds()
{
	json worldsJson, groupsJson, objectsJson, componentsJson;

	for (const auto& world : mWorlds)
	{
		worldsJson += world.second->Serialize();

		auto objectGroups = world.second->GetObjectGroups();
		for (const auto& group : objectGroups)
		{
			groupsJson += group->Serialize();

			for (const auto& object : group->GetObjects())
			{
				objectsJson += object->Serialize();

				for (const auto& component : object->GetComponents<Component>())
					componentsJson += component->Serialize();
			}
		}
	}

	std::ofstream file("save_worlds.json");
	file << worldsJson.dump(4);
	file.close();

	std::ofstream file2("save_objectGroups.json");
	file2 << groupsJson.dump(4);
	file2.close();

	std::ofstream file3("save_objects.json");
	file3 << objectsJson.dump(4);
	file3.close();

	std::ofstream file4("save_components.json");
	file4 << componentsJson.dump(4);
	file4.close();

}

void WorldManager::LoadWorlds()
{
	json worldsJson, groupsJson, objectsJson, componentsJson;

	std::ifstream worldsFile("save_worlds.json");
	worldsFile >> worldsJson;
	worldsFile.close();

	std::ifstream groupsFile("save_objectGroups.json");
	groupsFile >> groupsJson;
	groupsFile.close();

	std::ifstream objectsFile("save_objects.json");
	objectsFile >> objectsJson;
	objectsFile.close();

	std::ifstream componentsFile("save_components.json");
	componentsFile >> componentsJson;
	componentsFile.close();


	for (auto& worldJson : worldsJson)
	{
		//월드 생성 
		std::wstring worldName = Helper::ToWString(worldJson["name"].get<std::string>());
		World* world = CreateWorld<World>(worldName);
		world->SetId(worldJson["id"].get<unsigned int>());
		world->Deserialize(worldJson); //이 안에서 속한 그룹 생성 (id, 이름 설정)
	}

	for (auto& groupJson : groupsJson)
	{
		unsigned int id = groupJson["id"].get<unsigned int>();
		ObjectGroup* group = static_cast<ObjectGroup*>(Engine::SaveBase::mMap[id]);
		group->Deserialize(groupJson); 
	}

	for (auto& objectJson : objectsJson)
	{
		unsigned int id = objectJson["id"].get<unsigned int>();
		Object* object = static_cast<Object*>(Engine::SaveBase::mMap[id]);
		object->Deserialize(objectJson);
	}

	for (auto& componentJson : componentsJson)
	{
		unsigned int id = componentJson["id"].get<unsigned int>();
		Component* component = static_cast<Component*>(Engine::SaveBase::mMap[id]);
		component->Deserialize(componentJson);
	}

}
