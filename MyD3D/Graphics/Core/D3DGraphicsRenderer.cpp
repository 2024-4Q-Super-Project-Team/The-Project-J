#include "pch.h"
#include "Graphics/Core/D3DGraphicsRenderer.h"
#include "Graphics/Core/D3DGraphicsDevice.h"
#include "Graphics/Core/D3DGraphicsRenderTarget.h"
#include "Graphics/Core/D3DGraphicsSwapChain.h"
#include "Graphics/GPUResource/D3DGraphicsTexture.h"

D3DGraphicsRenderTarget*    D3DGraphicsRenderer::mCurrRenderTarget = nullptr;
ID3D11DeviceContext*        D3DGraphicsRenderer::mDeviceContext = nullptr;


void D3DGraphicsRenderer::SetRenderTarget(D3DGraphicsRenderTarget* _pRenderTarget)
{
    if (_pRenderTarget)
    {
        // 이미 현재 렌더타겟과 같으면 바꿀 필요가 없다.
        if (mCurrRenderTarget == _pRenderTarget)
            return;
        mCurrRenderTarget = _pRenderTarget;
        mDeviceContext->OMSetRenderTargets(1,
            &mCurrRenderTarget->mRenderTargetView,
            mCurrRenderTarget->mDepthStencilView);
    }
}

BOOL D3DGraphicsRenderer::Initialize()
{
    return TRUE;
}

void D3DGraphicsRenderer::Finalization()
{
    SAFE_RELEASE(mDeviceContext);
}

void D3DGraphicsRenderer::Clear()
{
    if (nullptr == mDeviceContext)
        throw std::runtime_error("NullReference Exeption : D3DGraphicsRenderer::mDeviceContext");
    if (mCurrRenderTarget)
    {
        if (mCurrRenderTarget->mRenderTargetView)
        {
            const FLOAT* clearColor = mCurrRenderTarget->mClearColor;
            mDeviceContext->ClearRenderTargetView(
                mCurrRenderTarget->mRenderTargetView,
                mCurrRenderTarget->mClearColor);
        }
        if (mCurrRenderTarget->mDepthStencilView)
        {
            mDeviceContext->ClearDepthStencilView(
                mCurrRenderTarget->mDepthStencilView,
                D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                1.0f, 0);
        }
    }
}

void D3DGraphicsRenderer::DrawCall(UINT _numIndex, UINT _startIndex, INT _baseVertex)
{
    if(mDeviceContext)
        mDeviceContext->DrawIndexed(_numIndex, _startIndex, _baseVertex);
    else throw std::runtime_error("NullReference Exeption : D3DGraphicsRenderer::mDeviceContext");
}
