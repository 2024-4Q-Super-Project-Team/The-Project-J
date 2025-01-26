#pragma once
#include "Editor/Interface/IEditorObject.h"
#include "Manipulate/GuizmoManipulater.h"

namespace Editor
{
	class GuizmoHandler : public IWidget
	{
	public:
		explicit GuizmoHandler();
		virtual ~GuizmoHandler();
	public:
		virtual void Render() override;
	public:
		GuizmoManipulater*	mManipulater = nullptr;
	};
}
