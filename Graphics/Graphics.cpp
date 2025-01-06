// Graphics.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//
#include "pch.h"
#include "framework.h"
#include "GraphicsDevice.h"
#include "GraphicsRenderer.h"


HRESULT CreateGraphicsDeviceAndRenderer(IGraphicsDevice** _ppGraphicsDevice, IGraphicsRenderer** _ppRenderer)
{
    ID3D11Device* pDevice;
    IDXGIFactory* pFactory;
    IDXGIAdapter3* pDXGIAdapter;
    ID3D11DeviceContext* pContext;

    {
        UINT creationFlags = 0;
        HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, nullptr,
            0, D3D11_SDK_VERSION, &pDevice, nullptr, &pContext);
        if (hr != S_OK)
            return hr;
    }

    {
        HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
        if (hr != S_OK)
            return hr;
    }

    {
        HRESULT hr = pFactory->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(&pDXGIAdapter));
        if (hr != S_OK)
            return hr;
    }

    GraphicsDevice* device = new GraphicsDevice(pDevice, pFactory, pDXGIAdapter);
    GraphicsRenderer* renderer = new GraphicsRenderer(device, pContext);

    if (device)
    {
        (*_ppGraphicsDevice) = device;
        if (renderer)
            (*_ppRenderer) = renderer;
        else
        {
            delete device;
            return E_OUTOFMEMORY;
        }
    }
    return S_OK;
}