#include "pch.h"
#include "EditorWorldRemover.h"
#include "Editor/EditorManager.h"
#include "World/WorldManager.h"

Editor::WorldRemover::WorldRemover()
{
	mlabel = ("Delete World##" + std::to_string(reinterpret_cast<uintptr_t>(this))).c_str();
}

Editor::WorldRemover::~WorldRemover()
{
}

void Editor::WorldRemover::Render()
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    if (ImGui::BeginMenu(mlabel.c_str()))
    {
        auto pFocusWorldManager = EditorManager::GetHierarchyViewer()->GetFocusWorldManager();
        auto pWorldContainer = pFocusWorldManager->GetWorlds();
        for (auto& [name, world] : pWorldContainer)
        {
            std::string worldName = Helper::ToString(name);
            if (ImGui::MenuItem((worldName + uid).c_str()))
            {
                pFocusWorldManager->DestroyWorld(name);
            }
        }
        ImGui::EndMenu();
    }
}
