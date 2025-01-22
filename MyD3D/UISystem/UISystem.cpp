#include "pch.h"
#include "UISystem.h"

#include "Widget/Widget.h"
#include "Component/Camera/Camera.h"

template <class T>
T* AddWidget(Widget* _pWidget);

UISystem::UISystem()
{

}

UISystem::~UISystem()
{

}

void UISystem::Start()
{
	if (mWidgetContainer.empty())
		return;

	for (auto& widget : mWidgetContainer)
	{
		widget->Start();
	}
}

void UISystem::Tick()
{

}

void UISystem::FixedUpdate()
{

}

void UISystem::PreUpdate()
{

}

void UISystem::Update()
{
	if (mWidgetContainer.empty())
		return;

	for (auto& widget : mWidgetContainer)
	{
		widget->Update();
	}
}

void UISystem::PostUpdate()
{

}

void UISystem::PreRender()
{

}

void UISystem::Render()
{
	if (mWidgetContainer.empty())
		return;

	for (auto& widget : mWidgetContainer)
	{
		widget->Render();
	}
}

void UISystem::Draw(Camera* _pCamera)
{

}

void UISystem::PostRender()
{

}
