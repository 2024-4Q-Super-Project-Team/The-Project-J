#pragma once
#include "Editor/Interface/IEditorObject.h"

namespace Editor
{
	class EditorDebugger : public IWidget
	{
	public:
		explicit EditorDebugger();
		virtual ~EditorDebugger() = default;
	public:
		virtual void Render() override;
	};
}