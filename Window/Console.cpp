#include "pch.h"
#include "Console.h"

namespace Display
{
    HWND   Console::mHwnd           = nullptr;
    HWND   Console::mHParent        = nullptr;
    HANDLE Console::mOutputHandle   = nullptr;
    HANDLE Console::mInputHandle    = nullptr;

    BOOL Console::OpenConsole(const WCHAR* _title, POINT _xy, POINT _wh, LONG _style)
    {
#ifdef _DEBUG
        if (NULL == GetConsoleWindow())
        {
            BOOL bRes = AllocConsole();
            if (bRes)
            {
                mHwnd = GetConsoleWindow();
                mOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
                mInputHandle = GetStdHandle(STD_INPUT_HANDLE);
                SetConsoleTitleW(_title);
                // 스타일 변경
                LONG curStyle = GetWindowLongPtr(mHwnd, GWL_STYLE);
                SetWindowLong(mHwnd, GWL_STYLE, _style | (curStyle & WS_VISIBLE));
                SetWindowPos(mHwnd, NULL, _xy.x, _xy.y, _wh.x, _wh.y, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
                //if (_parentHwnd) SetParent(mHwnd, _parentHwnd);
                return TRUE;
            }
        }
#endif // _DEBUG
        return FALSE;
    }

    BOOL Console::CloseConsole()
    {
        if (mHwnd) {
            DestroyWindow(mHwnd);
            mHwnd = nullptr;
        }
        else return FALSE;

        if (mOutputHandle) {
            CloseHandle(mOutputHandle);
            mOutputHandle = nullptr;
        }

        if (mInputHandle) {
            CloseHandle(mInputHandle);
            mInputHandle = nullptr;
        }

        return TRUE;
    }

    HWND Console::GetHandle()
    {
        return mHwnd;
    }

    POINT Console::GetPosition()
    {
        if (mHwnd) {
            RECT rect;
            if (GetWindowRect(mHwnd, &rect)) {
                return { rect.left, rect.top };
            }
        }
        return { 0, 0 };
    }

    POINT Console::GetSize()
    {
        if (mHwnd) {
            RECT rect;
            if (GetWindowRect(mHwnd, &rect)) {
                return { rect.right - rect.left, rect.bottom - rect.top };
            }
        }
        return { 0, 0 };
    }

    BOOL Console::SetPosition(POINT _xy)
    {
        if (mHwnd) {
            return SetWindowPos(mHwnd, NULL, _xy.x, _xy.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
        }
        return FALSE;
    }

    BOOL Console::SetSize(POINT _wh)
    {
        if (mHwnd) {
            return SetWindowPos(mHwnd, NULL, 0, 0, _wh.x, _wh.y, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
        }
        return FALSE;
    }
}