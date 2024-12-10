#include "pch.h"
#include "Graphics.h"

namespace Graphics
{
	HRESULT CreateGraphicsDeviceAndRenderer(GraphicsDevice** _ppGraphicsDevice, Renderer** _ppRenderer)
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
		Renderer* renderer = new Renderer(device, pContext);

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
}