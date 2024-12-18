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
		static inline HWND	GetHandle();
		static inline POINT	GetPosition();
		static inline POINT	GetSize();
		static inline BOOL	SetPosition(POINT _xy);
		static inline BOOL	SetSize(POINT _wh);
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
				// ���� ���ڸ� ó���ϴ� wstringstream ���
				std::wstringstream wss;
				(wss << ... << args);  // fold expression�� ����� ���� ���� ��Ʈ���� ���

				std::wstring logMessage = wss.str();
				DWORD charsWritten = 0;

				// ��ȯ�� ���ڿ� ���
				WriteConsoleW(mOutputHandle, logMessage.c_str(), static_cast<DWORD>(logMessage.size()), &charsWritten, nullptr);
			}
		}
#endif // DEBUG
	}
}