#include "pch.h"
#include "MonoBehaviour.h"
#include "Manager/GameManager.h"
#include "ViewportScene/ViewportManager.h"
#include "World/WorldManager.h"
#include "Editor/EditorManager.h"
#include "ViewportScene/ViewportScene.h"
#include "ObjectGroup/ObjectGroup.h"
#include "Object/Object.h"
#include "World/World.h"
#include "Resource/Prefab/Prefab.h"

MonoBehaviour::MonoBehaviour(Object* _owner)
    : Component(_owner)
{
    mType = eComponentType::SCRIPT;

}

Object* MonoBehaviour::FindObject(std::wstring_view _name, std::wstring_view _tag)
{
    World* curWorld = GameManager::GetCurrentWorld();
	const std::vector<ObjectGroup*>& objectGroups = curWorld->GetObjectGroups();
	for (auto group : objectGroups)
	{
		const std::list<Object*>& objects = group->GetObjects();
		auto itr = FIND_CONTAINER(objects,
			[&](Object* _obj) {
				return (_obj->GetName() == _name.data() && _obj->GetTag() == _tag.data());
			});
		if (FIND_SUCCESS(itr, objects))
		{
			return (*itr);
		}
	}
	return nullptr;
}
Object* MonoBehaviour::FindObjectWithName(std::wstring_view _name)
{
    World* curWorld = GameManager::GetCurrentWorld();
	const std::vector<ObjectGroup*>& objectGroups = curWorld->GetObjectGroups();
	for (auto group : objectGroups)
	{
		const std::list<Object*>& objects = group->GetObjects();
		auto itr = FIND_CONTAINER(objects,
			[&](Object* _obj) {
				return (_obj->GetName() == _name.data());
			});
		if (FIND_SUCCESS(itr, objects))
		{
			return (*itr);
		}
	}
	return nullptr;
}
std::vector<Object*> MonoBehaviour::FindObjectsWithTag(std::wstring_view _tag)
{
	std::vector<Object*> tempArr;
    World* curWorld = GameManager::GetCurrentWorld();
	const std::vector<ObjectGroup*>& objectGroups = curWorld->GetObjectGroups();
	for (auto& group : objectGroups)
	{
		const std::list<Object*>& objects = group->GetObjects();
		for (auto& object : objects)
		{
			if (object->GetTag() == _tag.data())
				tempArr.push_back(object);
		}
	}
	return tempArr;
}
std::vector<Object*> MonoBehaviour::FindObjectsWithName(std::wstring_view _name)
{
	std::vector<Object*> tempArr;
    World* curWorld = GameManager::GetCurrentWorld();
	const std::vector<ObjectGroup*>& objectGroups = curWorld->GetObjectGroups();
	for (auto group : objectGroups)
	{
		const std::list<Object*>& objects = group->GetObjects();
		for (auto& object : objects)
		{
			if (object->GetName() == _name.data())
				tempArr.push_back(object);
		}
	}
	return tempArr;
}

Object* MonoBehaviour::Instantiate(Prefab* _pInstant)
{
    if (_pInstant)
    {
        std::list<Object*>                          cloneArray;
        std::unordered_map<std::wstring, Object*>   cloneTable;

        cloneTable.reserve(_pInstant->mObjectTable.size());

        World*          curWorld = GameManager::GetCurrentWorld();
        ObjectGroup*    destGroup = curWorld->GetObjectGroup(_pInstant->mGroupName);

        /// 그룹이 없으면 만든다.
        if (destGroup == nullptr)
        {
            destGroup = curWorld->CreateObjectGroup(_pInstant->mGroupName);
        }

        for (auto itr = _pInstant->mObjectList.begin(); itr != _pInstant->mObjectList.end(); ++itr)
        {
            Object* clone = destGroup->CreateObject((*itr)->GetName(), (*itr)->GetTag());
            cloneTable[clone->GetName()] = clone;
            cloneArray.push_back(clone);
        }

        auto clone = cloneArray.begin();
        auto prefab = _pInstant->mObjectList.begin();

        // 만들어둔 오브젝트 Clone(컴포넌트)
        while (clone != cloneArray.end())
        {
            (*prefab)->Clone((*clone), cloneTable);
            clone++;
            prefab++;
        }
        return cloneArray.front();
    }
    return nullptr;
}

Object* MonoBehaviour::Instantiate(Prefab* _pInstant, Vector3 _position)
{
    Object* pObject = Instantiate(_pInstant);
    if (pObject)
    {
        pObject->transform->position = _position;
    }
	return pObject;
}

Object* MonoBehaviour::CreateObject(std::wstring_view _name, std::wstring_view _tag)
{
    ObjectGroup* pObjectGroup = gameObject->GetOwnerObjectGroup();
	return pObjectGroup->CreateObject(_name, _tag);
}

void MonoBehaviour::Destroy(Object* _object)
{
	_object->SetDestroy();
}

json MonoBehaviour::Serialize()
{
	json ret;
	ret["initial active"] = isActive;
	return ret;
}

void MonoBehaviour::Deserialize( json& j)
{
}

void MonoBehaviour::EditorRendering()
{
	for (auto serial : mSerials)
	{
		serial->widget->Render();
	}
}
