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

        HRESULT hr = mFactory->CreateSwapChain(mDevice, &swapDesc, &pSwapChain);
        if (FAILED(hr))
        {
            throw std::runtime_error("HRESULT Failed to CreateSwapChain()");
        }
    }
    D3D11_TEXTURE2D_DESC depthDesc = {};
    { // 뎊스&스텐실 버퍼 설정
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
            depthStencilTexture->Release();	//외부 참조 카운트를 감소시킨다.
    }
    // 백버퍼 생성
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
    // 풀스크린 사각형의 정점 데이터 정의
    BackBufferVertex vertices[] =
    {
        { {-1.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 1.0f} }, // 왼쪽 아래
        { {-1.0f,  1.0f, 0.0f, 1.0f}, {0.0f, 0.0f} }, // 왼쪽 위
        { { 1.0f, -1.0f, 0.0f, 1.0f}, {1.0f, 1.0f} }, // 오른쪽 아래
        { { 1.0f,  1.0f, 0.0f, 1.0f}, {1.0f, 0.0f} }, // 오른쪽 위
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
        FLOAT clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // 검정색으로 클리어
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
    // 현재 클라이언트 크기를 가져옵니다.
    RECT clientRect = {};
    LONG width = 0;
    LONG height = 0;
    if (GetClientRect(mHwnd, &clientRect)) {
        width = clientRect.right - clientRect.left;
        height = clientRect.bottom - clientRect.top;
    }

    // 크기가 0인 경우 리사이즈를 진행하지 않습니다.
    if (width == 0 || height == 0) {
        return;
    }

    // 기존 백 버퍼 및 뎁스 스텐실 뷰 해제
    SAFE_RELEASE(mBackBufferRTV);
    SAFE_RELEASE(mDepthStencilView);

    // 스왑 체인을 리사이즈
    Helper::HRT(mSwapChain->ResizeBuffers(
        0,                // 버퍼 수를 변경하지 않습니다.
        width,            // 새 너비
        height,           // 새 높이
        DXGI_FORMAT_UNKNOWN, // 기존 포맷 유지
        0), "Failed to resize swap chain buffers.");

    // 리사이즈된 백 버퍼를 가져옵니다.
    ID3D11Texture2D* pBackBuffer = nullptr;
    Helper::HRT(mSwapChain->GetBuffer(0,
        __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(&pBackBuffer)),
        "Failed to get buffer from swap chain.");

    // 새 렌더 타겟 뷰 생성
    Helper::HRT(pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mBackBufferRTV),
        "Failed to create render target view for resized swap chain.");
    pBackBuffer->Release();

    // 새 뎁스 스텐실 뷰 생성
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
    // TODO : 렌더타겟의 ShaderResourceView를 백버퍼에 그린다. 어떻게?
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
