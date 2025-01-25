#include "pch.h"
#include "SaveManager.h"
#include "ObjectGroup/ObjectGroup.h"
#include "Object/Object.h"
#include "World/WorldManager.h"
#include "World/World.h"
#include "Component/Component.h"
#include "ViewportScene/ViewportManager.h"
#include "ViewportScene/ViewportScene.h"

std::string SaveManager::filePath = "../Save/";

void SaveManager::Save()
{
	ResourceManager::SaveResources();

	auto viewports = ViewportManager::GetViewports();

	for (auto& viewport : viewports)
	{
		auto& worlds = viewport->GetWorldManager()->GetWorlds();

		json defaultJson, worldsJson, groupsJson, objectsJson, componentsJson;

		for (const auto& world : worlds)
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
		std::ofstream file1(filePath + Helper::ToString(viewport->GetName()) + "-worlds" + ".json");
		file1 << worldsJson.dump(4);
		file1.close();

		std::ofstream file2(filePath + Helper::ToString(viewport->GetName()) + "-objectGroups" + ".json");
		file2 << groupsJson.dump(4);
		file2.close();

		std::ofstream file3(filePath + Helper::ToString(viewport->GetName()) + "-objects" + ".json");
		file3 << objectsJson.dump(4);
		file3.close();

		std::ofstream file4(filePath + Helper::ToString(viewport->GetName()) + "-components" +  ".json");
		file4 << componentsJson.dump(4);
		file4.close();
	}
}

void SaveManager::Load()
{
	auto viewports = ViewportManager::GetViewports();

	for (auto& viewport : viewports)
	{
		auto& worlds = viewport->GetWorldManager()->GetWorlds();

		json defaultsJson, worldsJson, groupsJson, objectsJson, componentsJson;

		std::ifstream worldsFile(filePath + Helper::ToString(viewport->GetName()) + "-worlds.json");
		if (worldsFile.is_open())
		{
			worldsFile >> worldsJson;
			worldsFile.close();
		}

		std::ifstream groupsFile(filePath + Helper::ToString(viewport->GetName()) + "-objectGroups.json");
		if (groupsFile.is_open())
		{
			groupsFile >> groupsJson;
			groupsFile.close();
		}

		std::ifstream objectsFile(filePath + Helper::ToString(viewport->GetName()) + "-objects.json");
		if (objectsFile.is_open())
		{
			objectsFile >> objectsJson;
			objectsFile.close();
		}

		std::ifstream componentsFile(filePath + Helper::ToString(viewport->GetName()) + "-components.json");
		if (componentsFile.is_open())
		{
			componentsFile >> componentsJson;
			componentsFile.close();
		}

		for (auto& worldJson : worldsJson)
		{
			//월드 생성 
			std::wstring worldName = Helper::ToWString(worldJson["name"].get<std::string>());
			World* world = new World(viewport->GetWorldManager()->GetViewportScene(), worldName, L"", false);
			viewport->GetWorldManager()->AddWorld(world);

			world->SetId(worldJson["id"].get<unsigned int>());
			world->Deserialize(worldJson); //이 안에서 속한 그룹 생성 (id, 이름 설정)
		}


		for (auto& groupJson : groupsJson)
		{
			unsigned int id = groupJson["id"].get<unsigned int>();
			ObjectGroup* group = static_cast<ObjectGroup*>(Engine::SaveBase::mMap[id]);
			if (group)
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
			if (component)
			{
				component->Deserialize(componentJson);
			}
		}
	}
	
}
