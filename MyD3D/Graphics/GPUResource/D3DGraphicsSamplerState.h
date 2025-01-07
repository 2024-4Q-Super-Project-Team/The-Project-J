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

