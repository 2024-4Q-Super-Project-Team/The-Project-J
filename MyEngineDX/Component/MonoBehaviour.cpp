#include "pch.h"
#include "MonoBehaviour.h"
#include "GameManager/GameManager.h"
#include "Viewport/ViewportManager.h"
#include "World/WorldManager.h"
#include "ObjectGroup/ObjectGroup.h"
#include "Object/Object.h"

namespace Engine
{
	MonoBehaviour::MonoBehaviour()
	{
	}
	MonoBehaviour::~MonoBehaviour()
	{
	}

	GameObject* MonoBehaviour::FindObject(std::wstring_view _name, std::wstring_view _tag)
	{
		ViewportScene* curViewport = GameManager::GetViewportManager()->GetActiveViewport();
		DXWorld* curWorld = curViewport->GetWorldManager()->GetActiveWorld();
		const std::vector<ObjectGroup*>& objectGroups = curWorld->GetObjectGroups();
		for (auto group : objectGroups)
		{
			const std::list<GameObject*>& objects = group->GetObjects();
			auto itr = FIND_CONTAINER(objects,
				[&](GameObject* _obj) {
					return (_obj->GetName() == _name.data() && _obj->GetTag() == _tag.data());
				});
			if (FIND_SUCCES(itr, objects))
			{
				return (*itr);
			}
		}
		return nullptr;
	}
	GameObject* MonoBehaviour::FindObjectWithName(std::wstring_view _name)
	{
		ViewportScene* curViewport = GameManager::GetViewportManager()->GetActiveViewport();
		DXWorld* curWorld = curViewport->GetWorldManager()->GetActiveWorld();
		const std::vector<ObjectGroup*>& objectGroups = curWorld->GetObjectGroups();
		for (auto group : objectGroups)
		{
			const std::list<GameObject*>& objects = group->GetObjects();
			auto itr = FIND_CONTAINER(objects,
				[&](GameObject* _obj) {
					return (_obj->GetName() == _name.data());
				});
			if (FIND_SUCCES(itr, objects))
			{
				return (*itr);
			}
		}
		return nullptr;
	}
	std::vector<GameObject*> MonoBehaviour::FindObjectsWithTag(std::wstring_view _tag)
	{
		std::vector<GameObject*> tempArr;
		ViewportScene* curViewport = GameManager::GetViewportManager()->GetActiveViewport();
		DXWorld* curWorld = curViewport->GetWorldManager()->GetActiveWorld();
		const std::vector<ObjectGroup*>& objectGroups = curWorld->GetObjectGroups();
		for (auto& group : objectGroups)
		{
			const std::list<GameObject*>& objects = group->GetObjects();
			for (auto& object : objects)
			{
				if (object->GetTag() == _tag.data())
					tempArr.push_back(object);
			}
		}
		return tempArr;
	}
	std::vector<GameObject*> MonoBehaviour::FindObjectsWithName(std::wstring_view _name)
	{
		std::vector<GameObject*> tempArr;
		ViewportScene* curViewport = GameManager::GetViewportManager()->GetActiveViewport();
		DXWorld* curWorld = curViewport->GetWorldManager()->GetActiveWorld();
		const std::vector<ObjectGroup*>& objectGroups = curWorld->GetObjectGroups();
		for (auto group : objectGroups)
		{
			const std::list<GameObject*>& objects = group->GetObjects();
			for (auto& object : objects)
			{
				if (object->GetName() == _name.data())
					tempArr.push_back(object);
			}
		}
		return tempArr;
	}

	GameObject* MonoBehaviour::Instantiate(Resource::Prefab* _pInstant)
	{
		return nullptr;
	}
	GameObject* MonoBehaviour::Instantiate(Resource::Prefab* _pInstant, Vector3 _position)
	{
		return nullptr;
	}

	void MonoBehaviour::Destroy(GameObject* _object)
	{
		_object->SetDestroy();
	}
}

