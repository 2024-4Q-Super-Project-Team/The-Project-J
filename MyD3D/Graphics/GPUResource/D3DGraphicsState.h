#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Interface/IGraphics.h"

class D3DGraphicsSamplerState
    : public I3DGraphicsGPUResource
    , public RenderStageBindHandler
    , public RegisterSlotBindHandler
{
public:
    explicit D3DGraphicsSamplerState(IN D3D11_SAMPLER_DESC* _pSamplerDesc);
    virtual ~D3DGraphicsSamplerState() = default;
public:
    virtual void Release() override;
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
private:
    ID3D11SamplerState* mSamplerState;
};

class D3DGraphicsBlendState
    : public I3DGraphicsGPUResource
{
public:
    explicit D3DGraphicsBlendState(IN D3D11_BLEND_DESC* _pBlendDesc);
    virtual ~D3DGraphicsBlendState() = default;
public:
    virtual void    Release() override;
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
private:
    ID3D11BlendState* mBlendState;
};

class D3DGraphicsDepthStencilState
    : public I3DGraphicsGPUResource
{
public:
    explicit D3DGraphicsDepthStencilState(IN D3D11_DEPTH_STENCIL_DESC* _pDSDesc);
    virtual ~D3DGraphicsDepthStencilState() = default;
public:
    virtual void Release() override;
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
private:
    ID3D11DepthStencilState* mDSState;
};

class D3DGraphicsRasterizerState
    : public I3DGraphicsGPUResource
{
public:
    explicit D3DGraphicsRasterizerState(IN D3D11_RASTERIZER_DESC* _pRRDesc);
    virtual ~D3DGraphicsRasterizerState() = default;
public:
    virtual void Release() override;
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
private:
    ID3D11RasterizerState* mRRState;
};