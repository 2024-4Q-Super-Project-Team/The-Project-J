#pragma once
#include "IDisplay.h"

namespace Display
{
	class DisplayDevice;

	class Console
	{
	public:
		static BOOL			OpenConsole(const WCHAR* _title, POINT _xy, POINT _wh, LONG _style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU);
		static BOOL			CloseConsole();
	public:
		static HWND	GetHandle();
		static POINT	GetPosition();
		static POINT	GetSize();
		static BOOL	SetPosition(POINT _xy);
		static BOOL	SetSize(POINT _wh);
	public:
		template<typename... Args>
		static void Log(Args&&... args);
	private:
		static HWND   mHwnd;
		static HWND   mHParent;
		static HANDLE mOutputHandle;
		static HANDLE mInputHandle;
	};
	template<typename... Args>
	static void Console::Log(Args&&... args)
	{
#ifdef _DEBUG
		if (mHwnd)
		{
			if (mOutputHandle != INVALID_HANDLE_VALUE)
			{
				// 가변 인자를 처리하는 wstringstream 사용
				std::wstringstream wss;
				(wss << ... << args);  // fold expression을 사용해 가변 인자 스트림에 출력

				std::wstring logMessage = wss.str();
				DWORD charsWritten = 0;

				// 변환된 문자열 출력
				WriteConsoleW(mOutputHandle, logMessage.c_str(), static_cast<DWORD>(logMessage.size()), &charsWritten, nullptr);
			}
		}
#endif // DEBUG
	}
}