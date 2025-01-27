#include "pch.h"
#include "GuizmoManipulater.h"
#include "Editor/EditorManager.h"
#include "ViewportScene/ViewportScene.h"
#include "ViewportScene/ViewportManager.h"
#include "Object/Object.h"

namespace Editor
{
    Object* GuizmoManipulater::mFocusObject = nullptr;

	GuizmoManipulater::GuizmoManipulater()
	{
	}
    void GuizmoManipulater::Render()
    {
        if (mFocusObject == nullptr)
            return;

        auto pos    =   EditorManager::mFocusViewport->GetIWindow()->GetPosition();
        auto size   =   EditorManager::mFocusViewport->GetIWindow()->GetSize();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, size.x, size.y);
        ImGuizmo::SetOrthographic(false);

        Matrix Matrix = mFocusObject->transform->GetLocalMatrix();

        if (CanUseManipulate)
        {
            if (ImGuizmo::Manipulate(
                *EditorManager::mEditorCamera.mViewMatrix.m,
                *EditorManager::mEditorCamera.mProjectionMatrix.m,
                mCurrentGizmoOperation,
                ImGuizmo::MODE::LOCAL,
                *Matrix.m))
            {
                mFocusObject->transform->SetLocalMatrix(Matrix);
            }
        }
        if (ImGuizmo::IsUsing() == true)
        {
            // Manipulate�� ������� ù �����ӿ� ���ÿ� �װ� Using�÷��׸� On
            if (isUsingManipulate == false)
            {
                isUsingManipulate = true;
                UndoMatrixStack.push_back({ mFocusObject, mFocusObject->transform->GetLocalMatrix() });
            }
            //// Manipulate�� ����߿� ESC�� ������ ���ÿ��� ���� ���
            //else if (isUsingManipulate == true)
            //{
            //    if (Input::IsKeyDown(Key::ESCAPE) || Input::IsMouseDown(Mouse::RIGHT))
            //    {
            //        ProcessUndo();
            //        isUsingManipulate = false;
            //        CanUseManipulate = false;
            //    }
            //}
        }
        // Manipulate�� ����� ���� ù �������̸� Using�÷��׸� Off
        if (Input::IsMouseUp(Mouse::LEFT) && isUsingManipulate == true)
        {
            CanUseManipulate = true;
            isUsingManipulate = false;
        }

        if (Input::IsKeyHold(Key::LCONTROL) && Input::IsKeyDown('Z'))
        {
            ProcessUndo();
        }
    }
    void GuizmoManipulater::ProcessUndo()
    {
        // �ڷΰ��� ������ ������� �ʴٸ�
        if (UndoMatrixStack.empty() == false)
        {
            auto& [focusObject, matrix] = UndoMatrixStack.back();
            focusObject->transform->SetLocalMatrix(matrix);
            UndoMatrixStack.pop_back();
        }
    }

    void GuizmoManipulater::RotateFocusObject()
    {
        if (mFocusObject)
        {
            mFocusObject->transform->Rotate180(2.0f, Dotween::EasingEffect::InOutQuad);
        }
    }

	void GuizmoManipulater::LookAtTargetObject()
	{
		if (mFocusObject)
		{
            Vector3 targetPosition(10.0f, 0.0f, 10.0f); // ���Ƿ�  
			mFocusObject->transform->LookAt(targetPosition, 2.0f, Dotween::EasingEffect::InOutQuad);
		}
	}
}