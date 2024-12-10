#include "pch.h"
#include "Viewport.h"


ViewportScene::ViewportScene(Display::IWindow* _pWindow, Graphics::RenderTarget* _pRenderTarget)
    : mWorldManager(new WorldManager())
	, mWindow(_pWindow)
	, mRenderTarget(_pRenderTarget)
{
}

ViewportScene::~ViewportScene()
{
    SAFE_DELETE(mWorldManager)
}

void ViewportScene::FixedUpdate()
{
	if (mWorldManager)
	{
		mWorldManager->FixedUpdate();
	}
}

void ViewportScene::PreUpdate()
{
	if (mWorldManager)
	{
		mWorldManager->PreUpdate();
	}
}

void ViewportScene::Update()
{
	if (mWorldManager)
	{
		mWorldManager->Update();
	}
}

void ViewportScene::PostUpdate()
{
	if (mWorldManager)
	{
		mWorldManager->PostUpdate();
	}
}

void ViewportScene::PreRender()
{
	if (mWorldManager)
	{
		mWorldManager->PreRender();
	}
}

void ViewportScene::Render(GraphicsManager* _graphicsManager)
{
	if (mWorldManager)
	{
		Graphics::Renderer* renderer = _graphicsManager->GetRenderer();
		if (renderer)
		{
			renderer->SetRenderTarget(mRenderTarget);
			renderer->BeginRender();
			mWorldManager->Render(_graphicsManager);
			renderer->EndRender();
		}
	}
}

void ViewportScene::PostRender()
{
	if (mWorldManager)
	{
		mWorldManager->PostRender();
	}
}
