#include "pch.h"
#include "D3DGraphicsRenderTarget.h"
#include "Graphics/Core/D3DGraphicsDevice.h"
#include "Graphics/GPUResource/D3DGraphicsTexture.h"

std::vector<D3DGraphicsRenderTarget*> D3DGraphicsRenderTarget::mRenderTargetBindingStack;

D3DGraphicsRenderTarget::D3DGraphicsRenderTarget()
{
}

void D3DGraphicsRenderTarget::BeginDraw()
{
    if (SetTarget())
    {
        // 바인딩 스택에 넣는다.
        D3DGraphicsRenderTarget::mRenderTargetBindingStack.push_back(this);
        isBinding = TRUE;
    }
}

void D3DGraphicsRenderTarget::Clear()
{
    auto pDeviceContext = D3DGraphicsRenderer::GetDevicecontext();
    if (pDeviceContext)
    {
        if (!mRenderTargetViews.empty()) 
        {
            for (auto rtv : mRenderTargetViews)
                pDeviceContext->ClearRenderTargetView(rtv->mRTV, mClearColor);
        }
        if (mDepthStencilView)
        {
            pDeviceContext->ClearDepthStencilView(mDepthStencilView->mDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        }
    }
}

BOOL D3DGraphicsRenderTarget::SetTarget()
{
    auto pDeviceContext = D3DGraphicsRenderer::GetDevicecontext();
    if (pDeviceContext)
    {
        std::vector<ID3D11RenderTargetView*> RTVs;
        RTVs.reserve(mRenderTargetViews.size());
        for (auto rtv : mRenderTargetViews) RTVs.push_back(rtv->mRTV);
        // 벡터가 비어있어도 nullptr이 들어가기 때문에 상관없다.

        pDeviceContext->OMSetRenderTargets(mRenderTargetViews.size(), RTVs.data(),
            mDepthStencilView == nullptr ? nullptr : mDepthStencilView->mDSV);
    
        return TRUE;
    }
    return FALSE;
}

D3DHwndRenderTarget::D3DHwndRenderTarget(HWND _hWnd)
    : mHwnd(_hWnd)
{
    RECT clientRect = {};
    if (GetClientRect(_hWnd, &clientRect)) {
        mWidth  = (UINT)(clientRect.right - clientRect.left);
        mHeight = (UINT)(clientRect.bottom - clientRect.top);
    }

    auto* pFactory = D3DGraphicsDevice::GetFactory();
    auto* pDevice = D3DGraphicsDevice::GetDevice();

    { // 스왑체인 생성
        DXGI_SWAP_CHAIN_DESC swapDesc = {};
        swapDesc.BufferCount = 1;
        swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapDesc.OutputWindow = _hWnd;	// 스왑체인 출력할 창 핸들 값.
        swapDesc.Windowed = true;		// 창 모드 여부 설정.
        swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        // 백버퍼(텍스처)의 가로/세로 크기 설정.
        swapDesc.BufferDesc.Width = mWidth;
        swapDesc.BufferDesc.Height = mHeight;
        // 화면 주사율 설정.
        swapDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapDesc.BufferDesc.RefreshRate.Denominator = 1;
        // 샘플링 관련 설정.
        swapDesc.SampleDesc.Count = 1;
        swapDesc.SampleDesc.Quality = 0;
        // 스왑체인 생성.
        Helper::HRT(pFactory->CreateSwapChain(pDevice, &swapDesc, &mSwapChain));

        { // 백버퍼 RTV 생성
            ID3D11Texture2D* pD3DTex2D = nullptr;
            Helper::HRT(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pD3DTex2D));
            D3DGraphicsTexture2D* pTexture = new D3DGraphicsTexture2D(pD3DTex2D);
            mRenderTargetViews.push_back(new D3DGraphicsRTV(pTexture, nullptr));
            if (pTexture)
                pTexture->Release();
        }
    }

    //D3D11_TEXTURE2D_DESC TexDesc = D3DGraphicsDefault::DefaultTextureDesc;
    //TexDesc.Width = mWidth;
    //TexDesc.Height = mHeight;
    //TexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    //TexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    //D3DGraphicsTexture2D* pTexture = new D3DGraphicsTexture2D(&TexDesc);
    //mDepthStencilView = new D3DGraphicsDSV(pTexture, nullptr);
    //pTexture->Release();
}

D3DHwndRenderTarget::~D3DHwndRenderTarget()
{
    SAFE_RELEASE_ARRAY(mRenderTargetViews);
    SAFE_RELEASE(mSwapChain);
    SAFE_RELEASE(mDepthStencilView);
}

void D3DHwndRenderTarget::Release()
{
    SAFE_RELEASE_ARRAY(mRenderTargetViews);
    SAFE_RELEASE(mSwapChain);
    SAFE_RELEASE(mDepthStencilView);
    delete this;
}

//void D3DHwndRenderTarget::BeginDraw()
//{
//    auto pDeviceContext = D3DGraphicsRenderer::GetDevicecontext();
//    if (pDeviceContext)
//    {
//        pDeviceContext->OMSetRenderTargets(1, &mRenderTargetViews[0]->mRTV, mDepthStencilView->mDSV);
//        D3DGraphicsRenderTarget::mRenderTargetBindingStack.push_back(this);
//        isBinding = TRUE;
//    }
//}

void D3DHwndRenderTarget::EndDraw()
{
    // 바인드 타겟 스택이 비어있으면 에러
    if (D3DGraphicsRenderTarget::mRenderTargetBindingStack.empty())
    {
        throw std::runtime_error("Out of order BeginDraw() & EndDraw()");
    }
    // 맨 뒤의 렌더타겟이 나 자신이어야만 한다.
    if (D3DGraphicsRenderTarget::mRenderTargetBindingStack.back() == this)
    {
        mSwapChain->Present(0, 0);
        D3DGraphicsRenderTarget::mRenderTargetBindingStack.pop_back();
    }
    else // 아니라면 순서가 잘못된거다.
    {
        throw std::runtime_error("Out of order BeginDraw() & EndDraw()");
    }
}

D3DBitmapRenderTarget::D3DBitmapRenderTarget(UINT _width, UINT _height)
{
    mWidth = _width;
    mHeight = _height;
}

D3DBitmapRenderTarget::~D3DBitmapRenderTarget()
{
    SAFE_RELEASE_ARRAY(mRenderTargetViews);
    SAFE_RELEASE_MAP(mShaderResourceViewTable);
    SAFE_RELEASE(mDepthStencilView);
}

void D3DBitmapRenderTarget::Release()
{
    SAFE_RELEASE_ARRAY(mRenderTargetViews);
    SAFE_RELEASE_MAP(mShaderResourceViewTable);
    SAFE_RELEASE(mDepthStencilView);
    delete this;
}

//void D3DBitmapRenderTarget::BeginDraw()
//{
//    auto pDeviceContext = D3DGraphicsRenderer::GetDevicecontext();
//    if (pDeviceContext)
//    {
//        std::vector<ID3D11RenderTargetView*> RTVs;
//        RTVs.reserve(mRenderTargetViews.size());
//        for (auto rtv : mRenderTargetViews) RTVs.push_back(rtv->mRTV);
//        // 벡터가 비어있어도 nullptr이 들어가기 때문에 상관없다.
//
//        pDeviceContext->OMSetRenderTargets(mRenderTargetViews.size(), RTVs.data(), mDepthStencilView == nullptr ? nullptr : mDepthStencilView->mDSV);
//
//        // 바인딩 스택에 넣는다.
//        D3DGraphicsRenderTarget::mRenderTargetBindingStack.push_back(this);
//        isBinding = TRUE;
//    }
//}

void D3DBitmapRenderTarget::EndDraw()
{
    auto pDeviceContext = D3DGraphicsRenderer::GetDevicecontext();
    if (pDeviceContext)
    {
        // 바인드 타겟 스택이 비어있으면 에러
        if (D3DGraphicsRenderTarget::mRenderTargetBindingStack.empty())
        {
            throw std::runtime_error("Out of order BeginDraw() & EndDraw()");
        }
        // 맨 뒤의 렌더타겟이 나 자신이어야만 한다.
        if (D3DGraphicsRenderTarget::mRenderTargetBindingStack.back() == this)
        {
            // 자신을 스택에서 빼고 맨 뒤에 렌더타겟을 다시 바인딩 해준다.
            D3DGraphicsRenderTarget::mRenderTargetBindingStack.pop_back();
            D3DGraphicsRenderTarget* mNextTarget = D3DGraphicsRenderTarget::mRenderTargetBindingStack.back();
            mNextTarget->SetTarget();
            isBinding = FALSE;
        }
        else // 아니라면 순서가 잘못된거다.
        {
            throw std::runtime_error("Out of order BeginDraw() & EndDraw()");
        }
    }
}

void D3DBitmapRenderTarget::BindAllSRV()
{
    for (auto& srv : mShaderResourceViewTable)
    {
        if (srv.second->GetBindStage() != eShaderStage::PASS)
        {
            srv.second->Bind();
        }
    }
}

void D3DBitmapRenderTarget::ResetAllSRV()
{
    for (auto& srv : mShaderResourceViewTable)
    {
        if (srv.second->GetBindStage() != eShaderStage::PASS)
        {
            srv.second->Reset();
        }
    }
}

void D3DBitmapRenderTarget::PushResourceView(D3DGraphicsRTV* _pRTV, D3DGraphicsSRV* _pSRV)
{
    // 바인딩 중이면 허용하지 않음.
    if (isBinding == TRUE)
    {
        return;
    }
    if (_pRTV)
    {
        mRenderTargetViews.push_back(_pRTV);
        if (_pSRV)
        {
            mShaderResourceViewTable[_pRTV] = _pSRV;
        }
    }
}

void D3DBitmapRenderTarget::PushResourceView(D3DGraphicsDSV* _pDSV, D3DGraphicsSRV* _pSRV)
{
    // 바인딩 중이면 허용하지 않음.
    if (isBinding == TRUE)
    {
        return;
    }
    if (_pDSV)
    {
        mDepthStencilView = _pDSV;
        if (_pSRV)
        {
            mShaderResourceViewTable[_pDSV] = _pSRV;
        }
    }
}

void D3DBitmapRenderTarget::PushResourceView(std::pair<D3DGraphicsRTV*, D3DGraphicsSRV*> _pResourceView)
{
    PushResourceView(_pResourceView.first, _pResourceView.second);
}

void D3DBitmapRenderTarget::PushResourceView(std::pair<D3DGraphicsDSV*, D3DGraphicsSRV*> _pResourceView)
{
    PushResourceView(_pResourceView.first, _pResourceView.second);
}

D3DGraphicsSRV* D3DBitmapRenderTarget::GetSRV(D3DResourceView* _pView)
{
    // 바인딩 중이면 허용하지 않음.
    if (isBinding == TRUE)
    {
        return nullptr;
    }
    auto itr = mShaderResourceViewTable.find(_pView);
    if (FIND_SUCCESS(itr, mShaderResourceViewTable))
    {
        return mShaderResourceViewTable[_pView];
    }
    else
    {
        return nullptr;
    }
}