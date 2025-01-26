#include "pch.h"
#include "EditorWorldChanger.h"
#include "Editor/EditorManager.h"
#include "World/WorldManager.h"
#include "World/World.h"

namespace Editor
{
    WorldChanger::WorldChanger()
    {
        mlabel = ("Change World##" + std::to_string(reinterpret_cast<uintptr_t>(this))).c_str();
    }

    WorldChanger::~WorldChanger()
    {
    }

    void WorldChanger::Render()
    {
        std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
        if (ImGui::BeginMenu(mlabel.c_str()))
        {
            auto pFocusWorldManager = EditorManager::GetHierarchyViewer()->GetFocusWorldManager();
            auto pWorldContainer = pFocusWorldManager->GetWorlds();
            for (auto& world : pWorldContainer)
            {
                std::string worldName = Helper::ToString(world->GetName());
                if (ImGui::MenuItem((worldName + uid).c_str()))
                {
                    pFocusWorldManager->SetActiveWorld(world);
                }
            }
            ImGui::EndMenu();
        }
    }

}
