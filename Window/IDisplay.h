#pragma once
#include "DisplayDesc.h"
#include "Console.h"
namespace Display
{
	class IDisplay;
	class IWindow;
	class IConsole;
	class IDisplayDevice;

	HRESULT CreateIDisplayDevice(HINSTANCE, IDisplayDevice**);

	class IDisplayDevice
	{
	public:
		IDisplayDevice() = default;
		virtual ~IDisplayDevice() = default;
		IDisplayDevice(const IDisplayDevice& _other) = default;
		IDisplayDevice(IDisplayDevice&& _other) noexcept = default;
		IDisplayDevice& operator=(const IDisplayDevice& _other) = default;
		IDisplayDevice& operator=(IDisplayDevice&& _other) noexcept = default;
	public:
		virtual void Release() = 0;
	public:
		virtual HRESULT CreateWindowDisplay(WindowDesc* _pWindDesc, IWindow** _ppIWindow) = 0;
		virtual HRESULT CreateConsoleDisplay(ConsoleDesc* _pConsoleDesc, IConsole** _ppIConsole) = 0;
		virtual HRESULT DestroyDisplay(IDisplay** _ppDisplay) = 0;
		virtual HRESULT DestroyDisplay(HWND _hwnd) = 0;
	};

	class IDisplay
	{
	public:
		IDisplay() = default;
		virtual ~IDisplay() = default;
	public:
		virtual HWND			GetHandle() = 0;
		virtual const WCHAR*	GetTitle() = 0;
		virtual RECT			GetRect() = 0;
		virtual POINT			GetPosition() = 0;
		virtual POINT			GetSize() = 0;

		virtual BOOL			SetPosition(POINT _xy) = 0;
		virtual BOOL			SetSize(POINT _wh) = 0;

		virtual BOOL			Show(BOOL _bShow) = 0;
	};

	class IWindow : public IDisplay
	{
	public:
		IWindow() = default;
		virtual ~IWindow() = default;
		IWindow(const IWindow& _other) = default;
		IWindow(IWindow&& _other) noexcept = default;
		IWindow& operator=(const IWindow& _other) = default;
		IWindow& operator=(IWindow&& _other) noexcept = default;
	public:
		virtual HWND GetParentHandle() = 0;
		virtual void SetFocus() = 0;
		virtual BOOL IsFocusing() = 0;
		virtual BOOL Show(BOOL _bShow) = 0;
	};
}


