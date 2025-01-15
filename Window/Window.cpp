#include "pch.h"
#include "Window.h"
#include "DisplayDevice.h"

namespace Display
{
	Window::Window(DisplayDevice* _pDevice, HINSTANCE _hInstance, HWND _Hwnd, WindowDesc* _pWndDesc)
		: mHwnd(_Hwnd)
		, mTitle((*_pWndDesc).WndClass.lpszClassName)
		, mHParent(((*_pWndDesc).WndParent == nullptr) ? nullptr : (*_pWndDesc).WndParent->GetHandle())
        , mStyle((*_pWndDesc).WndStyle)
		, mDevice(_pDevice)
		, mHInstance(nullptr)
	{
	}

	Window::~Window()
	{
		if (mHwnd)
		{
			mDevice->DestroyDisplay(mHwnd);
			DestroyWindow(mHwnd);
			mHwnd = nullptr;
			UnregisterClass(mTitle, mHInstance);
		}
	}

	HWND Window::GetHandle()
	{
		return mHwnd;
	}

	const WCHAR* Window::GetTitle()
	{
		return mTitle;
	}

	RECT Window::GetRect()
	{
        RECT rect;
        if (GetClientRect(mHwnd, &rect)) {
            return rect;
        }
        return RECT{ 0, 0, 0, 0 }; // 실패 시 기본값 반환
	}

    POINT Window::GetOffset()
    {
        RECT rect = { 0, 0, 0, 0 };
        AdjustWindowRect(&rect, GetWindowLong(mHwnd, GWL_STYLE), FALSE);
        POINT offset;
        offset.x = -rect.left;
        offset.y = -rect.top;
        return offset;
    }

    POINT Window::GetPosition()
    {
        RECT rect = {};
        POINT position = {};
        if (GetWindowRect(mHwnd, &rect)) {
            position.x = rect.left;
            position.y = rect.top;
            return position;
        }
        return position;
    }

	POINT Window::GetSize()
	{
        RECT rect = GetRect();
        POINT size;
        size.x = rect.right - rect.left;
        size.y = rect.bottom - rect.top;
        return size;
	}

	HWND Window::GetParentHandle()
	{
		return mHParent;
	}

    BOOL Window::SetPosition(POINT _xy)
    {
        const POINT size = GetSize();
        const POINT offset = GetOffset();
        BOOL res = SetWindowPos(mHwnd
            , NULL
            , static_cast<int>(_xy.x - offset.x)
            , static_cast<int>(_xy.y - offset.y)
            , 0
            , 0
            , SWP_NOZORDER | SWP_NOSIZE
        );
        return res;
    }

    BOOL Window::SetPositionCenter()
    {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        POINT size = GetSize();
        POINT rePos = {};
        rePos.x = (screenWidth - size.x) / 2;
        rePos.y = (screenHeight - size.y) / 2;
        SetPosition(rePos);
        return 0;
    }

    BOOL Window::SetSize(POINT _wh)
    {
        const POINT offset = GetOffset();
        const int Width = static_cast<int>(_wh.x + offset.x * 2);
        const int Height = static_cast<int>(_wh.y + offset.y * 2);

        BOOL res = SetWindowPos(mHwnd
            , NULL
            , 0
            , 0
            , Width
            , Height
            , SWP_NOZORDER | SWP_NOMOVE
        );
        return res;
    }


	BOOL Window::Show(BOOL _bShow)
	{
		if (mHwnd)
			return ShowWindow(mHwnd, _bShow ? SW_SHOW : SW_HIDE);
		else return FALSE;
	}

	void Window::SetFocus()
	{
		::SetFocus(mHwnd);
	}

	BOOL Window::IsFocusing()
	{
		return (BOOL)(GetFocus() == mHwnd);
	}
}