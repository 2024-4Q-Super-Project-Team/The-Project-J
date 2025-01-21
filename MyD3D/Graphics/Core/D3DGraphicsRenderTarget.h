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
    // 렌더링할때 그려지는 주체 (쓰기 전용)
    std::vector<D3DGraphicsRTV*> mRenderTargetViews;
    // 깊이 값을 통한 Z-PASS 수행
    D3DGraphicsDSV* mDepthStencilView = nullptr;
    // 후처리할때 쓸 세이더 리소스 뷰 (읽기 전용)
    std::unordered_map<D3DResourceView*, D3DGraphicsSRV*> mShaderResourceViewTable;
    // 너비, 높이
    UINT mWidth = 0.0f;
    UINT mHeight = 0.0f;
    // 스크린 클리어시 색깔
    FLOAT mClearColor[4] = { 0.0f,0.0f,0.0f, 1.0f };
    // 바인딩 여부
    BOOL isBinding = FALSE;
protected: 
    // 바인드 상태 스택
    static std::vector<D3DGraphicsRenderTarget*> mRenderTargetBindingStack;
};

// 이 렌더타겟은 메인 렌더타겟으로, 윈도우에 스왑해주는 역할을 한다.
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