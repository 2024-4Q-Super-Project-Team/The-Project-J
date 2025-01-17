#pragma once
#include "Resource/ResourceManager.h"

struct EditorItemState
{
	void* mItemPtr = nullptr;
	std::string mName = "";
};

// 에디터 드래그 앤 드롭 핸들러
class EditorDragNDrop
{
public:
	static void Render();
private:
	static BOOL isDragging;
	static EditorItemState mItemState;
public:
	static void SendDragAndDropData(IN EditorItemState _itemState)
	{
		if (isDragging == FALSE && ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			mItemState = _itemState;
			isDragging = true;
		}
	}
    template <typename T>
    static void ReceiveDragAndDropResourceData(const char* _uid, OUT std::shared_ptr<T>& _data)
    {
        ImGui::PushID(_uid);
        if ( isDragging == TRUE && ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) )
        {
            std::wstring resourceName = Helper::ToWString(EditorDragNDrop::mItemState.mName);
            _data = ResourceManager::GetResource<T>(resourceName);
            isDragging = false;
        }
        ImGui::PopID();
    }
};
