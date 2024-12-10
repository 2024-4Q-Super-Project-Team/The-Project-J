#include "pch.h"
#include "GraphicsDevice.h"
#include "GraphicsRenderTarget.h"

namespace Graphics
{
	GraphicsDevice::GraphicsDevice(ID3D11Device* _pDevice, IDXGIFactory* _pFactory, IDXGIAdapter3* _pAdapter)
		: mDevice(nullptr), mFactory(nullptr), mAdapter(nullptr)
	{
		if (!_pDevice)
		{
			std::runtime_error("Failed to create d3d11device.");
		}
		else
		{
			if (_pDevice)
				mDevice	 = _pDevice;
			if (_pFactory)
				mFactory = _pFactory;
			if (_pAdapter)
				mAdapter = _pAdapter;
		}
	}
	GraphicsDevice::~GraphicsDevice()
	{
	}
	void GraphicsDevice::Release()
	{
		if (mDevice)
			mDevice->Release();
		if (mFactory)
			mFactory->Release();
		if (mAdapter)
			mAdapter->Release();
	}
	HRESULT GraphicsDevice::CreateRenderTarget(HWND _hwnd, RenderTarget** _ppRenderTarget)
	{
		if (!_hwnd)
		{
			return E_INVALIDARG;
		}
		RenderTarget* pRenderTarget;
		pRenderTarget = new RenderTarget(this, _hwnd);
		if (pRenderTarget)
		{
			mRenderTargetTable.insert(std::make_pair(_hwnd, pRenderTarget));
			(*_ppRenderTarget) = pRenderTarget;
		}
		return S_OK;
	}
//	inline HRESULT GraphicsDevice::CreateShaderFromFile(const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel, ID3D10Blob** _ppblobOut)
//	{
//		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
//#ifdef _DEBUG
//		dwShaderFlags |= D3DCOMPILE_DEBUG;
//		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
//#endif
//		ID3DBlob* pErrorBlob = nullptr;
//		HRESULT hr = D3DCompileFromFile(_path, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _entryPoint, _shaderModel,
//			dwShaderFlags, 0, _ppblobOut, &pErrorBlob);
//		if (FAILED(hr))
//		{
//			if (pErrorBlob)
//			{
//				MessageBoxA(NULL, static_cast<char*>(pErrorBlob->GetBufferPointer()), "CompileShaderFromFile", MB_OK);
//				pErrorBlob->Release();
//			}
//			return hr;
//		}
//		if (pErrorBlob) pErrorBlob->Release();
//		return S_OK;
//	}
//	inline HRESULT GraphicsDevice::CreateVertexShader(const void* _shaderBytecode, SIZE_T _bytecodeLength, ID3D11ClassLinkage* _classLinkage, ID3D11VertexShader** _vertexShader)
//	{
//		return mDevice->CreateVertexShader(_shaderBytecode, _bytecodeLength, _classLinkage, _vertexShader);
//	}
//	inline HRESULT GraphicsDevice::CreatePixelShader(const void* _shaderBytecode, SIZE_T _bytecodeLength, ID3D11ClassLinkage* _classLinkage, ID3D11PixelShader** _pixelShader)
//	{
//		return mDevice->CreatePixelShader(_shaderBytecode, _bytecodeLength, _classLinkage, _pixelShader);
//	}
//	inline HRESULT GraphicsDevice::CreateSwapChain(DXGI_SWAP_CHAIN_DESC* _pSwapChainDesc, IDXGISwapChain** _ppSwapChain)
//	{
//		return mFactory->CreateSwapChain(mDevice, _pSwapChainDesc, _ppSwapChain);
//	}
//	inline HRESULT GraphicsDevice::CreateRenderTargetView(ID3D11Resource* _pBuffer, const D3D11_RENDER_TARGET_VIEW_DESC* _pRtvDesc, ID3D11RenderTargetView** _ppRenderTargetView)
//	{
//		return mDevice->CreateRenderTargetView(_pBuffer, _pRtvDesc, _ppRenderTargetView);
//	}
//	inline HRESULT GraphicsDevice::CreateDepthStencilView(ID3D11Resource* _pBuffer, const D3D11_DEPTH_STENCIL_VIEW_DESC* _pDsvDesc, ID3D11DepthStencilView** _ppDepthStencilView)
//	{
//		return mDevice->CreateDepthStencilView(_pBuffer, _pDsvDesc, _ppDepthStencilView);
//	}
//	inline HRESULT GraphicsDevice::CreateTexture2D(D3D11_TEXTURE2D_DESC* _pTexDesc, const D3D11_SUBRESOURCE_DATA* _pInitialData, ID3D11Texture2D** _ppTexture2D)
//	{
//		return mDevice->CreateTexture2D(_pTexDesc, _pInitialData, _ppTexture2D);
//	}
}
