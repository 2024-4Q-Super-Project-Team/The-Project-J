#include "pch.h"
#include "ViewportScene.h"
#include "Graphics/GraphicsManager.h"
#include "World/WorldManager.h"

ViewportScene::ViewportScene(std::wstring_view _name, Display::IWindow* _pWindow, D3DHwndRenderTarget* _pSwapChain)
    : Entity(_name, L"")
    , mWorldManager(new WorldManager(this))
    , mWindow(_pWindow)
    , mSwapChain(_pSwapChain)
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
        mSwapChain->BeginDraw();

        mSwapChain->Clear();
       
        mWorldManager->Render();

        mSwapChain->EndDraw();
    }
}

void ViewportScene::PostRender()
{
    if (mWorldManager)
    {
        mWorldManager->PostRender();
    }
}