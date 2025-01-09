#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Interface/IGraphics.h"

class D3DGraphicsRTV;
class D3DGraphicsDSV;
class D3DGraphicsSRV;

// �������� �� ��ü�� ���� ���� Ŭ����
class D3DGraphicsRenderTarget : public I3DGraphicsCore
{
public:
    explicit D3DGraphicsRenderTarget(HWND _hWnd);
    virtual ~D3DGraphicsRenderTarget() = default;
public:
    virtual void Release() override;
public:
    void Resize(UINT _width, UINT _height);
    //inline void SetClearColor(FLOAT* _color) { mClearColor = _color; }
public:
    // ��ũ�� Ŭ����� ����
    FLOAT                       mClearColor[3] = { 1.0f,1.0f,1.0f };
    // 
    IDXGISwapChain* mSwapChain;
    // �������Ҷ� �׷����� ��ü (���� ����)
    D3DGraphicsRTV* mRenderTargetView;
    // ���� ���� ���� Z-PASS ����
    D3DGraphicsDSV* mDepthStencilView;
    // ��ó���Ҷ� �� ���̴� ���ҽ� �� (�б� ����)
    D3DGraphicsSRV* mShaderResourceView;
};

//class D3DGraphicsRenderTarget : public I3DGraphicsCore
//{
//public:
//    explicit D3DGraphicsRenderTarget(UINT _width, UINT _height);
//    virtual ~D3DGraphicsRenderTarget() = default;
//public:
//    virtual void Release() override;
//public:
//    void Resize(UINT _width, UINT _height);
//    //inline void SetClearColor(FLOAT* _color) { mClearColor = _color; }
//public:
//    UINT mWidth;
//    UINT mHeight;
//    // ��ũ�� Ŭ����� ����
//    FLOAT               mClearColor[3] = { 1.0f,1.0f,1.0f };
//    // �������Ҷ� �׷����� ��ü (���� ����)
//    ID3D11RenderTargetView* mRenderTargetView;
//    // ���� ���� ���� Z-PASS ����
//    ID3D11DepthStencilView* mDepthStencilView;
//    // ��ó���Ҷ� �� ���̴� ���ҽ� �� (�б� ����)
//    ID3D11ShaderResourceView* mShaderResourceView;
//};