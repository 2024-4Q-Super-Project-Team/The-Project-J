#include "pch.h"
#include "EditorStartWorldSetter.h"
#include "World/WorldManager.h"
#include "World/World.h"

Editor::StartWorldSetter::StartWorldSetter()
{
	mlabel = ("Set Start World##" + std::to_string(reinterpret_cast<uintptr_t>(this))).c_str();
}

Editor::StartWorldSetter::~StartWorldSetter()
{
}

void Editor::StartWorldSetter::Render()
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
                pFocusWorldManager->SetStartWorld(world);
            }
        }
        ImGui::EndMenu();
    }
}
