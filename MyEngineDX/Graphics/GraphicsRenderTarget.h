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
		void SetClearColor(FLOAT* _rgba);
	private:
		FLOAT mClearColor[4];
		HWND mHwnd;
		ID3D11RenderTargetView* mRenderTargetView;
		ID3D11DepthStencilView* mDepthStencilView;
		D3D11_VIEWPORT          mViewPort;
		IDXGISwapChain*			mSwapChain;
		GraphicsDevice*			mGraphicsDevice;
	public:
		const FLOAT*				GetClearColor() const { return mClearColor; }
		ID3D11RenderTargetView*	GetRenderTargetView() const { return mRenderTargetView; }
		ID3D11DepthStencilView*	GetDepthStencilView() const { return mDepthStencilView; }
		const D3D11_VIEWPORT&	GetViewport() const { return mViewPort; }
		IDXGISwapChain*			GetSwapChain() const { return mSwapChain; }
	};
}
