#include "pch.h"
#include "D3DGraphicsViewport.h"

D3DGraphicsViewport::D3DGraphicsViewport(FLOAT _offsetX, FLOAT _offsetY
    , FLOAT _width, FLOAT _height, FLOAT _minDepth, FLOAT _maxDepth)
{
    mViewport.TopLeftX = _offsetX;
    mViewport.TopLeftY = _offsetY;
    mViewport.Width = _width;
    mViewport.Height = _height;
    mViewport.MinDepth = _minDepth;
    mViewport.MaxDepth = _maxDepth;
}

void D3DGraphicsViewport::Release()
{
    delete this;
}

HRESULT D3DGraphicsViewport::Create()
{
    return S_OK;
}

HRESULT D3DGraphicsViewport::Bind()
{
    auto pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    if (pDeviceContext)
    {
        pDeviceContext->RSSetViewports(1, &mViewport);
        return S_OK;
    }
    return E_FAIL;
}

HRESULT D3DGraphicsViewport::Reset()
{
    auto pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    if (pDeviceContext)
    {
        // 기본 Viewport를 설정합니다.
        D3D11_VIEWPORT defaultViewport = {};
        defaultViewport.TopLeftX = 0.0f;
        defaultViewport.TopLeftY = 0.0f;
        defaultViewport.Width = 1.0f;  // 최소 크기 (렌더링에는 의미 없는 값)
        defaultViewport.Height = 1.0f;
        defaultViewport.MinDepth = 0.0f;
        defaultViewport.MaxDepth = 1.0f;
        pDeviceContext->RSSetViewports(1, &defaultViewport);
        return S_OK;
    }
    return E_FAIL; // DeviceContext가 nullptr일 경우 실패
}
