#pragma once
#include "Component/EditorUI/Contents/EditorWidget.h"
#include "Component/EditorUI/Contents/EditorUID.h"

namespace Editor
{
	class EditorResourceView
		: public Widget, public EUID
	{
	public:
		explicit EditorResourceView();
		virtual ~EditorResourceView();
	public:
		virtual void Render() override;
	};
}