#pragma once
#include "IGraphics.h"

class GraphicsDevice;

SET_UTID(_UTID_GRAPHICS_SAMPLERSTATE, eUTID::SamplerState);

class GraphicsSamplerState
    : public ISamplerState
{
public:
    explicit GraphicsSamplerState(const WCHAR* _name, SamplerDesc* _pDesc, GraphicsDevice* _pDevice);
    virtual ~GraphicsSamplerState();
    GraphicsSamplerState(GraphicsSamplerState&) = delete;
    GraphicsSamplerState& operator=(const GraphicsSamplerState&) = delete;
    GraphicsSamplerState(GraphicsSamplerState&&) noexcept = default;
    GraphicsSamplerState& operator=(GraphicsSamplerState&&) noexcept = default;
public:
    virtual UTID    GetTypeID() override;
    virtual HRESULT QueryInterface(const UTID _UTID, void** _ppObject) override;
    virtual void    Release() override;
public:
    virtual const WCHAR* GetName() override;
public:
    virtual HRESULT Bind(IGraphicsRenderer* _renderer) override;
    virtual HRESULT Reset(IGraphicsRenderer* _renderer) override;
public:
    virtual ISamplerState& SetSlot(UINT _startSlot) override;
    virtual ISamplerState& SetShaderStage(eShaderStage _stage) override;
public:
    const WCHAR*    mName;
    UINT            mSlot;
    eShaderStage    mShaderStage;
    ID3D11SamplerState* mSaplerState;
};

