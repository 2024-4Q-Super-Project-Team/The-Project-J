#include "pch.h"
#include "ViewportScene.h"
#include "Graphics/GraphicsManager.h"
#include "World/WorldManager.h"

ViewportScene::ViewportScene(std::wstring_view _name, Display::IWindow* _pWindow, D3DGraphicsRenderTarget* _pRenderTarget)
    : Entity(_name, L"")
    , mWorldManager(new WorldManager(this))
    , mWindow(_pWindow)
    , mRenderTarget(_pRenderTarget)
{
}

ViewportScene::~ViewportScene()
{
    SAFE_DELETE(mWorldManager)
}

void ViewportScene::Tick()
{
    if (mWorldManager)
    {
        mWorldManager->Tick();
    }
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

void ViewportScene::Render()
{
    if (mWorldManager)
    {
        D3DGraphicsRenderer::SetRenderTarget(mRenderTarget);
        D3DGraphicsRenderer::Clear();
        mWorldManager->Render();
        mRenderTarget->mSwapChain->Present(0, 0);
    }
}

void ViewportScene::PostRender()
{
    if (mWorldManager)
    {
        mWorldManager->PostRender();
    }
}