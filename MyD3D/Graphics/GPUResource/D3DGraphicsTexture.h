#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Interface/IGraphics.h"

class D3DGraphicsDevice;

class D3DGraphicsTexture2D
    : public I3DGraphicsGPUResource
{
public:
    explicit D3DGraphicsTexture2D(D3D11_TEXTURE2D_DESC* _Desc);
    explicit D3DGraphicsTexture2D(ID3D11Texture2D* _pTex);
    virtual ~D3DGraphicsTexture2D();
public:
    virtual void Release() override;
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
public:
    HRESULT Resize(UINT _width, UINT _height);
    inline UINT GetWidth() { return mDesc.Width; }
    inline UINT GetHeight() { return mDesc.Height; }
public:
    ID3D11Texture2D* mTex;
    D3D11_TEXTURE2D_DESC mDesc;
};

class D3DResourceView 
    : public I3DGraphicsGPUResource
{
public:
    explicit D3DResourceView(D3DGraphicsTexture2D* _pRefTex);
    virtual ~D3DResourceView();
public:
    HRESULT Resize(UINT _width, UINT _height);
public:
    D3DGraphicsTexture2D* mRefTex = nullptr;
    UINT mWidth;
    UINT mHeight;
};

class D3DGraphicsRTV
    : public D3DResourceView
{
public:
    explicit D3DGraphicsRTV(D3DGraphicsTexture2D* _pTex2D, D3D11_RENDER_TARGET_VIEW_DESC* _pDesc);
    virtual ~D3DGraphicsRTV();
public:
    virtual void Release() override;
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
public:
    // �������Ҷ� �׷����� ��ü (���� ����)
    ID3D11RenderTargetView* mRTV = nullptr;
    D3D11_RENDER_TARGET_VIEW_DESC* mDesc;
};

class D3DGraphicsDSV
    : public D3DResourceView
{
public:
    explicit D3DGraphicsDSV(D3DGraphicsTexture2D* _pTex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* _pDesc);
    virtual ~D3DGraphicsDSV();
public:
    virtual void Release() override;
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
public:
    // ���� ���� ���� Z-PASS ����
    ID3D11DepthStencilView* mDSV = nullptr;
    D3D11_DEPTH_STENCIL_VIEW_DESC* mDesc;
};

class D3DGraphicsSRV
    : public D3DResourceView
    , public RenderStageBindHandler
    , public RegisterSlotBindHandler
{
public:
    explicit D3DGraphicsSRV(D3DGraphicsTexture2D* _pTex2D, D3D11_SHADER_RESOURCE_VIEW_DESC* _pDesc);
    virtual ~D3DGraphicsSRV();
public:
    virtual void Release() override;
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
public:
    // ��ó���Ҷ� �� ���̴� ���ҽ� �� (�б� ����)
    ID3D11ShaderResourceView* mSRV = nullptr;
    D3D11_SHADER_RESOURCE_VIEW_DESC* mDesc;
};

// �ؽ��ĸ� �ҷ����� ���� ���� ���ε� �̸��� ������...
class D3DGraphicsImg
    : public D3DResourceView
    , public GraphicsResourceKeyHandler
    , public RenderStageBindHandler
    , public RegisterSlotBindHandler
{
public:
    explicit D3DGraphicsImg(std::wstring_view _path);
    virtual ~D3DGraphicsImg() ;
public:
    virtual void Release() override;
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
public:
    ID3D11ShaderResourceView* mSRV = nullptr;
};
