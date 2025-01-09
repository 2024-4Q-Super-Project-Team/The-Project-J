#include "pch.h"
#include "D3DGraphicsRenderTarget.h"
#include "Graphics/Core/D3DGraphicsDevice.h"
#include "Graphics/GPUResource/D3DGraphicsTexture.h"

D3DGraphicsRenderTarget::D3DGraphicsRenderTarget(HWND _hWnd)
    : mSwapChain(nullptr)
    , mRenderTargetView(nullptr)
    , mDepthStencilView(nullptr)
    , mShaderResourceView(nullptr)
{
    ID3D11Device* pDevice = D3DGraphicsDevice::GetDevice();
    IDXGIFactory* pFactory = D3DGraphicsDevice::GetFactory();
    RECT clientRect = {};
    LONG width = 0;
    LONG height = 0;
    if (GetClientRect(_hWnd, &clientRect)) {
        width = clientRect.right - clientRect.left;
        height = clientRect.bottom - clientRect.top;
    }
    DXGI_SWAP_CHAIN_DESC swapDesc = {};
    { // 스왑체인 생성
        swapDesc.BufferCount = 1;
        swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapDesc.OutputWindow = _hWnd;	// 스왑체인 출력할 창 핸들 값.
        swapDesc.Windowed = true;		// 창 모드 여부 설정.
        swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        // 백버퍼(텍스처)의 가로/세로 크기 설정.
        swapDesc.BufferDesc.Width = static_cast<UINT>(width);
        swapDesc.BufferDesc.Height = static_cast<UINT>(height);
        // 화면 주사율 설정.
        swapDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapDesc.BufferDesc.RefreshRate.Denominator = 1;
        // 샘플링 관련 설정.
        swapDesc.SampleDesc.Count = 1;
        swapDesc.SampleDesc.Quality = 0;

        pFactory->CreateSwapChain(pDevice, &swapDesc, &mSwapChain);
    }
    { // 백버퍼 생성
        ID3D11Texture2D* backBufferTexture = nullptr;
        Helper::HRT(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture));
        D3DGraphicsTexture2D* pTexture = new D3DGraphicsTexture2D(backBufferTexture);
        mRenderTargetView = new D3DGraphicsRTV(pTexture, nullptr);
        if (pTexture)
            pTexture->Release();
    }
    {
        // Depth Stencil Texture 생성
        D3D11_TEXTURE2D_DESC texDesc = {};
        texDesc.Width = width;
        texDesc.Height = height;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        texDesc.SampleDesc.Count = 1;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        texDesc.CPUAccessFlags = 0;
        texDesc.MiscFlags = 0;
        D3DGraphicsTexture2D* pTexture = new D3DGraphicsTexture2D(&texDesc);
        mDepthStencilView = new D3DGraphicsDSV(pTexture, nullptr);
        if (pTexture)
            pTexture->Release();
    }
}

void D3DGraphicsRenderTarget::Release()
{
    SAFE_RELEASE(mSwapChain);
    SAFE_RELEASE(mRenderTargetView);
    SAFE_RELEASE(mDepthStencilView);
    SAFE_RELEASE(mShaderResourceView);
}

void D3DGraphicsRenderTarget::Resize(UINT _width, UINT _height)
{
    // 크기가 0인 경우 재조정을 건너뜁니다.
    if (_width == 0 || _height == 0) return;

    //// 새로운 RTV 생성
    //Helper::HRT(mRenderTargetView->ReSize(_width, _height));
    //// 새로운 DSV 생성
    //Helper::HRT(mDepthStencilView->ReSize(_width, _height));
    //// 새로운 SRV 생성
    //Helper::HRT(mShaderResourceView->ReSize(_width, _height));

    // 내부 크기 업데이트
    //mWidth = _width;
    //mHeight = _height;

}
