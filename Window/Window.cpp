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
		return RECT();
	}

	POINT Window::GetPosition()
	{
		return mPosition;
	}

	POINT Window::GetSize()
	{
		return mSize;
	}

	HWND Window::GetParentHandle()
	{
		return mHParent;
	}

	BOOL Window::SetPosition(POINT _xy)
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

	BOOL Window::SetSize(POINT _wh)
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