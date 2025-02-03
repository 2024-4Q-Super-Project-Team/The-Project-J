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
		void SetLocalMatrixFromWorldMatrix(const Matrix& _worldMatrix);
	public:
		static inline void SetFocusObjedct(Object* _pObject) { mFocusObject = _pObject; }
		static inline Object* GetFocusObject() { return mFocusObject; }
		static inline bool IsFocusObject(Object* _object) { return mFocusObject == _object; }
		inline void SetGizmoOperation(ImGuizmo::OPERATION operation) { mCurrentGizmoOperation = operation; }
		void RotateFocusObject();
		void LookAtTargetObject();
		void GoToTarget();
		

	private:
		static Object* mFocusObject;
		ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		bool isUsingManipulate = false;
		bool CanUseManipulate = true;
		std::vector<std::pair<Object*, Matrix>> UndoMatrixStack;
	};
}