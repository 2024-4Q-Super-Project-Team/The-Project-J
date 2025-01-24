#include "pch.h"
#include "UIManager.h"

#include "Widget/Widget.h"
#include "Component/Camera/Camera.h"

#include "Graphics/GraphicsManager.h"

std::vector<Widget*> UIManager::mWidgetContainer = {};

BOOL UIManager::Initialize()
{
	auto pImg = AddWidget<WidgetImage>(L"Cat_01", L"resource/texture/cat.dds");
	pImg->SetPosition(Vector2(500, 150));

	auto pText = AddWidget<WidgetText>(L"Text_01", L"resource/myfile.sfont");
	pText->SetPosition(Vector2(500, 100));
	pText->SetTextLine(14);
	ColorF color = ColorF::CadetBlue();
	pText->SetColor(Color(color.r, color.g, color.b));
	pImg->SetColor(Color(color.r, color.g, color.b));
	pText->SetOutline(true);

	if (mWidgetContainer.empty())
		return FALSE;

	for (auto& widget : mWidgetContainer)
	{
		widget->Init();
	}

	pText->SetTextFormat(L"¾È³çÇÏ¼¼¿ë");

	return TRUE;
}

void UIManager::Finalization()
{
	if (mWidgetContainer.empty())
		return;

	for (auto& widget : mWidgetContainer)
	{
		widget->Release();
	}
}

void UIManager::Tick()
{

}

void UIManager::Update()
{
	if (mWidgetContainer.empty())
		return;

	for (auto& widget : mWidgetContainer)
	{
		widget->Update();
	}
}

void UIManager::Render()
{
	if (mWidgetContainer.empty())
		return;

	for (auto& widget : mWidgetContainer)
	{
		widget->Render();
	}

	GraphicsManager::GetDepthStencilState(eDepthStencilStateType::DEFAULT)->Bind();
	GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->Bind();
}