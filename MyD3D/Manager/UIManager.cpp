#include "pch.h"
#include "UIManager.h"

#include "Component/UI/UIWidget/UIWidget.h"

#include "Graphics/GraphicsManager.h"
#include "ViewportScene/ViewportScene.h"
#include "ViewportScene/ViewportManager.h"

std::vector<UIWidget*>	UIManager::mDrawWidgetList{};
SpriteBatch*			UIManager::m_pSpriteBatch = nullptr;
ViewportScene*			UIManager::m_pFocusViewport = nullptr;
Vector2					UIManager::mFocusScreen{};
Vector2					UIManager::mCurrScreen{};
Vector2					UIManager::mScale{1, 1};

BOOL UIManager::Initialize()
{
	m_pSpriteBatch = new SpriteBatch(D3DGraphicsRenderer::GetDevicecontext());

	return TRUE;
}

void UIManager::Finalization()
{
	if (m_pSpriteBatch)
	{
		delete m_pSpriteBatch;
		m_pSpriteBatch = nullptr;
	}
}


void UIManager::Update()
{
	if (m_pFocusViewport)
	{
		if (m_pFocusViewport->GetIWindow())
		{
			Display::IWindow* window = m_pFocusViewport->GetIWindow();
			mCurrScreen = Vector2(window->GetSize().x, window->GetSize().y);

			SetScale();
		}
	}
}

void UIManager::Render()
{
	if (mDrawWidgetList.empty())
		return;

	//m_pSpriteBatch->SetViewport(EditorManager::GetFocusViewport()->GetMainViewport()->mViewport);
	
	ID3D11BlendState* pBlendState = GraphicsManager::GetBlendState(eBlendStateType::ALPHA)->GetBlendState();
	m_pSpriteBatch->Begin(SpriteSortMode_Deferred, pBlendState);

	for (auto& widget : mDrawWidgetList)
	{
		widget->DrawWidget(mScale);
	}

	mDrawWidgetList.clear();

	m_pSpriteBatch->End();

	GraphicsManager::GetRasterizerState(eRasterizerStateType::NONE_CULLING)->Bind();
	GraphicsManager::GetSamplerState(eSamplerStateType::LINEAR_WRAP)->Bind();
	GraphicsManager::GetBlendState(eBlendStateType::DEFAULT)->Bind();
	GraphicsManager::GetDepthStencilState(eDepthStencilStateType::DEFAULT)->Bind();
	GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->Bind();
}

void UIManager::ClearDrawList()
{
	if (!mDrawWidgetList.empty())
	{
		Object* obj = mDrawWidgetList[0]->gameObject->transform->GetParent()->gameObject;
		
		if (obj)
		{
			auto* canvas = obj->GetComponent<Canvas>();

			if (canvas)
			{
				canvas->mWidgets.clear();
			}
		}

	}
}

void UIManager::SetFocusViewport(ViewportScene* _pViewport)
{
	if (_pViewport)
	{
		m_pFocusViewport = _pViewport;
		if (m_pFocusViewport->GetIWindow())
		{
			POINT size = _pViewport->GetIWindow()->GetSize();
			mFocusScreen.x =	(UINT)size.x;
			mFocusScreen.y =	(UINT)size.y;
		}
	}
}