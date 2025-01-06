#pragma once
#include "IGraphics.h"

class GraphicsRenderTarget;
class GraphicsDevice;

SET_UTID(_UTID_GRAPHICS_RENDERER, eUTID::GraphicsRenderer);

class GraphicsRenderer
	: public IGraphicsRenderer
{
public:
	GraphicsRenderer(GraphicsDevice* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~GraphicsRenderer();
	GraphicsRenderer(GraphicsRenderer&) = delete;
	GraphicsRenderer& operator=(const GraphicsRenderer&) = delete;
	GraphicsRenderer(GraphicsRenderer&&) noexcept = default;
	GraphicsRenderer& operator=(GraphicsRenderer&&) noexcept = default;
public:
    virtual UTID    GetTypeID() override;
    virtual HRESULT QueryInterface(const UTID _UTID, void** _ppObject) override;
	virtual void    Release() override;
public:
	virtual BOOL Clear() override;
	virtual BOOL BeginRender() override;
	virtual BOOL EndRender() override;
	virtual BOOL DrawCall(UINT _numIndex, UINT _startIndex, INT _baseVertex) override;
	virtual BOOL SetRenderTarget(IGraphicsRenderTarget* _pRenderTarget) override;

    // ¾È¾¸
    virtual HRESULT BindConstantBuffer(eShaderStage _stage, UINT _startSlot, UINT _numBuff, IConstantBuffer* _pCBuff) override;
    
    virtual HRESULT BindViewport(ViewportDesc* _pViewportDesc) override;
    virtual HRESULT BindBuffer(IBuffer* _pBuffer) override;
    virtual HRESULT BindTexture(ITexture* _pTexture) override;
    virtual HRESULT UpdateSubResource(IGraphicsResource* _pSubResource, UINT _dstSubresource, const D3D11_BOX* _dstBox, const void* _srcData, UINT _srcRowPitch, UINT _srcDepthPitch) override;
public:
	GraphicsRenderTarget*	mCurRenderTarget;
	ID3D11DeviceContext*	mDeviceContext;
	GraphicsDevice*			mGraphicsDevice;
};

