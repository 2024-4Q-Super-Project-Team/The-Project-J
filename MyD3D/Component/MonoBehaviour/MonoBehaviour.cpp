#include "pch.h"
#include "MonoBehaviour.h"
#include "Manager/GameManager.h"
#include "ViewportScene/ViewportManager.h"
#include "World/WorldManager.h"
#include "Editor/EditorManager.h"
#include "ViewportScene/ViewportScene.h"
#include "Object/Object.h"
#include "World/World.h"
#include "Resource/Prefab/Prefab.h"

MonoBehaviour::MonoBehaviour(Object* _owner, const std::string& _eid)
    : Component(_owner)
{
    mType = eComponentType::SCRIPT;
	SetEID(_eid);
}

Object* MonoBehaviour::FindObject(std::wstring_view _name, std::wstring_view _tag)
{
    World* curWorld = GameManager::GetCurrentWorld();
	auto& objects = curWorld->GetObjectArray();
	auto itr = FIND_CONTAINER(objects,
		[&](Object* _obj) {
			return (_obj->GetName() == _name.data() && _obj->GetTag() == _tag.data());
		});
	if (FIND_SUCCESS(itr, objects))
	{
		return (*itr);
	}
	return nullptr;
}
Object* MonoBehaviour::FindObjectWithName(std::wstring_view _name)
{
    World* curWorld = GameManager::GetCurrentWorld();
	auto& objects = curWorld->GetObjectArray();
	auto itr = FIND_CONTAINER(objects,
		[&](Object* _obj) {
			return (_obj->GetName() == _name.data());
		});
	if (FIND_SUCCESS(itr, objects))
	{
		return (*itr);
	}
	return nullptr;
}
std::vector<Object*> MonoBehaviour::FindObjectsWithTag(std::wstring_view _tag)
{
	std::vector<Object*> tempArr;
    World* curWorld = GameManager::GetCurrentWorld();
	auto& objects = curWorld->GetObjectArray();
	for (auto& object : objects)
	{
		if (object->GetTag() == _tag.data())
			tempArr.push_back(object);
	}
	return tempArr;
}
std::vector<Object*> MonoBehaviour::FindObjectsWithName(std::wstring_view _name)
{
	std::vector<Object*> tempArr;
    World* curWorld = GameManager::GetCurrentWorld();
	auto& objects = curWorld->GetObjectArray();
	for (auto& object : objects)
	{
		if (object->GetName() == _name.data())
			tempArr.push_back(object);
	}
	return tempArr;
}

Object* MonoBehaviour::Instantiate(PrefabResource* _pInstant)
{
    if (_pInstant)
    {
		World* curWorld = GameManager::GetCurrentWorld();
        std::list<Object*>	cloneArray;
        std::unordered_map<std::wstring, Object*>	cloneTable;

        cloneTable.reserve(_pInstant->mObjectTable.size());

		if (curWorld)
		{
			for (auto itr = _pInstant->mObjectList.begin(); itr != _pInstant->mObjectList.end(); ++itr)
			{
				Object* clone = curWorld->CreateObject((*itr)->GetName(), (*itr)->GetTag());
				cloneTable[clone->GetName()] = clone;
				cloneArray.push_back(clone);
			}

			auto clone = cloneArray.begin();
			auto PrefabResource = _pInstant->mObjectList.begin();

			// 만들어둔 오브젝트 Clone(컴포넌트)
			while (clone != cloneArray.end())
			{
				(*PrefabResource)->Clone((*clone), cloneTable);
				clone++;
				PrefabResource++;
			}
			return cloneArray.front();
		}
    }
    return nullptr;
}

Object* MonoBehaviour::Instantiate(PrefabResource* _pInstant, Vector3 _position)
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
	World* curWorld = GameManager::GetCurrentWorld();
	if (curWorld)
	{
		return curWorld->CreateObject(_name, _tag);
	}
	return nullptr;
}

void MonoBehaviour::Destroy(Object* _object)
{
	_object->SetDestroy();
}

// JSON_TODO :
// 모노비헤이어 시리얼라이즈 이거 맞나요? 진짜 몰라서 물어봄. 확인점
json MonoBehaviour::Serialize()
{
	json ret;
	ret["id"] = GiveId();
	ret["name"] = GetEID();
	ret["initial active"] = isActive;
	return ret;
}

void MonoBehaviour::Deserialize(json& j)
{
	SetId(j["id"].get<unsigned int>());
	isActive = j["initial active"].get<bool>();
}

void MonoBehaviour::EditorRendering(EditorViewerType _viewerType)
{
	for (auto serial : mSerials)
	{
		serial->widget->Render();
	}
}
