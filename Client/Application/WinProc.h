#pragma once

LRESULT CALLBACK WinProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
LRESULT CALLBACK EditorWinProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

BOOL WindowReposition();