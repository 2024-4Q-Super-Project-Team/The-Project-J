#include "pch.h"
#include "IDisplay.h"
#include "DisplayDevice.h"

namespace Display
{
	HRESULT CreateIDisplayDevice(HINSTANCE _hInstance, IDisplayDevice** _ppDevice)
	{
		if (!_hInstance || (*_ppDevice))
		{
			return E_INVALIDARG;
		}

		DisplayDevice* pDevice = new DisplayDevice(_hInstance);

		if (!pDevice)
		{
			return E_OUTOFMEMORY;
		}

		(*_ppDevice) = pDevice;

		return S_OK;
	}
}