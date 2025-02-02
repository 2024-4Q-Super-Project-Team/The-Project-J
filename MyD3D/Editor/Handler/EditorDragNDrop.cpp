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
        mItemState.Clear();
    }
}

BOOL EditorDragNDrop::ReceiveDragAndDropObjectData(const char* _uid, Object** _pObject)
{
    BOOL isOk = FALSE;
    ImGui::PushID(_uid);
    if (isDragging)
    {
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            if (ImGui::IsItemHovered(
                ImGuiHoveredFlags_AllowWhenBlockedByActiveItem 
                //ImGuiHoveredFlags_AllowWhenOverlapped  |
                //ImGuiHoveredFlags_AllowWhenDisabled |//비활성화된 요소(버튼 등) 위에서도 감지.
                //ImGuiHoveredFlags_AllowWhenOverlappedByItem | //다른 아이템이 겹쳐져 있어도 감지.
                //ImGuiHoveredFlags_AllowWhenOverlappedByWindow | //다른 윈도우가 위에 덮여 있어도 감지.
            ))
            {
                *_pObject = EditorDragNDrop::mItemState.mObjectPtr;
                EditorDragNDrop::mItemState.Clear();
                isDragging = false;
                isOk = TRUE;
            }
        }
    }
    ImGui::PopID();
    return isOk;
}
