#pragma once

namespace Display
{
	class IDisplay;
	class IWindow;
	class IConsole;
	struct DisplayDesc
	{
	    POINT Position = { 0,0 };
		POINT Size = { 0,0 };
	};

	struct WindowDesc : public DisplayDesc
	{
		WindowDesc();
		WNDCLASS WndClass = {};
		DWORD	 WndStyle = WS_OVERLAPPEDWINDOW;
		IWindow* WndParent = nullptr;
	};

	struct ConsoleDesc : public DisplayDesc
	{
	};
}


