#pragma once
#include "Editor/Interface/IEditorObject.h"

namespace Editor
{
	class EditorResourceView
		: public IWidget
	{
	public:
		explicit EditorResourceView();
		virtual ~EditorResourceView();
	public:
		virtual void Render() override;
	};
}