#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Interface/IGraphics.h"

class D3DGraphicsDevice;

class D3DGraphicsTexture2D 
    : public I3DGraphicsGPUResource
{
public:
    explicit D3DGraphicsTexture2D(D3D11_TEXTURE2D_DESC* _Desc);
    virtual ~D3DGraphicsTexture2D() = default;
public:
    virtual void Release() override;
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
public:
    ID3D11Texture2D* mTex;
    D3D11_TEXTURE2D_DESC* mDesc;
};

class D3DGraphicsRTV 
    : public I3DGraphicsGPUResource
{
public:
    explicit D3DGraphicsRTV(D3DGraphicsTexture2D* _pTex2D, const D3D11_RENDER_TARGET_VIEW_DESC* _pDesc);
    virtual ~D3DGraphicsRTV() = default;
public:
    virtual void Release() override;
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
public:
    // 렌더링할때 그려지는 주체 (쓰기 전용)
    ID3D11RenderTargetView* mRTV = nullptr;
};

class D3DGraphicsDSV 
    : public I3DGraphicsGPUResource
{
public:
    explicit D3DGraphicsDSV(D3DGraphicsTexture2D* _pTex2D, const D3D11_DEPTH_STENCIL_VIEW_DESC* _pDesc);
    virtual ~D3DGraphicsDSV() = default;
public:
    virtual void Release() override;
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
public:
    // 깊이 값을 통한 Z-PASS 수행
    ID3D11DepthStencilView* mDSV = nullptr;
};

class D3DGraphicsSRV 
    : public I3DGraphicsGPUResource
    , public RenderStageBindHandler
    , public RegisterSlotBindHandler
{
public:
    explicit D3DGraphicsSRV(D3DGraphicsTexture2D* _pTex2D, const D3D11_SHADER_RESOURCE_VIEW_DESC* _pDesc);
    virtual ~D3DGraphicsSRV() = default;
public:
    virtual void Release() override;
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
public:
    // 후처리할때 쓸 세이더 리소스 뷰 (읽기 전용)
    ID3D11ShaderResourceView* mSRV = nullptr;
};

// 텍스쳐를 불러오기 위해 만든 놈인데 이름이 마땅히...
class D3DGraphicsImg
    : public I3DGraphicsGPUResource
    , public GraphicsResourceKeyHandler
    , public RenderStageBindHandler
    , public RegisterSlotBindHandler
{
public:
    explicit D3DGraphicsImg(std::wstring_view _path);
    virtual ~D3DGraphicsImg() = default;
public:
    virtual void Release() override;
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
public:
    ID3D11ShaderResourceView* mSRV = nullptr;
};
