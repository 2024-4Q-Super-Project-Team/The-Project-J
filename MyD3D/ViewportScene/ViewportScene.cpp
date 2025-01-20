#include "pch.h"
#include "ViewportScene.h"
#include "Graphics/GraphicsManager.h"
#include "World/WorldManager.h"
#include "Editor/EditorManager.h"

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

void ViewportScene::Start()
{
    if (mWorldManager)
    {
        mWorldManager->Start();
    }
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

        if (EditorManager::IsRenderView(this))
            EditorManager::RenderEditorWindow();

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

void ViewportScene::EditorUpdate()
{
    if (mWorldManager)
    {
        mWorldManager->EditorUpdate();

        if (EditorManager::IsFocusView(this))
        {
            EditorManager::EditorUpdate();
        }
    }
}

void ViewportScene::EditorRender()
{
    if (mWorldManager)
    {
        mSwapChain->BeginDraw();

        mSwapChain->Clear();

        mWorldManager->EditorRender();

        if (EditorManager::IsFocusView(this))
        {
            EditorManager::EditorRender();
        }

        if (EditorManager::IsRenderView(this))
        {
            EditorManager::RenderEditorWindow();
        }

        mSwapChain->EndDraw();
    }
}

std::shared_ptr<D3DGraphicsViewport> ViewportScene::GetMainViewport()
{
    if (mSharedViewport.expired())
    {
        auto size = mWindow->GetSize();
        UINT width = (UINT)(size.x);
        UINT height = (UINT)(size.y);

        auto pViewport = std::make_shared<D3DGraphicsViewport>(0.0f, 0.0f, width, height);

        mSharedViewport = pViewport;

        return pViewport;
    }
    else
    {
        return mSharedViewport.lock();
    }
}

std::shared_ptr<D3DBitmapRenderTarget> ViewportScene::GetMainRenderTarget()
{
    if (mSharedRenderTarget.expired())
    {
        auto size = mWindow->GetSize();
        UINT width = (UINT)(size.x);
        UINT height = (UINT)(size.y);

        auto pRenderTarget = std::make_shared<D3DBitmapRenderTarget>(width, height);

        pRenderTarget->PushResourceView(GraphicsManager::CreateDefaultRenderTargetView(width, height));
        pRenderTarget->PushResourceView(GraphicsManager::CreateDefaultDepthStencilView(width, height));

        auto pSRV = pRenderTarget->GetSRV(pRenderTarget->GetRTV());
        pSRV->SetBindStage(eShaderStage::PS);
        pSRV->SetBindSlot(11);

        mSharedRenderTarget = pRenderTarget;

        return pRenderTarget;
    }
    else
    {
        return mSharedRenderTarget.lock();
    }
}

std::shared_ptr<D3DBitmapRenderTarget> ViewportScene::GetDeferredRenderTarget()
{
    if (mSharedDeferredRenderTarget.expired())
    {
        auto size = mWindow->GetSize();
        UINT width = (UINT)(size.x);
        UINT height = (UINT)(size.y);

        auto pRenderTarget = std::make_shared<D3DBitmapRenderTarget>(width, height);
        // G-Buffer
        pRenderTarget->PushResourceView(GraphicsManager::CreateAlbedoGBuffer(width, height));
        pRenderTarget->PushResourceView(GraphicsManager::CreateNormalGBuffer(width, height));
        pRenderTarget->PushResourceView(GraphicsManager::CreateMaterialGBuffer(width, height));
        pRenderTarget->PushResourceView(GraphicsManager::CreateEmessiveGBuffer(width, height));
        pRenderTarget->PushResourceView(GraphicsManager::CreateWorldPosGBuffer(width, height));
        pRenderTarget->PushResourceView(GraphicsManager::CreateDefaultDepthStencilView(width, height));

        mSharedDeferredRenderTarget = pRenderTarget;

        return pRenderTarget;
    }
    else
    {
        return mSharedDeferredRenderTarget.lock();
    }
}

void ViewportScene::ResizeSharedResourceView()
{
    auto size = mWindow->GetSize();
    if (size.x != 0 && size.y != 0)
    {
        UINT width = (UINT)(size.x);
        UINT height = (UINT)(size.y);

        mSwapChain->Resize(width, height);

        // weak_ptr에 존재한다면
        if (false == mSharedViewport.expired())
        {
            mSharedViewport.lock()->SetWidth(width);
            mSharedViewport.lock()->SetHeight(height);
        }
        if (false == mSharedRenderTarget.expired())
        {
            mSharedRenderTarget.lock()->Resize(width, height);
        }
        if (false == mSharedDeferredRenderTarget.expired())
        {
            mSharedDeferredRenderTarget.lock()->Resize(width, height);
        }
    }
}
