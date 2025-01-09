#include "pch.h"
#include "D3DGraphicsSwapChain.h"
#include "Graphics/Core/D3DGraphicsDevice.h"
#include "Graphics/GPUResource/D3DGraphicsTexture.h"

#include "Graphics/GraphicsManager.h"
HRESULT D3DGraphicsDevice::CreateD3DSwapChain(HWND _hWnd, D3DGraphicsSwapChain** _ppSwapChain)
{
    RECT clientRect = {};
    LONG width = 0;
    LONG height = 0;
    if (GetClientRect(_hWnd, &clientRect)) {
        width = clientRect.right - clientRect.left;
        height = clientRect.bottom - clientRect.top;
    }

    IDXGISwapChain* pSwapChain;
    ID3D11RenderTargetView* pSwapChainRTV = nullptr;
    ID3D11DepthStencilView* pSwapChainDSV = nullptr;
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

        HRESULT hr = mFactory->CreateSwapChain(mDevice, &swapDesc, &pSwapChain);
        if (FAILED(hr))
        {
            throw std::runtime_error("HRESULT Failed to CreateSwapChain()");
        }
    }
    D3D11_TEXTURE2D_DESC depthDesc = {};
    { // �X��&���ٽ� ���� ����
        depthDesc.Width = static_cast<UINT>(width);
        depthDesc.Height = static_cast<UINT>(height);
        depthDesc.MipLevels = 1;
        depthDesc.ArraySize = 1;
        depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthDesc.SampleDesc.Count = 1;
        depthDesc.SampleDesc.Quality = 0;
        depthDesc.Usage = D3D11_USAGE_DEFAULT;
        depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthDesc.CPUAccessFlags = 0;
        depthDesc.MiscFlags = 0;

        ID3D11Texture2D* depthStencilTexture = nullptr;
        mDevice->CreateTexture2D(&depthDesc, nullptr, &depthStencilTexture);
        mDevice->CreateDepthStencilView(depthStencilTexture, nullptr, &pSwapChainDSV);
        if (depthStencilTexture)
            depthStencilTexture->Release();	//�ܺ� ���� ī��Ʈ�� ���ҽ�Ų��.
    }
    // ����� ����
    ID3D11Texture2D* pBackBuffer = nullptr;
    pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
    mDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pSwapChainRTV);
    if (pBackBuffer)
        pBackBuffer->Release();

    D3DGraphicsSwapChain* pD3DSwapChain
        = new D3DGraphicsSwapChain(_hWnd, pSwapChain, pSwapChainRTV, pSwapChainDSV);

    (*_ppSwapChain) = pD3DSwapChain;

    return S_OK;
}

D3DGraphicsSwapChain::D3DGraphicsSwapChain(HWND _hWnd, IDXGISwapChain* _pSwapChain, ID3D11RenderTargetView* _pBackBufferRTV, ID3D11DepthStencilView* _pDepthStencilView)
    : mHwnd(_hWnd), mSwapChain(_pSwapChain), mBackBufferRTV(_pBackBufferRTV), mDepthStencilView(_pDepthStencilView)
{
    // Ǯ��ũ�� �簢���� ���� ������ ����
    BackBufferVertex vertices[] =
    {
        { {-1.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 1.0f} }, // ���� �Ʒ�
        { {-1.0f,  1.0f, 0.0f, 1.0f}, {0.0f, 0.0f} }, // ���� ��
        { { 1.0f, -1.0f, 0.0f, 1.0f}, {1.0f, 1.0f} }, // ������ �Ʒ�
        { { 1.0f,  1.0f, 0.0f, 1.0f}, {1.0f, 0.0f} }, // ������ ��
    };
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(vertices);
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = vertices;
    mVertexBuffer = new D3DGraphicsVertexBuffer(&vertexBufferDesc, &vertexData);

    unsigned int indices[] = { 0, 1, 2, 2, 1, 3 };
    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(indices);
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indices;
    mIndexBuffer = new D3DGraphicsIndexBuffer(&vertexBufferDesc, &vertexData);
}

void D3DGraphicsSwapChain::Release()
{
    SAFE_RELEASE(mSwapChain);
    SAFE_RELEASE(mBackBufferRTV);
    SAFE_RELEASE(mDepthStencilView);
    SAFE_RELEASE(mVertexBuffer);
    SAFE_RELEASE(mIndexBuffer);
    delete this;
}

void D3DGraphicsSwapChain::BeginDraw()
{
    auto pDeviceContext = D3DGraphicsRenderer::GetDevicecontext();
    if (pDeviceContext) {
        FLOAT clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // ���������� Ŭ����
        if (mBackBufferRTV) {
            pDeviceContext->ClearRenderTargetView(mBackBufferRTV, clearColor);
        }
        if (mDepthStencilView) {
            pDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        }
        pDeviceContext->OMSetRenderTargets(1, &mBackBufferRTV, mDepthStencilView);
    }
}

void D3DGraphicsSwapChain::ReSize()
{
    auto* pDevice = D3DGraphicsDevice::GetDevice();
    // ���� Ŭ���̾�Ʈ ũ�⸦ �����ɴϴ�.
    RECT clientRect = {};
    LONG width = 0;
    LONG height = 0;
    if (GetClientRect(mHwnd, &clientRect)) {
        width = clientRect.right - clientRect.left;
        height = clientRect.bottom - clientRect.top;
    }

    // ũ�Ⱑ 0�� ��� ������� �������� �ʽ��ϴ�.
    if (width == 0 || height == 0) {
        return;
    }

    // ���� �� ���� �� ���� ���ٽ� �� ����
    SAFE_RELEASE(mBackBufferRTV);
    SAFE_RELEASE(mDepthStencilView);

    // ���� ü���� ��������
    Helper::HRT(mSwapChain->ResizeBuffers(
        0,                // ���� ���� �������� �ʽ��ϴ�.
        width,            // �� �ʺ�
        height,           // �� ����
        DXGI_FORMAT_UNKNOWN, // ���� ���� ����
        0), "Failed to resize swap chain buffers.");

    // ��������� �� ���۸� �����ɴϴ�.
    ID3D11Texture2D* pBackBuffer = nullptr;
    Helper::HRT(mSwapChain->GetBuffer(0,
        __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(&pBackBuffer)),
        "Failed to get buffer from swap chain.");

    // �� ���� Ÿ�� �� ����
    Helper::HRT(pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mBackBufferRTV),
        "Failed to create render target view for resized swap chain.");
    pBackBuffer->Release();

    // �� ���� ���ٽ� �� ����
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 24-bit depth, 8-bit stencil
    depthDesc.SampleDesc.Count = 1;
    depthDesc.SampleDesc.Quality = 0;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthDesc.CPUAccessFlags = 0;
    depthDesc.MiscFlags = 0;

    ID3D11Texture2D* pDepthStencilBuffer = nullptr;
    Helper::HRT(pDevice->CreateTexture2D(&depthDesc, nullptr, &pDepthStencilBuffer));
    Helper::HRT(pDevice->CreateDepthStencilView(pDepthStencilBuffer, nullptr, &mDepthStencilView),
        "Failed to create depth stencil buffer for resized swap chain.");
    pDepthStencilBuffer->Release();
}
void D3DGraphicsSwapChain::Draw(D3DGraphicsRenderTarget* _pRenderTarget)
{
    // TODO : ����Ÿ���� ShaderResourceView�� ����ۿ� �׸���. ���?
    //auto pDeviceContext = D3DGraphicsRenderer::GetDevicecontext();
    //if (pDeviceContext) 
    //{
    //    pDeviceContext->OMSetRenderTargets(1, &mBackBufferRTV, mDepthStencilView);
    //
    //    GraphicsManager::GetVertexShader(eVertexShaderType::SPRITE)->Bind();
    //    GraphicsManager::GetPixelShader(ePixelShaderType::SPRITE)->Bind();
    //
    //    mVertexBuffer->Bind();
    //    mIndexBuffer->Bind();
    //
    //    pDeviceContext->PSSetShaderResources(0, 1, &_pRenderTarget->mShaderResourceView);
    //
    //    D3DGraphicsRenderer::DrawCall(6, 0, 0);
    //}
}

void D3DGraphicsSwapChain::EndDraw()
{
    mSwapChain->Present(0, 0);
}
