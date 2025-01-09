#include "pch.h"
#include "Graphics/Core/D3DGraphicsRenderer.h"
#include "Graphics/Core/D3DGraphicsDevice.h"
#include "Graphics/Core/D3DGraphicsRenderTarget.h"
#include "Graphics/Core/D3DGraphicsSwapChain.h"
#include "Graphics/GPUResource/D3DGraphicsTexture.h"

D3DGraphicsRTV* D3DGraphicsRenderer::mCurrRTV = nullptr;
D3DGraphicsDSV* D3DGraphicsRenderer::mCurrDSV = nullptr;
D3DGraphicsRenderTarget* D3DGraphicsRenderer::mCurrRenderTarget = nullptr;
ID3D11DeviceContext* D3DGraphicsRenderer::mDeviceContext = nullptr;

BOOL D3DGraphicsRenderer::Initialize()
{
    mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    return TRUE;
}

void D3DGraphicsRenderer::Finalization()
{
    SAFE_RELEASE(mDeviceContext);
}

void D3DGraphicsRenderer::SetRenderTarget(IN D3DGraphicsRTV* _pRTV, IN D3DGraphicsDSV* _pDSV)
{
    ID3D11RenderTargetView* RTV = nullptr;
    ID3D11DepthStencilView* DSV = nullptr;
    if (_pRTV)
    {
        RTV = _pRTV->mRTV;
        mCurrRTV = _pRTV;
    }
    if (_pDSV)
    {
        DSV = _pDSV->mDSV;
        mCurrDSV = _pDSV;
    }

    mDeviceContext->OMSetRenderTargets(1, &RTV, DSV);
}

void D3DGraphicsRenderer::SetRenderTarget(IN D3DGraphicsRenderTarget* _pRenderTarget)
{
    if (_pRenderTarget)
    {
        SetRenderTarget(_pRenderTarget->mRenderTargetView, _pRenderTarget->mDepthStencilView);
    }
}

void D3DGraphicsRenderer::Clear()
{
    if (nullptr == mDeviceContext)
        throw std::runtime_error("NullReference Exeption : D3DGraphicsRenderer::mDeviceContext");

    if (mCurrRTV)
    {
        const FLOAT clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
        mDeviceContext->ClearRenderTargetView(mCurrRTV->mRTV, clearColor);
    }
    if (mCurrDSV)
    {
        mDeviceContext->ClearDepthStencilView(mCurrDSV->mDSV,
            D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }
}

void D3DGraphicsRenderer::DrawCall(UINT _numIndex, UINT _startIndex, INT _baseVertex)
{
    if (mDeviceContext)
        mDeviceContext->DrawIndexed(_numIndex, _startIndex, _baseVertex);
    else throw std::runtime_error("NullReference Exeption : D3DGraphicsRenderer::mDeviceContext");
}
