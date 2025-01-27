#pragma once
#include "Editor/Interface/IEditorObject.h"

namespace Editor
{
	class IWidget;

	class ResourceViewer
		: public IWidget
	{
	public:
		explicit ResourceViewer();
		virtual ~ResourceViewer();
	public:
		virtual void Render() override;
	private:
		std::vector<const char*> mResourceStringArray;
		bool isLoadResorceListPopupOpen = false;
		void LoadResourceListPopup();
	};
}