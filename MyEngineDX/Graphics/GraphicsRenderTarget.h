#pragma once

namespace Graphics
{
	class GraphicsDevice;

	class RenderTarget
	{
	public:
		explicit RenderTarget(GraphicsDevice* _pDevice, HWND _hWnd);
		virtual ~RenderTarget();
		RenderTarget(RenderTarget&) = delete;
		RenderTarget& operator=(const RenderTarget&) = delete;
		RenderTarget(RenderTarget&&) noexcept = default;
		RenderTarget& operator=(RenderTarget&&) noexcept = default;
	public:
		void Release();
	public:
	private:
		FLOAT mClearColor[4];
		HWND mHwnd;
		ID3D11RenderTargetView* mRenderTargetView;
		ID3D11DepthStencilView* mDepthStencilView;
		D3D11_VIEWPORT          mViewPort;
		IDXGISwapChain*			mSwapChain;
		GraphicsDevice*			mGraphicsDevice;
	public:
		inline const FLOAT*				GetClearColor() const { return mClearColor; }
		inline ID3D11RenderTargetView*	GetRenderTargetView() const { return mRenderTargetView; }
		inline ID3D11DepthStencilView*	GetDepthStencilView() const { return mDepthStencilView; }
		inline const D3D11_VIEWPORT&	GetViewport() const { return mViewPort; }
		inline IDXGISwapChain*			GetSwapChain() const { return mSwapChain; }
	};
}
