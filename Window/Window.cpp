#include "pch.h"
#include "Window.h"
#include "DisplayDevice.h"

namespace Display
{
	Window::Window(DisplayDevice* _pDevice, HINSTANCE _hInstance, HWND _Hwnd, WindowDesc* _pWndDesc)
		: mHwnd(_Hwnd)
		, mTitle((*_pWndDesc).WndClass.lpszClassName)
		, mHParent(((*_pWndDesc).WndParent == nullptr) ? nullptr : (*_pWndDesc).WndParent->GetHandle())
		, mPosition((*_pWndDesc).Position)
		, mSize((*_pWndDesc).Size)
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

	inline HWND Window::GetHandle()
	{
		return mHwnd;
	}

	inline const WCHAR* Window::GetTitle()
	{
		return mTitle;
	}

	inline RECT Window::GetRect()
	{
		return RECT();
	}

	inline POINT Window::GetPosition()
	{
		return mPosition;
	}

	inline POINT Window::GetSize()
	{
		return mSize;
	}

	inline HWND Window::GetParentHandle()
	{
		return mHParent;
	}

	inline BOOL Window::SetPosition(POINT _xy)
	{
		BOOL res;
		res = MoveWindow(mHwnd
			, static_cast<int>(_xy.x)
			, static_cast<int>(_xy.y)
			, static_cast<int>(mSize.x)
			, static_cast<int>(mSize.y)
			, TRUE);
		if (res)
		{
			mPosition = _xy;
		}
		return res;
	}

	inline BOOL Window::SetSize(POINT _wh)
	{
		BOOL res;
		res = SetWindowPos(mHwnd
			, HWND_TOP
			, 0, 0 
			, static_cast<int>(_wh.x)
			, static_cast<int>(_wh.y)
			, SWP_NOMOVE);
		if (res)
		{
			mSize = _wh;
		}
		return res;
	}

	inline void Window::SetFocus()
	{
		::SetFocus(mHwnd);
	}

	bool Window::IsFocusing()
	{
		return false;
	}
}