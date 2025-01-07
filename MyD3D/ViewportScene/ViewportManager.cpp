#include "pch.h"
#include "ViewportManager.h"
#include "Graphics/GraphicsManager.h"
#include "Manager/GameManager.h"
#include "ViewportScene/ViewportScene.h"
#include "Application/Application.h"

ViewportManager::ViewportManager()
    : mActiveViewport(nullptr)
{
    HRESULT hr = Display::CreateIDisplayDevice(
        GameManager::GetApplication()->GetHInstance()
        , &mDisplayDevice);
    if (FAILED(hr))
    {
        throw std::runtime_error("Hresult Failed to CreateIDisplayDevice");
    }
}

ViewportManager::~ViewportManager()
{
}

BOOL ViewportManager::Initialize()
{
    return TRUE;
}

void ViewportManager::Finalization()
{
    SAFE_DELETE_ARRAY(mViewportScenes);
    mDisplayDevice->Release();
}

void ViewportManager::Tick()
{
    mActiveViewport = nullptr;
    for (auto& vp : mViewportScenes)
    {
        mActiveViewport = vp;
        vp->Tick();
    }
    mActiveViewport = nullptr;
}

void ViewportManager::FixedUpdate()
{
    mActiveViewport = nullptr;
    for (auto& vp : mViewportScenes)
    {
        mActiveViewport = vp;
        vp->FixedUpdate();
    }
    mActiveViewport = nullptr;
}

void ViewportManager::PreUpdate()
{
    mActiveViewport = nullptr;
    for (auto& vp : mViewportScenes)
    {
        mActiveViewport = vp;
        vp->FixedUpdate();
    }
    mActiveViewport = nullptr;
}

void ViewportManager::Update()
{
    mActiveViewport = nullptr;
    for (auto& vp : mViewportScenes)
    {
        mActiveViewport = vp;
        vp->Update();
    }
    mActiveViewport = nullptr;
}

void ViewportManager::PostUpdate()
{
    mActiveViewport = nullptr;
    for (auto& vp : mViewportScenes)
    {
        mActiveViewport = vp;
        vp->PostUpdate();
    }
    mActiveViewport = nullptr;
}

void ViewportManager::PreRender()
{
    mActiveViewport = nullptr;
    for (auto& vp : mViewportScenes)
    {
        mActiveViewport = vp;
        vp->PreRender();
    }
    mActiveViewport = nullptr;
}

void ViewportManager::Render()
{
    mActiveViewport = nullptr;
    for (auto& vp : mViewportScenes)
    {
        mActiveViewport = vp;
        vp->Render();
    }
    mActiveViewport = nullptr;
}

void ViewportManager::PostRender()
{
    mActiveViewport = nullptr;
    for (auto& vp : mViewportScenes)
    {
        mActiveViewport = vp;
        vp->PostRender();
    }
    mActiveViewport = nullptr;
}

ViewportScene* ViewportManager::CreateViewportScene(Display::WindowDesc* _pWinDesc)
{
    Display::IWindow* pWindow = nullptr;
    D3DGraphicsRenderTarget* pRenderTarget = nullptr;
    if (FAILED(mDisplayDevice->CreateWindowDisplay(_pWinDesc, &pWindow)))
    {
        throw std::runtime_error("Hresult Failed to ViewportScene::CreateViewport...CreateWindowDisplay()");
    }
    pRenderTarget = new D3DGraphicsRenderTarget(pWindow->GetHandle());
    if (pWindow && pRenderTarget)
    {
        const WCHAR* winName = _pWinDesc->WndClass.lpszClassName;
        ViewportScene* pViewport = new ViewportScene(winName, pWindow, pRenderTarget);
        mViewportScenes.push_back(pViewport);
        return pViewport;
    }
    return nullptr;
}

ViewportScene* ViewportManager::GetViewportSceneFromWindowName(std::wstring _winName)
{
    auto itr = FIND_CONTAINER(mViewportScenes,
        [&_winName](const ViewportScene* _pViewport)
        {
            return (_pViewport->GetName() == _winName);
        });
    if (FIND_SUCCESS(itr, mViewportScenes))
    {
        return (*itr);
    }
    else
    {
        return nullptr;
    }
}

ViewportScene* ViewportManager::GetViewportSceneFromHwnd(HWND _hWnd)
{
    auto itr = FIND_CONTAINER(mViewportScenes,
        [&_hWnd](ViewportScene* _pViewport)
        {
            return (_pViewport->GetIWindow()->GetHandle() == _hWnd);
        });
    if (FIND_SUCCESS(itr, mViewportScenes))
    {
        return (*itr);
    }
    else
    {
        return nullptr;
    }
}
