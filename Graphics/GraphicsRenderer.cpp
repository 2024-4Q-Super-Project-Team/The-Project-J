#include "pch.h"
#include "GraphicsRenderer.h"
#include "GraphicsRenderTarget.h"
#include "GraphicsBuffer.h"

GraphicsRenderer::GraphicsRenderer(GraphicsDevice* _pDevice, ID3D11DeviceContext* _pDeviceContext)
    : mGraphicsDevice(_pDevice)
    , mDeviceContext(_pDeviceContext)
    , mCurRenderTarget(nullptr)
{
    mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

GraphicsRenderer::~GraphicsRenderer()
{
}

UTID GraphicsRenderer::GetTypeID()
{
    return _UTID_GRAPHICS_RENDERER;
}

HRESULT GraphicsRenderer::QueryInterface(const UTID _UTID, void** _ppObject)
{
    if (_ppObject == nullptr)
    {
        return E_INVALIDARG;
    }
    *_ppObject = nullptr;
    switch (_UTID)
    {
    case _UTID_GRAPHICS_RENDERER:
        *_ppObject = static_cast<GraphicsRenderer*>(this);
        break;
    case _UTID_GRAPHICS_UNKNOWN:
        *_ppObject = static_cast<IGraphicsUnknown*>(this);
        break;
    default:
        *_ppObject = nullptr;
        return E_NOINTERFACE;
        break;
    }
    return (*_ppObject) ? S_OK : E_NOINTERFACE;
}

void GraphicsRenderer::Release()
{
    if (mDeviceContext)
        mDeviceContext->Release();

    delete this;
}

BOOL GraphicsRenderer::Clear()
{
	if (mCurRenderTarget)
	{
		if (mCurRenderTarget->mRenderTargetView)
		{
			const FLOAT* clearColor = mCurRenderTarget->mClearColor;
			mDeviceContext->ClearRenderTargetView(
                mCurRenderTarget->mRenderTargetView, 
                mCurRenderTarget->mClearColor);
		}
		if (mCurRenderTarget->mDepthStencilView)
		{
			mDeviceContext->ClearDepthStencilView(
                mCurRenderTarget->mDepthStencilView, 
                D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
                1.0f, 0);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL GraphicsRenderer::BeginRender()
{
	if (mCurRenderTarget)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL GraphicsRenderer::EndRender()
{
    if (mCurRenderTarget)
    {
        IDXGISwapChain* swapChain = mCurRenderTarget->mSwapChain;
        swapChain->Present(0, 0);
        return TRUE;
    }
    return FALSE;
}

BOOL GraphicsRenderer::DrawCall(UINT _numIndex, UINT _startIndex, INT _baseVertex)
{
	mDeviceContext->DrawIndexed(_numIndex, _startIndex, _baseVertex);
	return TRUE;
}

BOOL GraphicsRenderer::SetRenderTarget(IGraphicsRenderTarget* _pRenderTarget)
{
    GraphicsRenderTarget* pRenderTarget;
	if (S_OK == _pRenderTarget->QueryInterface(_UTID_GRAPHICS_RENDERTARGET, (void**)&pRenderTarget))
	{
		// 이미 현재 렌더타겟과 같으면 바꿀 필요가 없다.
		if (mCurRenderTarget == pRenderTarget)
			return TRUE;
		mDeviceContext->OMSetRenderTargets(1,
            &pRenderTarget->mRenderTargetView,
            pRenderTarget->mDepthStencilView);
		mDeviceContext->RSSetViewports(1, &pRenderTarget->mViewPort);
		mCurRenderTarget = pRenderTarget;
		return TRUE;
	}
	return TRUE;
}

HRESULT GraphicsRenderer::BindConstantBuffer(eShaderStage _stage, UINT _startSlot, UINT _numBuff, IConstantBuffer* _pCBuff)
{
    if (_pCBuff == nullptr)
    {
        return E_INVALIDARG;
    }
    GraphicsConstantBuffer* pGraphicsCBuffer;
    if (S_OK != _pCBuff->QueryInterface(_UTID_GRAPHICS_CONSTANTBUFFER, (void**)&pGraphicsCBuffer))
    {
        return E_NOINTERFACE;
    }
    ID3D11Buffer* pCBuffer = pGraphicsCBuffer->mBuffer;
    if (pCBuffer == nullptr)
    {
        return E_FAIL;
    }
    switch (_stage)
    {
    case eShaderStage::VS:
        mDeviceContext->VSSetConstantBuffers(_startSlot, _numBuff, &pCBuffer);
        break;
    case eShaderStage::PS:
        mDeviceContext->PSSetConstantBuffers(_startSlot, _numBuff, &pCBuffer);
        break;
    case eShaderStage::GS:
        mDeviceContext->GSSetConstantBuffers(_startSlot, _numBuff, &pCBuffer);
        break;
    case eShaderStage::HS:
        mDeviceContext->HSSetConstantBuffers(_startSlot, _numBuff, &pCBuffer);
        break;
    case eShaderStage::DS:
        mDeviceContext->DSSetConstantBuffers(_startSlot, _numBuff, &pCBuffer);
        break;
    case eShaderStage::CS:
        mDeviceContext->CSSetConstantBuffers(_startSlot, _numBuff, &pCBuffer);
        break;
    case eShaderStage::ALL:
        mDeviceContext->VSSetConstantBuffers(_startSlot, _numBuff, &pCBuffer);
        mDeviceContext->PSSetConstantBuffers(_startSlot, _numBuff, &pCBuffer);
        mDeviceContext->GSSetConstantBuffers(_startSlot, _numBuff, &pCBuffer);
        mDeviceContext->HSSetConstantBuffers(_startSlot, _numBuff, &pCBuffer);
        mDeviceContext->DSSetConstantBuffers(_startSlot, _numBuff, &pCBuffer);
        mDeviceContext->CSSetConstantBuffers(_startSlot, _numBuff, &pCBuffer);
        break;
    default:
        return E_INVALIDARG;
    }
    return S_OK;
}

HRESULT GraphicsRenderer::BindViewport(ViewportDesc* _pViewportDesc)
{
    if (_pViewportDesc == nullptr)
    {
        return E_INVALIDARG;
    }
    D3D11_VIEWPORT vp = {};
    vp.TopLeftX = _pViewportDesc->TopLeftX;
    vp.TopLeftY = _pViewportDesc->TopLeftY;
    vp.Width = _pViewportDesc->Width;
    vp.Height = _pViewportDesc->Height;
    vp.MinDepth = _pViewportDesc->MinDepth;
    vp.MaxDepth = _pViewportDesc->MaxDepth;
    mDeviceContext->RSSetViewports(1, &vp);
    return S_OK;
}

HRESULT GraphicsRenderer::BindBuffer(IBuffer* _pBuffer)
{
    if (_pBuffer == nullptr)
    {
        return E_INVALIDARG;
    }
    return _pBuffer->Bind(this);
}

HRESULT GraphicsRenderer::BindTexture(ITexture* _pTexture)
{
    if (_pTexture == nullptr)
    {
        return E_INVALIDARG;
    }
    return _pTexture->Bind(this);
}

HRESULT GraphicsRenderer::UpdateSubResource(IGraphicsResource* _pSubResource, UINT _dstSubresource, const D3D11_BOX* _dstBox, const void* _srcData, UINT _srcRowPitch, UINT _srcDepthPitch)
{
    if (_pSubResource == nullptr)
    {
        return E_INVALIDARG;
    }
    eUTID TypeID = static_cast<eUTID>(_pSubResource->GetTypeID());
    GraphicsConstantBuffer* pResource;
    switch (TypeID)
    {
    case eUTID::ConstantBuffer:
        if (S_OK == _pSubResource->QueryInterface(_UTID_GRAPHICS_CONSTANTBUFFER, (void**)&pResource))
            mDeviceContext->UpdateSubresource(pResource->mBuffer, _dstSubresource, _dstBox, _srcData, _srcRowPitch, _srcDepthPitch);
        else return E_FAIL;
        break;
    default:
        return E_INVALIDARG;
    }
   
    return S_OK;
}
