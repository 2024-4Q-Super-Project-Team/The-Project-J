#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Interface/IGraphics.h"

class D3DResourceView;
class D3DGraphicsRTV;
class D3DGraphicsDSV;
class D3DGraphicsSRV;

class D3DGraphicsRenderTarget : public I3DGraphicsCore
{
public:
    explicit D3DGraphicsRenderTarget();
    virtual ~D3DGraphicsRenderTarget() = default;
public:
    virtual void Release() = 0;
public:
    virtual void BeginDraw();
    virtual void Clear();
    virtual void EndDraw() = 0;
public:
    BOOL SetTarget();
    void Reset();
    virtual void Resize(UINT _width, UINT _height) = 0;
public:
    inline FLOAT SetClearColor(FLOAT* _color) { memcpy(mClearColor, _color, sizeof(FLOAT) * 3); }
public:
    inline BOOL             IsBinding()     { return isBinding; }
    inline UINT             GetWidth()      { return mWidth; }
    inline UINT             GetHeight()     { return mHeight; }
    inline FLOAT*           GetColor()      { return mClearColor; }
    inline UINT             GetCount()      { return mRenderTargetViews.size(); }
    inline D3DGraphicsRTV*  GetRTV(UINT _index = 0) { return mRenderTargetViews[_index]; }
    inline D3DGraphicsDSV*  GetDSV()        { return mDepthStencilView; }
protected:
    // �������Ҷ� �׷����� ��ü (���� ����)
    std::vector<D3DGraphicsRTV*> mRenderTargetViews;
    // ���� ���� ���� Z-PASS ����
    D3DGraphicsDSV* mDepthStencilView = nullptr;
    // ��ó���Ҷ� �� ���̴� ���ҽ� �� (�б� ����)
    std::unordered_map<D3DResourceView*, D3DGraphicsSRV*> mShaderResourceViewTable;
    // �ʺ�, ����
    UINT mWidth = 0.0f;
    UINT mHeight = 0.0f;
    // ��ũ�� Ŭ����� ����
    FLOAT mClearColor[4] = { 0.0f,0.0f,0.0f, 1.0f };
    // ���ε� ����
    BOOL isBinding = FALSE;
protected: 
    // ���ε� ���� ����
    static std::vector<D3DGraphicsRenderTarget*> mRenderTargetBindingStack;
};

// �� ����Ÿ���� ���� ����Ÿ������, �����쿡 �������ִ� ������ �Ѵ�.
class D3DHwndRenderTarget : public D3DGraphicsRenderTarget
{
public:
    explicit D3DHwndRenderTarget(HWND _hWnd);
    virtual ~D3DHwndRenderTarget();
public:
    virtual void Release() override;
    virtual void EndDraw() override;
public:
    virtual void Resize(UINT _width, UINT _height) override;
private:
    const HWND      mHwnd;
    IDXGISwapChain* mSwapChain;
};


class D3DBitmapRenderTarget : public D3DGraphicsRenderTarget
{
public:
    explicit D3DBitmapRenderTarget(UINT _width, UINT _height);
    virtual ~D3DBitmapRenderTarget();
public:
    virtual void Release() override;
    virtual void EndDraw() override;
public:
    virtual void Resize(UINT _width, UINT _height) override;
public:
    void BindAllSRV();
    void ResetAllSRV();

    void RemoveResourceView(D3DGraphicsRTV* _pRTV);
    void RemoveResourceView(D3DGraphicsDSV* _pDSV);
public:
    void PushResourceView(D3DGraphicsRTV* _pRTV, D3DGraphicsSRV* _pSRV = nullptr);
    void PushResourceView(D3DGraphicsDSV* _pDSV, D3DGraphicsSRV* _pSRV = nullptr);
    void PushResourceView(std::pair<D3DGraphicsRTV*, D3DGraphicsSRV*> _pResourceView);
    void PushResourceView(std::pair<D3DGraphicsDSV*, D3DGraphicsSRV*> _pResourceView);
public:
    D3DGraphicsSRV* GetSRV(D3DResourceView* _pView);
};