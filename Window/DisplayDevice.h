#pragma once
#include "IDisplay.h"
#include "Window.h"
#include "Console.h"

namespace Display
{
	class DisplayDevice : public IDisplayDevice
	{
	public:
		DisplayDevice(HINSTANCE _hInstance);
		virtual ~DisplayDevice();
		DisplayDevice(const DisplayDevice& _other) = default;
		DisplayDevice(DisplayDevice&& _other) noexcept = default;
		DisplayDevice& operator=(const DisplayDevice& _other) = default;
		DisplayDevice& operator=(DisplayDevice&& _other) noexcept = default;
	public:
		virtual	void Release() override;
	public:
		virtual HRESULT     CreateWindowDisplay(WindowDesc* _pWindDesc, IWindow** _ppIWindow) override;
		virtual HRESULT     CreateConsoleDisplay(ConsoleDesc* _pConsoleDesc, IConsole** _ppIConsole) override;
		virtual HRESULT     DestroyDisplay(IDisplay** _ppDisplay) override;
		virtual HRESULT     DestroyDisplay(HWND _hwnd) override;
        virtual IDisplay*   GetDevice(HWND _hWnd) override;
	private:
		HINSTANCE mHInstance;
		std::unordered_map<HWND, IDisplay*> mDisplays;
	};
}