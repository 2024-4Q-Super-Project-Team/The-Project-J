#pragma once
#include "IGraphics.h"

class GraphicsDevice;
class GraphicsRenderer;

SET_UTID(_UTID_GRAPHICS_RENDERTARGET, eUTID::GraphicsRenderTarget);

class GraphicsRenderTarget : public IGraphicsRenderTarget
{
public:
	explicit GraphicsRenderTarget(GraphicsDevice* _pDevice, HWND _hWnd);
	virtual ~GraphicsRenderTarget();
	GraphicsRenderTarget(GraphicsRenderTarget&) = delete;
	GraphicsRenderTarget& operator=(const GraphicsRenderTarget&) = delete;
	GraphicsRenderTarget(GraphicsRenderTarget&&) noexcept = default;
	GraphicsRenderTarget& operator=(GraphicsRenderTarget&&) noexcept = default;
public:
    virtual UTID    GetTypeID() override;
    virtual HRESULT QueryInterface(const UTID _UTID, void** _ppObject) override;
	virtual void    Release() override;
public:
    virtual void SetClearColor(FLOAT* _rgba) override;
public:
	FLOAT							mClearColor[4];
	HWND							mHwnd;
	ID3D11RenderTargetView*			mRenderTargetView;
	ID3D11DepthStencilView*			mDepthStencilView;
	D3D11_VIEWPORT					mViewPort;
	IDXGISwapChain*					mSwapChain;
	GraphicsDevice*					mGraphicsDevice;
};

