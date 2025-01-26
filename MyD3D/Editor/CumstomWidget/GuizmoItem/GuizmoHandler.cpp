#include "pch.h"
#include "GuizmoHandler.h"
#include "Object/Object.h"
#include "Save/SaveManager.h"

namespace Editor
{

	GuizmoHandler::GuizmoHandler()
        : mManipulater(new GuizmoManipulater)
	{
	}

    GuizmoHandler::~GuizmoHandler()
    {
        SAFE_DELETE(mManipulater);
    }

	void GuizmoHandler::Render()
	{
        bool isOpen = true;
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        if (ImGui::Begin("Run Mode", &isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
        {
            ImVec2 buttonSize = { 70, 20 };
            if (GameManager::GetRunType() == eEngineRunType::EDITOR_MODE)
            {
                if (ImGui::Button("Play", buttonSize))
                {
                    GameManager::SetRunType(eEngineRunType::GAME_MODE);
                }
            }
            else
            {
                if (ImGui::Button("Stop", buttonSize))
                {
                    GameManager::SetRunType(eEngineRunType::EDITOR_MODE);
                }
            }
            ImGui::End();
        }

        if (GameManager::GetRunType() == eEngineRunType::EDITOR_MODE)
        {
            if (mManipulater)
            {
                mManipulater->Render();
            }
            ImGui::SetNextWindowPos(ImVec2(0, 100));
            if (ImGui::Begin("Gizmo Mode", &isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
            {
                ImVec2 buttonSize = { 70, 20 };
                if (ImGui::Button("Translate", buttonSize) || Input::IsKeyDown('G'))
                {
                    mManipulater->SetGizmoOperation(ImGuizmo::TRANSLATE);
                }
                if (ImGui::Button("Rotate", buttonSize) || Input::IsKeyDown('R'))
                {
                    mManipulater->SetGizmoOperation(ImGuizmo::ROTATE);
                }
                if (ImGui::Button("Scale", buttonSize) || Input::IsKeyDown('S'))
                {
                    mManipulater->SetGizmoOperation(ImGuizmo::SCALE);
                }
                ImGui::End();
            }
        }
	}
}

