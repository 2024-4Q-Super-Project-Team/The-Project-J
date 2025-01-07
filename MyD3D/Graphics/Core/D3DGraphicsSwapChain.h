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
    // ����ü���� ���ҽ� �並 Ŭ�����մϴ�.
    void BeginDraw();
    // ����ü���� ũ��� ����� ũ�⸦ ������(��� HWND����) ����� �°� �������մϴ�. 
    void ReSize();
    // ����ü���� ����ۿ� �ش� ����Ÿ���� ���۸� �׸��ϴ�.
    void Draw(D3DGraphicsRenderTarget* _pRenderTarget);
    // ����ü���� ����ۿ� �ִ� �׸��� ����ο� ����մϴ�.
    void EndDraw();
public:
    const HWND                  mHwnd;
    IDXGISwapChain*             mSwapChain;
    ID3D11RenderTargetView*     mBackBufferRTV;
    ID3D11DepthStencilView*     mDepthStencilView;

    D3DGraphicsVertexBuffer*    mVertexBuffer;
    D3DGraphicsIndexBuffer*     mIndexBuffer;
};

