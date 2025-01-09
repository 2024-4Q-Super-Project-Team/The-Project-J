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
    { // ����ü�� ����
        swapDesc.BufferCount = 1;
        swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapDesc.OutputWindow = _hWnd;	// ����ü�� ����� â �ڵ� ��.
        swapDesc.Windowed = true;		// â ��� ���� ����.
        swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        // �����(�ؽ�ó)�� ����/���� ũ�� ����.
        swapDesc.BufferDesc.Width = static_cast<UINT>(width);
        swapDesc.BufferDesc.Height = static_cast<UINT>(height);
        // ȭ�� �ֻ��� ����.
        swapDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapDesc.BufferDesc.RefreshRate.Denominator = 1;
        // ���ø� ���� ����.
        swapDesc.SampleDesc.Count = 1;
        swapDesc.SampleDesc.Quality = 0;

        pFactory->CreateSwapChain(pDevice, &swapDesc, &mSwapChain);
    }
    { // ����� ����
        ID3D11Texture2D* backBufferTexture = nullptr;
        Helper::HRT(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture));
        D3DGraphicsTexture2D* pTexture = new D3DGraphicsTexture2D(backBufferTexture);
        mRenderTargetView = new D3DGraphicsRTV(pTexture, nullptr);
        if (pTexture)
            pTexture->Release();
    }
    {
        // Depth Stencil Texture ����
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
    // ũ�Ⱑ 0�� ��� �������� �ǳʶݴϴ�.
    if (_width == 0 || _height == 0) return;

    //// ���ο� RTV ����
    //Helper::HRT(mRenderTargetView->ReSize(_width, _height));
    //// ���ο� DSV ����
    //Helper::HRT(mDepthStencilView->ReSize(_width, _height));
    //// ���ο� SRV ����
    //Helper::HRT(mShaderResourceView->ReSize(_width, _height));

    // ���� ũ�� ������Ʈ
    //mWidth = _width;
    //mHeight = _height;

}
