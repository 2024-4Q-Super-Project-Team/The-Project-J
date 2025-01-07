#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Interface/IGraphics.h"

class D3DGraphicsRenderTarget;
class D3DGraphicsRTV;
class D3DGraphicsDSV;
class D3DGraphicsSRV;
class D3DGraphicsVertexBuffer;
class D3DGraphicsIndexBuffer;

class D3DGraphicsSwapChain : public I3DGraphicsCore
{
public:
    explicit D3DGraphicsSwapChain(HWND _hWnd, IDXGISwapChain* _pSwapChain, ID3D11RenderTargetView* _pBackBufferRTV, ID3D11DepthStencilView* _pDepthStencilView);
    virtual ~D3DGraphicsSwapChain() = default;
public:
    virtual void Release() override;
public:
    // 스왑체인의 리소스 뷰를 클리어합니다.
    void BeginDraw();
    // 스왑체인의 크기와 백버퍼 크기를 윈도우(멤버 HWND참조) 사이즈에 맞게 재조정합니다. 
    void ReSize();
    // 스왑체인의 백버퍼에 해당 렌더타겟의 버퍼를 그립니다.
    void Draw(D3DGraphicsRenderTarget* _pRenderTarget);
    // 스왑체인의 백버퍼에 있는 그림을 전면부에 출력합니다.
    void EndDraw();
public:
    const HWND                  mHwnd;
    IDXGISwapChain*             mSwapChain;
    ID3D11RenderTargetView*     mBackBufferRTV;
    ID3D11DepthStencilView*     mDepthStencilView;

    D3DGraphicsVertexBuffer*    mVertexBuffer;
    D3DGraphicsIndexBuffer*     mIndexBuffer;
};

