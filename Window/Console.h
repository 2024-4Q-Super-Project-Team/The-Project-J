#pragma once
#include "IDisplay.h"

static void AppendToStream(std::wstringstream& wss, const wchar_t* str);
static void AppendToStream(std::wstringstream& wss, const char* str);
template <typename T>
static void AppendToStream(std::wstringstream& wss, T&& value);

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
        if (mHwnd)
        {
            if (mOutputHandle != INVALID_HANDLE_VALUE)
            {
                std::wstringstream wss;

                // ���� ���� ó��
                (void)(std::initializer_list<int>{
                    (AppendToStream(wss, std::forward<Args>(args)), 0)...});

                std::wstring logMessage = wss.str();
                DWORD charsWritten = 0;

                // ��ȯ�� ���ڿ� ���
                WriteConsoleW(mOutputHandle, logMessage.c_str(), static_cast<DWORD>(logMessage.size()), &charsWritten, nullptr);
            }
        }
    }
}

static void AppendToStream(std::wstringstream& wss, const wchar_t* str)
{
    wss << str;
}

static void AppendToStream(std::wstringstream& wss, const char* str)
{
    // char* -> wchar_t* ��ȯ
    int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
    std::wstring wstr(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, str, -1, &wstr[0], len);
    wss << wstr.c_str();
}

template <typename T>
static void AppendToStream(std::wstringstream& wss, T&& value)
{
    // ��� ���ڿ� ��Ÿ Ÿ�� ����
    wss << value;
}
