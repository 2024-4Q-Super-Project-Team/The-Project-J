#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Interface/IGraphics.h"

class D3DGraphicsRTV;
class D3DGraphicsDSV;
class D3DGraphicsSRV;

// 렌더링이 될 주체에 대한 정의 클래스
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
    // 스크린 클리어시 색깔
    FLOAT                       mClearColor[3] = { 1.0f,1.0f,1.0f };
    // 
    IDXGISwapChain* mSwapChain;
    // 렌더링할때 그려지는 주체 (쓰기 전용)
    D3DGraphicsRTV* mRenderTargetView;
    // 깊이 값을 통한 Z-PASS 수행
    D3DGraphicsDSV* mDepthStencilView;
    // 후처리할때 쓸 세이더 리소스 뷰 (읽기 전용)
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
//    // 스크린 클리어시 색깔
//    FLOAT               mClearColor[3] = { 1.0f,1.0f,1.0f };
//    // 렌더링할때 그려지는 주체 (쓰기 전용)
//    ID3D11RenderTargetView* mRenderTargetView;
//    // 깊이 값을 통한 Z-PASS 수행
//    ID3D11DepthStencilView* mDepthStencilView;
//    // 후처리할때 쓸 세이더 리소스 뷰 (읽기 전용)
//    ID3D11ShaderResourceView* mShaderResourceView;
//};