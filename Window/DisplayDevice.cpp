#include "pch.h"
#include "DisplayDevice.h"

namespace Display
{
    DisplayDevice::DisplayDevice(HINSTANCE _hInstance)
        : mHInstance(_hInstance)
    {
    }
    DisplayDevice::~DisplayDevice()
    {
    }
    void DisplayDevice::Release()
    {
        delete this;
    }
    HRESULT DisplayDevice::CreateWindowDisplay(WindowDesc* _pWndDesc, IWindow** _ppIWindow)
    {
        if (!_pWndDesc)
        {
            return E_INVALIDARG;
        }
        if (!(*_pWndDesc).WndClass.lpfnWndProc)
        {
            return E_INVALIDARG;
        }

        (*_pWndDesc).WndClass.hInstance = mHInstance;
        (*_pWndDesc).WndClass.hIcon = LoadIcon(mHInstance, IDI_APPLICATION);

        if (!RegisterClass(&(*_pWndDesc).WndClass))
        {
            throw std::runtime_error("Failed to register window class.");
        }

        RECT rect = { 0, 0
            , static_cast<LONG>((*_pWndDesc).Size.x)
            , static_cast<LONG>((*_pWndDesc).Size.y) };
        AdjustWindowRect(&rect, (*_pWndDesc).WndStyle, FALSE);
        const LONG width = rect.right - rect.left;
        const LONG height = rect.bottom - rect.top;
        HWND hParent = (*_pWndDesc).WndParent == nullptr ? nullptr : (*_pWndDesc).WndParent->GetHandle();

        HWND hwnd = CreateWindow(
            (*_pWndDesc).WndClass.lpszClassName,
            (*_pWndDesc).WndClass.lpszClassName,
            (*_pWndDesc).WndStyle,
            rect.left, rect.top, width, height,
            hParent, NULL, mHInstance, NULL);

        if (!hwnd)
        {
            return E_FAIL;
        }
        (*_ppIWindow) = new Window(this, mHInstance, hwnd, _pWndDesc);
        mDisplays.insert(std::make_pair(hwnd, (*_ppIWindow)));
        if (!(*_ppIWindow))
        {
            return E_INVALIDARG;
        }

        ShowWindow(hwnd, SW_SHOWNORMAL);
        UpdateWindow(hwnd);

        return S_OK;
    }
    HRESULT DisplayDevice::DestroyDisplay(IDisplay** _ppDisplay)
    {
        return DestroyDisplay((*_ppDisplay)->GetHandle());
    }
    HRESULT DisplayDevice::DestroyDisplay(HWND _hwnd)
    {
        auto itr = mDisplays.find(_hwnd);
        if (itr == mDisplays.end())
        {
            return E_FAIL;
        };
        IDisplay* pDisplay = (*itr).second;
        mDisplays.erase(itr);
        delete pDisplay;
        return S_OK;
    }
}