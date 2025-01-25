#include "pch.h"
#include "UIManager.h"

#include "Widget/Widget.h"

#include "Graphics/GraphicsManager.h"
#include "ViewportScene/ViewportScene.h"
#include "ViewportScene/ViewportManager.h"

std::vector<Widget*>	UIManager::mWidgetContainer = {};
SpriteBatch*			UIManager::pSpriteBatch = nullptr;
ViewportScene*			UIManager::mFocusViewport = nullptr;
Vector2					UIManager::mFocusScreen{};
Vector2					UIManager::mCurrScreen{};
Vector2					UIManager::mScale{1, 1};


BOOL UIManager::Initialize()
{
	pSpriteBatch = new SpriteBatch(D3DGraphicsRenderer::GetDevicecontext());

	//auto pImg = AddWidget<WidgetImage>(L"UI_01", L"resource/UI_turorial_03.png");
	//pImg->SetPosition(Vector2(10, 10));

	auto pImg = AddWidget<WidgetImage>(L"Cat_02", L"resource/texture/cat.dds");
	pImg->SetPosition(Vector2(50, 50));

	auto pText = AddWidget<WidgetText>(L"Text_01", L"resource/myfile.sfont");
	pText->SetPosition(Vector2(500, 200));
	pText->SetTextLine(14);
	ColorF color = ColorF::LavenderPurple();
	pText->SetColor(Color(color.r, color.g, color.b));
	pText->SetOutline(true);
	pText->SetOutlineColor(Color(1, 1, 1));


	if (mWidgetContainer.empty())
		return FALSE;

	for (auto& widget : mWidgetContainer)
	{
		widget->Init();
	}

	pText->SetTextFormat(L"나는 고양이다옹");

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
	if (mFocusViewport)
	{
		if (mFocusViewport->GetIWindow())
		{
			Display::IWindow* window = mFocusViewport->GetIWindow();
			mCurrScreen = Vector2(window->GetSize().x, window->GetSize().y);

			SetScale();

			if (mWidgetContainer.empty())
				return;

			for (auto& widget : mWidgetContainer)
			{
				if (widget->IsActive())
				{
					widget->PreUpdate();
					widget->Update();
				}
			}
		}
	}
}

void UIManager::PostUpate()
{

}

void UIManager::Render()
{
	if (mWidgetContainer.empty())
		return;

	pSpriteBatch->SetViewport(EditorManager::GetFocusViewport()->GetMainViewport()->mViewport);
	pSpriteBatch->Begin();

	for (auto& widget : mWidgetContainer)
	{
		if (widget->IsActive())
		{
			widget->Render(mScale);
		}
	}

	pSpriteBatch->End();

	GraphicsManager::GetDepthStencilState(eDepthStencilStateType::DEFAULT)->Bind();
	GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->Bind();
}

void UIManager::SetFocusViewport(ViewportScene* _pViewport)
{
	if (_pViewport)
	{
		mFocusViewport = _pViewport;
		if (mFocusViewport->GetIWindow())
		{
			POINT size = _pViewport->GetIWindow()->GetSize();
			mFocusScreen.x =	(UINT)size.x;
			mFocusScreen.y =	(UINT)size.y;
		}
	}
}
