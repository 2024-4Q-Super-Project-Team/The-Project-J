#include "pch.h"
#include "EditorWorldCreator.h"
#include "Editor/EditorManager.h"
#include "World/WorldManager.h"

Editor::WorldCreator::WorldCreator()
{
	mlabel = ("Add World##" + std::to_string(reinterpret_cast<uintptr_t>(this))).c_str();
}

Editor::WorldCreator::~WorldCreator()
{
}

void Editor::WorldCreator::Render()
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    if (ImGui::BeginMenu(mlabel.c_str()))
    {
        auto pFocusWorldManager = EditorManager::GetHierarchyViewer()->GetFocusWorldManager();
        auto pWorldContainer = pFocusWorldManager->GetWorlds();

        static char buffer[64] = "";
        ImGui::Text("Enter World Name:");
        if (ImGui::InputText("##world_name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            const std::wstring worldName = Helper::ToWString(std::string(buffer));
            pFocusWorldManager->CreateWorld(worldName);
            const char* nullStr = "";
            strcpy_s(buffer, nullStr);
        }

        ImGui::EndMenu();
    }
}
