#include "pch.h"
#include "UIManager.h"

#include "Widget/Widget.h"

#include "Graphics/GraphicsManager.h"
#include "ViewportScene/ViewportScene.h"
#include "ViewportScene/ViewportManager.h"

std::vector<Widget*>	UIManager::mDrawWidgetList{};
SpriteBatch*			UIManager::pSpriteBatch = nullptr;
ViewportScene*			UIManager::mFocusViewport = nullptr;
Vector2					UIManager::mFocusScreen{};
Vector2					UIManager::mCurrScreen{};
Vector2					UIManager::mScale{1, 1};

BOOL UIManager::Initialize()
{
	pSpriteBatch = new SpriteBatch(D3DGraphicsRenderer::GetDevicecontext());

	return TRUE;
}

void UIManager::Finalization()
{
	if (pSpriteBatch)
	{
		delete pSpriteBatch;
		pSpriteBatch = nullptr;
	}
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
		}
	}
}

void UIManager::Render()
{
	if (mDrawWidgetList.empty())
		return;

	pSpriteBatch->SetViewport(EditorManager::GetFocusViewport()->GetMainViewport()->mViewport);
	pSpriteBatch->Begin();

	for (auto& widget : mDrawWidgetList)
	{
		widget->Draw(mScale);
	}

	mDrawWidgetList.clear();

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
