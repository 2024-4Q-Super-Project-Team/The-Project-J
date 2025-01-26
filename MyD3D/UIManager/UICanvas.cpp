#include "pch.h"
#include "UICanvas.h"

#include "UIManager/Widget/Widget.h"

UICanvas::UICanvas(Object* _owner)
	: Component(_owner)
{

}

UICanvas::~UICanvas()
{
}

void UICanvas::PushDrawList()
{
	if (!mWidgets.empty())
		return;

	for (auto& e : mWidgets)
	{
		if (e->IsActive())
		{
			UIManager::mDrawWidgetList.push_back(e);
		}
	}
}