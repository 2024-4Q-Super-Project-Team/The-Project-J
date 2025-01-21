#include "pch.h"
#include "EditorJsonLoader.h"

Editor::JsonLoader::JsonLoader()
{
	mlabel = ("Save File##" + std::to_string(reinterpret_cast<uintptr_t>(this))).c_str();
}

Editor::JsonLoader::~JsonLoader()
{
}

void Editor::JsonLoader::Render()
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    if (ImGui::BeginMenu(mlabel.c_str()))
    {
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            // 상태 전환
            if (GameManager::GetRunType() == eEngineRunType::GAME_MODE)
            {
                GameManager::SetRunType(eEngineRunType::EDITOR_MODE);
            }
            else
            {
                GameManager::SetRunType(eEngineRunType::GAME_MODE);
            }
        }
        ImGui::EndMenu();
    }
}
