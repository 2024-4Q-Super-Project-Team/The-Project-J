#pragma once
#include "Resource/ResourceManager.h"

struct EditorItemState
{
	Object*		mObjectPtr = nullptr;
	Resource*	mResourcePtr = nullptr;
	Component*	mComponentPtr = nullptr;
	std::string mName = "";	// popup에 보일 이름
	void Clear() {
		mObjectPtr = nullptr;
		mResourcePtr = nullptr;
		mComponentPtr = nullptr;
		mName = "";
	}
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
    static BOOL ReceiveDragAndDropResourceData(const char* _uid, ResourceHandle* _handle)
    {
		BOOL isOk = FALSE;
        ImGui::PushID(_uid);
        if (isDragging == TRUE && ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) )
        {
			T* pData = dynamic_cast<T*>(EditorDragNDrop::mItemState.mResourcePtr);
			if (pData)
			{
				(*_handle) = EditorDragNDrop::mItemState.mResourcePtr->GetHandle();
				EditorDragNDrop::mItemState.Clear();
				isDragging = false;
				isOk = TRUE;
			}
        }
        ImGui::PopID();

		return isOk;
    }
	template <typename T>
	static BOOL ReceiveDragAndDropComponentData(const char* _uid, T** _pComponent)
	{
		BOOL isOk = FALSE;
		ImGui::PushID(_uid);
		if (isDragging == TRUE && ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			T* pData = dynamic_cast<T*>(EditorDragNDrop::mItemState.mComponentPtr);
			if (pData)
			{
				(*_pComponent) = pData;
				EditorDragNDrop::mItemState.Clear();
				isDragging = false;
				isOk = TRUE;
			}
		}
		ImGui::PopID();

		return isOk;
	}
	static BOOL ReceiveDragAndDropObjectData(const char* _uid, Object** _pObject);
};
