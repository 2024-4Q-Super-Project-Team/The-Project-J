#include "pch.h"
#include "EditorDragNDrop.h"

BOOL			EditorDragNDrop::isDragging = FALSE;
EditorItemState EditorDragNDrop::mItemState = {};

void EditorDragNDrop::Render()
{
    if (isDragging)
    {
        UINT flags =
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize | 
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoSavedSettings | 
            ImGuiWindowFlags_NoFocusOnAppearing | 
            ImGuiWindowFlags_NoInputs | 
            ImGuiWindowFlags_Tooltip;

        ImVec2 mousePos = ImGui::GetMousePos();

        ImGui::SetNextWindowPos(ImVec2(mousePos.x + 10, mousePos.y + 10));

        // 드래그 중인 경우, 마우스 위치 근처에 오버레이 창 표시
        ImGui::Begin("DragOverlay", nullptr, flags);

        ImGui::Text(mItemState.mName.c_str());

        ImGui::End();
    }
    // 마우스가 떼져있으면 isDragging을 false로 바꾼다.
    if (isDragging == TRUE && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        isDragging = false; // 드래그 상태 해제
        mItemState = {};
    }
}
