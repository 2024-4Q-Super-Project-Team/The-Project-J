#pragma once
#include "IDisplay.h"

namespace Display
{
	class DisplayDevice;

	class Window : public IWindow
	{
	public:
		Window(DisplayDevice* _pDevice, HINSTANCE _hInstance, HWND _Hwnd, WindowDesc* _pWndDesc);
		virtual ~Window();
		Window(const Window& _other) = default;
		Window(Window&& _other) noexcept = default;
		Window& operator=(const Window& _other) = default;
		Window& operator=(Window&& _other) noexcept = default;
	public:
		virtual HWND			GetHandle() override;
		virtual const WCHAR*	GetTitle() override;
		virtual RECT			GetRect() override;
        virtual POINT			GetOffset() override;
		virtual POINT			GetPosition() override;
		virtual POINT			GetSize() override;

		virtual BOOL			SetPosition(POINT) override;
		virtual BOOL			SetPositionCenter() override;
		virtual BOOL			SetSize(POINT) override;

		virtual BOOL			Show(BOOL _bShow) override;
	public:
		virtual HWND			GetParentHandle() override;
		virtual void			SetFocus() override;
		virtual BOOL			IsFocusing() override;
	private:
		HWND  mHwnd;
		HWND  mHParent;
		const WCHAR* mTitle;
        DWORD mStyle;

		HINSTANCE	   mHInstance;
		DisplayDevice* mDevice;
	};
}