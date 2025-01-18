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

        // �巡�� ���� ���, ���콺 ��ġ ��ó�� �������� â ǥ��
        ImGui::Begin("DragOverlay", nullptr, flags);

        ImGui::Text(mItemState.mName.c_str());

        ImGui::End();
    }
    // ���콺�� ���������� isDragging�� false�� �ٲ۴�.
    if (isDragging == TRUE && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        isDragging = false; // �巡�� ���� ����
        mItemState = {};
    }
}
