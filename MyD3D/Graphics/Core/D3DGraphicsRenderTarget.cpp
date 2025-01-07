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
    //{
    //    // RTV, SRV ����
    //    D3D11_TEXTURE2D_DESC RTVDesc = {};
    //    RTVDesc.Width = width;
    //    RTVDesc.Height = height;
    //    RTVDesc.MipLevels = 1;
    //    RTVDesc.ArraySize = 1;
    //    RTVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    //    RTVDesc.SampleDesc.Count = 1;
    //    RTVDesc.Usage = D3D11_USAGE_DEFAULT;
    //    RTVDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    //    RTVDesc.CPUAccessFlags = 0;
    //    RTVDesc.MiscFlags = 0;
    //    ID3D11Texture2D* pTexture = nullptr;
    //    Helper::HRT(D3DGraphicsDevice::GetDevice()->CreateTexture2D(&RTVDesc, nullptr, &pTexture), "HRESULT Failed To CreateTexture2D()");
    //    Helper::HRT(D3DGraphicsDevice::GetDevice()->CreateRenderTargetView(pTexture, nullptr, &mRenderTargetView), "HRESULT Failed To CreateRenderTargetView()");
    //    Helper::HRT(D3DGraphicsDevice::GetDevice()->CreateShaderResourceView(pTexture, nullptr, &mShaderResourceView), "HRESULT Failed To CreateShaderResourceView()");
    //    pTexture->Release();
    //}
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
        mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture);
        pDevice->CreateRenderTargetView(backBufferTexture, nullptr, &mRenderTargetView);
        if (backBufferTexture)
            backBufferTexture->Release();	//�ܺ� ���� ī��Ʈ�� ���ҽ�Ų��.
    }
    {
        // Depth Stencil Texture ����
        D3D11_TEXTURE2D_DESC DSVDesc = {};
        DSVDesc.Width = width;
        DSVDesc.Height = height;
        DSVDesc.MipLevels = 1;
        DSVDesc.ArraySize = 1;
        DSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        DSVDesc.SampleDesc.Count = 1;
        DSVDesc.Usage = D3D11_USAGE_DEFAULT;
        DSVDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        DSVDesc.CPUAccessFlags = 0;
        DSVDesc.MiscFlags = 0;
        ID3D11Texture2D* pTexture = nullptr;
        Helper::HRT(D3DGraphicsDevice::GetDevice()->CreateTexture2D(&DSVDesc, nullptr, &pTexture), "HRESULT Failed To CreateTexture2D()");
        Helper::HRT(D3DGraphicsDevice::GetDevice()->CreateDepthStencilView(pTexture, nullptr, &mDepthStencilView), "HRESULT Failed To CreateDepthStencilView()");
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
