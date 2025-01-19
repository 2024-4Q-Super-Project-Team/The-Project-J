#pragma once
#include "Resource/ResourceManager.h"

struct EditorItemState
{
	void* mItemPtr = nullptr;
	std::string mName = "";
};

// ������ �巡�� �� ��� �ڵ鷯
class EditorDragNDrop
{
public:
	static void Render();
private:
	static BOOL isDragging;
	static EditorItemState mItemState;
public:
	static void SendDragAndDropData(const char* _uid, IN EditorItemState _itemState)
	{
		ImGui::PushID(_uid);
		if (isDragging == FALSE && ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			mItemState = _itemState;
			isDragging = true;
		}
		ImGui::PopID();
	}
    template <typename T>
    static void ReceiveDragAndDropResourceData(const char* _uid, OUT std::shared_ptr<T>& _data)
    {
        ImGui::PushID(_uid);
        if ( isDragging == TRUE && ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) )
        {
            std::wstring resourceName = Helper::ToWString(EditorDragNDrop::mItemState.mName);
            //_data = ResourceManager::RequestResource<T>(resourceName);
            isDragging = false;
        }
        ImGui::PopID();
    }
};
