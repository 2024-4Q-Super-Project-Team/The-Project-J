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

    void GuizmoManipulater::SetLocalMatrixFromWorldMatrix(const Matrix& _worldMatrix)
    {
        if (mFocusObject == nullptr)
            return;

        // 부모 객체가 있는 경우 부모 객체의 월드 행렬을 가져옴
        Matrix parentWorldMatrix = Matrix::Identity;
        if (mFocusObject->transform->GetParent() != nullptr)
        {
            parentWorldMatrix = mFocusObject->transform->GetParent()->GetWorldMatrix();
        }

        // 부모의 월드 행렬의 역행렬 구해서 로컬 구하기
        Matrix parentWorldMatrixInverse = parentWorldMatrix.Invert();
        Matrix localMatrix = _worldMatrix * parentWorldMatrixInverse;

        mFocusObject->transform->SetLocalMatrix(localMatrix);
    }

    void GuizmoManipulater::Render()
    {
        if (mFocusObject == nullptr)
            return;

        auto pos    =   EditorManager::mFocusViewport->GetIWindow()->GetPosition();
        auto size   =   EditorManager::mFocusViewport->GetIWindow()->GetSize();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, size.x, size.y);
        ImGuizmo::SetOrthographic(false);

        Matrix worldMatrix = mFocusObject->transform->GetWorldMatrix();

        if (CanUseManipulate)
        {
            if (ImGuizmo::Manipulate(
                *EditorManager::mEditorCamera.mViewMatrix.m,
                *EditorManager::mEditorCamera.mProjectionMatrix.m,
                mCurrentGizmoOperation,
                ImGuizmo::MODE::LOCAL,
                *worldMatrix.m))
            {
				SetLocalMatrixFromWorldMatrix(worldMatrix); // 부모 월드 역행렬 사용해서 로컬 좌표로 변환
            }
        }
        if (ImGuizmo::IsUsing() == true)
        {
            // Manipulate를 사용중인 첫 프레임에 스택에 쌓고 Using플래그를 On
            if (isUsingManipulate == false)
            {
                isUsingManipulate = true;
                UndoMatrixStack.push_back({ mFocusObject, mFocusObject->transform->GetLocalMatrix() });
            }
            //// Manipulate를 사용중에 ESC를 누르면 스택에서 빼고 취소
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
        // Manipulate의 사용이 끝난 첫 프레임이면 Using플래그를 Off
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
        // 뒤로가기 스택이 비어있지 않다면
        if (UndoMatrixStack.empty() == false)
        {
            auto& [focusObject, matrix] = UndoMatrixStack.back();
            focusObject->transform->SetLocalMatrix(matrix);
            UndoMatrixStack.pop_back();
        }
    }

    void GuizmoManipulater::RotateFocusObject()
    {
        //if (mFocusObject)
        //{
        //    mFocusObject->transform->Rotate90(2.0f, Dotween::EasingEffect::InOutQuad);
        //}
    }

	void GuizmoManipulater::LookAtTargetObject()
	{
		if (mFocusObject)
		{
            Vector3 targetPosition(10.0f, 0.0f, 10.0f); // 임의로 일단 정해놓음. 바라볼 포지션
			mFocusObject->transform->LookAt(targetPosition, 2.0f, Dotween::EasingEffect::InOutQuad);
		}
	}
}