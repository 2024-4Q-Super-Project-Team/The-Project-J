#pragma once

namespace Graphics
{
	class Renderer;
	class RenderTarget;

	class GraphicsDevice
	{
	public:
		explicit GraphicsDevice(ID3D11Device* _pDevice, IDXGIFactory* _pFactory, IDXGIAdapter3* _pAdapter);
		virtual ~GraphicsDevice();
		GraphicsDevice(GraphicsDevice&) = delete;
		GraphicsDevice& operator=(const GraphicsDevice&) = delete;
		GraphicsDevice(GraphicsDevice&&) noexcept = default;
		GraphicsDevice& operator=(GraphicsDevice&&) noexcept = default;
	public:
		void Release();
	public:
		HRESULT CreateRenderTarget(HWND _hwnd, RenderTarget** _ppRenderTarget);

		//inline HRESULT CreateShaderFromFile(const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel, ID3D10Blob** _ppblobOut);
		//inline HRESULT CreateVertexShader(const void* _shaderBytecode, SIZE_T _bytecodeLength, ID3D11ClassLinkage* _pclassLinkage, ID3D11VertexShader** _ppVertexShader);;
		//inline HRESULT CreatePixelShader(const void* _shaderBytecode, SIZE_T _bytecodeLength, ID3D11ClassLinkage* _pclassLinkage, ID3D11PixelShader** _ppPixelShader);;
		//inline HRESULT CreateSwapChain(DXGI_SWAP_CHAIN_DESC* _pSwapChainDesc, IDXGISwapChain** _ppSwapChain);
		//inline HRESULT CreateRenderTargetView(ID3D11Resource* _pBuffer, const D3D11_RENDER_TARGET_VIEW_DESC* _pRtvDesc, ID3D11RenderTargetView** _ppRenderTargetView);
		//inline HRESULT CreateDepthStencilView(ID3D11Resource* _pBuffer, const D3D11_DEPTH_STENCIL_VIEW_DESC* _pDsvDesc, ID3D11DepthStencilView** _ppDepthStencilView);
		//inline HRESULT CreateTexture2D(D3D11_TEXTURE2D_DESC* _pTexDesc, const D3D11_SUBRESOURCE_DATA* _pInitialData, ID3D11Texture2D** _ppTexture2D);
	private:
		ID3D11Device* mDevice;
		IDXGIFactory* mFactory;
		IDXGIAdapter3* mAdapter;
		std::unordered_map<HWND, Graphics::RenderTarget*> mRenderTargetTable;

		friend class RenderTarget;
	public:
		inline ID3D11Device* GetDevice() { return mDevice; }
		inline IDXGIFactory* GetFactory() { return mFactory; }
	};
}
