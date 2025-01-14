#include "pch.h"
#include "Graphics/Core/D3DGraphicsDevice.h"
#include "Graphics/Core/D3DGraphicsRenderer.h"
#include "Graphics/Core/D3DGraphicsRenderTarget.h"
#include "Graphics/GPUResource/D3DGraphicsTexture.h"

ID3D11Device*  D3DGraphicsDevice::mDevice = nullptr;
IDXGIFactory*  D3DGraphicsDevice::mFactory = nullptr;
IDXGIAdapter3* D3DGraphicsDevice::mAdapter = nullptr;
BOOL           D3DGraphicsDevice::CanUse = FALSE;

BOOL D3DGraphicsDevice::Initialize()
{
    UINT creationFlags = 0;
#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
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

void D3DGraphicsDevice::GetDisplayMemoryInfo(std::string& out)
{
	DXGI_ADAPTER_DESC desc;
	mAdapter->GetDesc(&desc);
	DXGI_QUERY_VIDEO_MEMORY_INFO local, nonLocal;
	mAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &local);
	mAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_NON_LOCAL, &nonLocal);

	out = std::to_string((desc.DedicatedVideoMemory + desc.SharedSystemMemory) / 1024 / 1024) + " MB\n";
	out += "Dedicated Video Memory : " + std::to_string(desc.DedicatedVideoMemory / 1024 / 1024) + " MB\n";
	out += "nShared System Memory : " + std::to_string(desc.SharedSystemMemory / 1024 / 1024) + " MB\n";

	out += "Local Video Memory: ";
	out += std::to_string(local.Budget / 1024 / 1024) + "MB" + " / " + std::to_string(local.CurrentUsage / 1024 / 1024) + " MB\n";
	out += "NonLocal Video Memory: ";
	out += std::to_string(nonLocal.Budget / 1024 / 1024) + "MB" + " / " + std::to_string(nonLocal.CurrentUsage / 1024 / 1024) + " MB";
}

void D3DGraphicsDevice::GetVirtualMemoryInfo(std::string& out)
{
	HANDLE hProcess = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS_EX pmc;
	pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);
	GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	out = "PrivateUsage: " + std::to_string((pmc.PrivateUsage) / 1024 / 1024) + " MB\n";
	out += "WorkingSetSize: " + std::to_string((pmc.WorkingSetSize) / 1024 / 1024) + " MB\n";
	out += "PagefileUsage: " + std::to_string((pmc.PagefileUsage) / 1024 / 1024) + " MB";
}
