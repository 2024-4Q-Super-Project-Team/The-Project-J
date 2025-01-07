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
        // �⺻ Viewport�� �����մϴ�.
        D3D11_VIEWPORT defaultViewport = {};
        defaultViewport.TopLeftX = 0.0f;
        defaultViewport.TopLeftY = 0.0f;
        defaultViewport.Width = 1.0f;  // �ּ� ũ�� (���������� �ǹ� ���� ��)
        defaultViewport.Height = 1.0f;
        defaultViewport.MinDepth = 0.0f;
        defaultViewport.MaxDepth = 1.0f;
        pDeviceContext->RSSetViewports(1, &defaultViewport);
        return S_OK;
    }
    return E_FAIL; // DeviceContext�� nullptr�� ��� ����
}
