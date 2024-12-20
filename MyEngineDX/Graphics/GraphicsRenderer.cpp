#include "pch.h"
#include "Graphics/GraphicsFramework.h"

namespace Graphics
{
	Renderer::Renderer(GraphicsDevice* _pDevice, ID3D11DeviceContext* _pContext)
		: mGraphicsDevice(_pDevice)
		, mContext(_pContext)
		, mCurRenderTarget(nullptr)
	{
		mPrimitiveBatch = new Batch(mContext);
	}
	Renderer::~Renderer()
	{
	}
	void Renderer::Release()
	{
		if (mContext)
			mContext->Release();
	}
	BOOL Renderer::Clear()
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
	BOOL Renderer::BeginRender()
	{
		if (mCurRenderTarget)
		{
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
			// �̹� ���� ����Ÿ�ٰ� ������ �ٲ� �ʿ䰡 ����.
			if (mCurRenderTarget == _pRenderTarget)
				return TRUE;
			ID3D11RenderTargetView* rtv = _pRenderTarget->GetRenderTargetView();
			ID3D11DepthStencilView* dsv = _pRenderTarget->GetDepthStencilView();
			const D3D11_VIEWPORT& vp = _pRenderTarget->GetViewport();
			mContext->OMSetRenderTargets(1, &rtv, dsv);
			mContext->RSSetViewports(1, &vp);
			mCurRenderTarget = _pRenderTarget;
			return TRUE;
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
	BOOL Renderer::BindConstantBuffer(eShaderStage _stage, UINT _startSlot, Buffer* _constanceBuffer)
	{
		ID3D11Buffer* pBuff = (*_constanceBuffer);
		if (pBuff)
		{
			switch (_stage)
			{
			case eShaderStage::ALL:
				mContext->VSSetConstantBuffers(_startSlot, 1, &pBuff); 
				mContext->PSSetConstantBuffers(_startSlot, 1, &pBuff); 
				break;
			case eShaderStage::VS:
				mContext->VSSetConstantBuffers(_startSlot, 1, &pBuff); break;
			case eShaderStage::PS:
				mContext->PSSetConstantBuffers(_startSlot, 1, &pBuff); break;
			default:
				break;
			}
			return TRUE;
		}
		return FALSE;
	}
	BOOL Renderer::BindVertexBuffer(UINT _startSlot, UINT _numBuffers, VertexBuffer* _pVertexBuffers)
	{
		ID3D11Buffer* pBuff = *_pVertexBuffers;
		UINT Strid = _pVertexBuffers->GetStride();
		UINT Offset = _pVertexBuffers->GetOffset();
		if (pBuff)
		{
			mContext->IASetVertexBuffers(_startSlot, _numBuffers, &pBuff, &Strid, &Offset);
			return TRUE;
		}
		return FALSE;
	}
	BOOL Renderer::BindIndexBuffer(IndexBuffer* _pIndexBuffer)
	{
		ID3D11Buffer* pBuff = *_pIndexBuffer;
		DXGI_FORMAT Format = static_cast<DXGI_FORMAT>(_pIndexBuffer->GetFormat());
		UINT Offset = _pIndexBuffer->GetOffset();
		if (pBuff)
		{
			mContext->IASetIndexBuffer(pBuff, Format, Offset);
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
			case eShaderStage::ALL:
				mContext->VSSetSamplers(_startSlot, 1, &pSampler);
				mContext->PSSetSamplers(_startSlot, 1, &pSampler);
				break;
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
	BOOL Renderer::BindTextrue(eShaderStage _stage, UINT _startSlot, Graphics::Texture* _pTexture)
	{
		ID3D11ShaderResourceView* pSRV = (*_pTexture);
		switch (_stage)
		{
		case eShaderStage::ALL:
			mContext->VSSetShaderResources(_startSlot, 1, &pSRV);
			mContext->PSSetShaderResources(_startSlot, 1, &pSRV); 
			break;
		case eShaderStage::VS:
			mContext->VSSetShaderResources(_startSlot, 1, &pSRV); break;
		case eShaderStage::PS:
			mContext->PSSetShaderResources(_startSlot, 1, &pSRV); break;
		default:
			break;
		}
		return TRUE;
	}
}