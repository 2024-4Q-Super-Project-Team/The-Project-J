#include "pch.h"
#include "GraphicsRenderer.h"
#include "GraphicsRenderTarget.h"
#include "Wrapper/Shader.h"
#include "Wrapper/GraphicsBuffer.h"
#include "Wrapper/InputLayout.h"
#include "Wrapper/SamplerState.h"

namespace Graphics
{
	Renderer::Renderer(GraphicsDevice* _pDevice, ID3D11DeviceContext* _pContext)
		: mGraphicsDevice(_pDevice)
		, mContext(_pContext)
		, mCurRenderTarget(nullptr)
	{
	}
	Renderer::~Renderer()
	{
	}
	void Renderer::Release()
	{
		if (mContext)
			mContext->Release();
	}
	BOOL Renderer::BeginRender()
	{
		if (mCurRenderTarget)
		{
			ID3D11RenderTargetView* renderTargetView = mCurRenderTarget->GetRenderTargetView();
			ID3D11DepthStencilView* depthStencilView = mCurRenderTarget->GetDepthStencilView();
			if (renderTargetView)
			{
				const FLOAT* clearColor = mCurRenderTarget->GetClearColor();
				mContext->ClearRenderTargetView(renderTargetView, clearColor);
			}
			if (depthStencilView)
			{
				mContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			}
			return TRUE;
		}
		return FALSE;
	}
	BOOL Renderer::EndRender()
	{
		IDXGISwapChain* swapChain = mCurRenderTarget->GetSwapChain();
		swapChain->Present(0, 0);
		return TRUE;
	}
	BOOL Renderer::DrawCall(UINT _numIndex, UINT _startIndex, INT _baseVertex)
	{
		mContext->DrawIndexed(_numIndex, _startIndex, _baseVertex);
		return TRUE;
	}
	BOOL Renderer::SetRenderTarget(RenderTarget* _pRenderTarget)
	{
		if (_pRenderTarget)
		{
			// 이미 현재 렌더타겟과 같으면 바꿀 필요가 없다.
			if (mCurRenderTarget == _pRenderTarget)
				return TRUE;
			ID3D11RenderTargetView* rtv = _pRenderTarget->GetRenderTargetView();
			ID3D11DepthStencilView* dsv = _pRenderTarget->GetDepthStencilView();
			const D3D11_VIEWPORT&	vp  = _pRenderTarget->GetViewport();
			if (rtv && dsv)
			{
				mContext->OMSetRenderTargets(1, &rtv, dsv);
				mContext->RSSetViewports(1, &vp);
				mCurRenderTarget = _pRenderTarget;
				return TRUE;
			}
		}
		return TRUE;
	}
	BOOL Renderer::BindShader(eShaderStage _stage, Shader* _pShader)
	{
		if (_pShader)
		{
			switch (_stage)
			{
			case eShaderStage::VS:
			{
				ID3D11VertexShader* pVertexShader = (*dynamic_cast<VertexShader*>(_pShader));
				mContext->VSSetShader(pVertexShader, nullptr, 0);
			}
				break;
			case eShaderStage::PS:
			{
				ID3D11PixelShader* pPixelShader = (*dynamic_cast<PixelShader*>(_pShader));
				mContext->PSSetShader(pPixelShader, nullptr, 0);
			}
				break;
			default:
				break;
			}
			return TRUE;
		}
		return FALSE;
	}
	//BOOL Renderer::SetVertexShader(VertexShader* _pVertexShader)
	//{
	//	ID3D11VertexShader* pVertexShader = (*_pVertexShader);
	//	if (pVertexShader)
	//	{
	//		mContext->VSSetShader(pVertexShader, nullptr, 0);
	//		return TRUE;
	//	}
	//	return FALSE;
	//}
	//BOOL Renderer::SetPixelShader(PixelShader* _pPixelShader)
	//{
	//	ID3D11PixelShader* pPixelShader = (*_pPixelShader);
	//	if (pPixelShader)
	//	{
	//		mContext->PSSetShader(pPixelShader, nullptr, 0);
	//		return TRUE;
	//	}
	//	return TRUE;
	//}
	BOOL Renderer::BindInputLayout(InputLayout* _pInputLayout)
	{
		ID3D11InputLayout* pInputLayout = (*_pInputLayout);
		if (pInputLayout)
		{
			mContext->IASetInputLayout(pInputLayout);
			return TRUE;
		}
		return FALSE;
	}
	BOOL Renderer::BindConstantBuffer(eShaderStage _stage, eCBufferType _startSlot, Buffer* _constanceBuffer)
	{
		ID3D11Buffer* pBuff = (*_constanceBuffer);
		if (pBuff)
		{
			switch (_stage)
			{
			case eShaderStage::VS:
				mContext->VSSetConstantBuffers(static_cast<UINT>(_startSlot), 1, &pBuff); break;
			case eShaderStage::PS:
				mContext->PSSetConstantBuffers(static_cast<UINT>(_startSlot), 1, &pBuff); break;
			default:
				break;
			}
			return TRUE;
		}
		return FALSE;
	}
	BOOL Renderer::BindSampler(eShaderStage _stage, UINT _startSlot, SamplerState* _sampler)
	{
		ID3D11SamplerState* pSampler = (*_sampler);
		if (pSampler)
		{
			switch (_stage)
			{
			case eShaderStage::VS:
				mContext->VSSetSamplers(_startSlot, 1, &pSampler); break;
			case eShaderStage::PS:
				mContext->PSSetSamplers(_startSlot, 1, &pSampler); break;
			default:
				break;
			}
			return TRUE;
		}
		return FALSE;
	}
}