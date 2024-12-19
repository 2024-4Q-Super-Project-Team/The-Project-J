#include "pch.h"
#include "ViewportManager.h"
#include "Viewport/Viewport.h"
#include "GameManager/GameManager.h"
#include "Graphics/GraphicsManager.h"
#include "Graphics/GraphicsFramework.h"

ViewportManager::ViewportManager()
    : mActiveViewport(nullptr)
{
}

ViewportManager::~ViewportManager()
{
}

void ViewportManager::Tick()
{
    for (auto& vp : mViewports)
    {
        mActiveViewport = vp;
        vp->Tick();
    }
}

void ViewportManager::FixedUpdate()
{
    for (auto& vp : mViewports)
    {
        mActiveViewport = vp;
        vp->FixedUpdate();
    }
}

void ViewportManager::PreUpdate()
{
    for (auto& vp : mViewports)
    {
        mActiveViewport = vp;
        vp->FixedUpdate();
    }
}

void ViewportManager::Update()
{
    for (auto& vp : mViewports)
    {
        mActiveViewport = vp;
        vp->Update();
    }
}

void ViewportManager::PostUpdate()
{
    for (auto& vp : mViewports)
    {
        mActiveViewport = vp;
        vp->PostUpdate();
    }
}

void ViewportManager::PreRender()
{
    for (auto& vp : mViewports)
    {
        mActiveViewport = vp;
        vp->PreRender();
    }
}

void ViewportManager::Render(GraphicsManager* _graphicsManager)
{
    for (auto& vp : mViewports)
    {
        mActiveViewport = vp;
        vp->Render(_graphicsManager);
    }
}

void ViewportManager::PostRender()
{
    for (auto& vp : mViewports)
    {
        mActiveViewport = vp;
        vp->PostRender();
    }
}

ViewportScene* ViewportManager::CreateViewport(Display::IWindow* _pWindow)
{
    if (_pWindow)
    {
        Graphics::RenderTarget* pRenderTarget;
        Graphics::GraphicsDevice* pDevice = GameManager::GetGraphicsManager()->GetDevice();
        if (pDevice)
        {
            pDevice->CreateRenderTarget(_pWindow->GetHandle(), &pRenderTarget);
            ViewportScene* instance = new ViewportScene(_pWindow, pRenderTarget);
            mViewports.push_back(instance);
            FLOAT clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
            pRenderTarget->SetClearColor(clearColor);
            return instance;
        }
    }
    return nullptr;
}
