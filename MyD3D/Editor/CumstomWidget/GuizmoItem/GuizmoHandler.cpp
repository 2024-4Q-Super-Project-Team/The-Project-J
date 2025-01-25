#include "pch.h"
#include "GuizmoHandler.h"

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
        if (mManipulater)
        {
            mManipulater->Render();
        }
        // ����� ��� ���� ��ư �׽�Ʈ
        // ���� �� �̻ڰ� ����
        bool isOpen = true;
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

