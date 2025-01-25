#pragma once
#include "Editor/Interface/IEditorObject.h"

class Object;

namespace Editor
{
	class GuizmoManipulater : public IWidget
	{
	public:
		explicit GuizmoManipulater();
		virtual ~GuizmoManipulater() = default;
	public:
		virtual void Render() override;
	public:
		void ProcessUndo();
	public:
		inline void SetFocusObjedct(Object* _pObject) { mFocusObject = _pObject; }
		inline void SetGizmoOperation(ImGuizmo::OPERATION operation) { mCurrentGizmoOperation = operation; }
	private:
		Object* mFocusObject = nullptr;
		ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		bool isUsingManipulate = false;
		bool CanUseManipulate = true;
		std::vector<std::pair<Object*, Matrix>> UndoMatrixStack;
	};
}