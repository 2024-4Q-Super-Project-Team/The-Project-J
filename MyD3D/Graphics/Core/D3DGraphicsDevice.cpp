#include "pch.h"
#include "Graphics/Core/D3DGraphicsDevice.h"
#include "Graphics/Core/D3DGraphicsRenderer.h"
#include "Graphics/Core/D3DGraphicsRenderTarget.h"
#include "Graphics/Core/D3DGraphicsSwapChain.h"
#include "Graphics/GPUResource/D3DGraphicsTexture.h"

ID3D11Device*   D3DGraphicsDevice::mDevice = nullptr;
IDXGIFactory*   D3DGraphicsDevice::mFactory = nullptr;
IDXGIAdapter3*  D3DGraphicsDevice::mAdapter = nullptr;
BOOL            D3DGraphicsDevice::CanUse = FALSE;

BOOL D3DGraphicsDevice::Initialize()
{
    UINT creationFlags = 0;
    Helper::HRT(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, nullptr,
        0, D3D11_SDK_VERSION, &mDevice, nullptr, &D3DGraphicsRenderer::mDeviceContext));
    Helper::HRT(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&mFactory));
    Helper::HRT(mFactory->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(&mAdapter)));

    CanUse = TRUE;
    return TRUE;
}

void D3DGraphicsDevice::Finalization()
{
    SAFE_RELEASE(mDevice);
    SAFE_RELEASE(mFactory);
    SAFE_RELEASE(mAdapter);
}
