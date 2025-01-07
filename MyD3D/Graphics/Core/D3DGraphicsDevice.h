#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Interface/IGraphics.h"

class D3DGraphicsRenderer;
class D3DGraphicsSwapChain;
class D3DGraphicsRenderTarget;

class D3DGraphicsTexture2D;
class D3DGraphicsRTV;
class D3DGraphicsDSV;
class D3DGraphicsSRV;

class D3DGraphicsDevice 
{
public:
    static BOOL Initialize();
    static void Finalization();
public:
    // Swapchain.cpp¿¡ ±¸Çö
    static HRESULT CreateD3DSwapChain(IN HWND _hWnd, OUT D3DGraphicsSwapChain** _ppSwapChain);
private:
    static BOOL CanUse;

    static ID3D11Device* mDevice;
    static IDXGIFactory* mFactory;
    static IDXGIAdapter3* mAdapter;
public:
    static inline auto GetDevice() { 
        if(!CanUse) 
            throw std::runtime_error("No Initialize to D3DGraphicsDevice.");
        return mDevice; 
    }
    static inline auto GetFactory() {
        if (!CanUse)
            throw std::runtime_error("No Initialize to D3DGraphicsDevice.");
        return mFactory;
    }
    static inline auto GetAdapter()  {
        if (!CanUse)
            throw std::runtime_error("No Initialize to D3DGraphicsDevice.");
        return mAdapter;
    }
};