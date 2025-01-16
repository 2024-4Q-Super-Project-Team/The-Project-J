#include "pch.h"
#include "WinProc.h"
#include "GameApp.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WinProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(_hwnd, _msg, _wParam, _lParam))
    {
        return true;
    }
    //ViewportScene* handler = GameManager::GetViewportManager()->GetViewportSceneFromHwnd(_hwnd);
    switch (_msg)
    {
        // 윈도우 만들어 졌을 때
    case WM_CREATE:
        break;
        // 윈도우 사이즈 조절할 때
    case WM_SIZE:
        GameApp::EditorRePosition();
        break;
        // 윈도우 이동할 때
    case WM_MOVE: 
        GameApp::EditorRePosition();
        break;
        // 시스템 키 눌르고 뗏을 때일걸..요?
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
        if (_wParam == VK_MENU)
            return 0;
        break;
        // 윈도우가 포커싱 되었을 때
    case WM_SETFOCUS:
        break;
        // 윈도우가 사라질 때
    case WM_DESTROY:
        //pApp->DestroyDisplay(_hwnd);
        break;
        // 윈도우 우측 상단 종료 버튼을 눌렀을 때
    case WM_CLOSE:
        if (GameManager::GetApplication())
            GameManager::GetApplication()->ShutDown();
        break;
    default:
        return DefWindowProc(_hwnd, _msg, _wParam, _lParam);
    }
    return 0;
}
LRESULT EditorWinProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(_hwnd, _msg, _wParam, _lParam))
    {
        return true;
    }
    Application* pApp = GameManager::GetApplication();
    switch (_msg)
    {
        // 윈도우 만들어 졌을 때
    case WM_CREATE:
        break;
        // 윈도우 사이즈 조절할 때
    case WM_SIZE:
        break;
        // 윈도우 이동할 때
    case WM_MOVE:
        break;
        // 시스템 키 눌르고 뗏을 때일걸..요?
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
        if (_wParam == VK_MENU)
            return 0;
        break;
        // 윈도우가 포커싱 되었을 때
    case WM_SETFOCUS:
        break;
        // 윈도우가 사라질 때
    case WM_DESTROY:
        //pApp->DestroyDisplay(_hwnd);
        break;
        // 윈도우 우측 상단 종료 버튼을 눌렀을 때
    case WM_CLOSE:
        if (pApp)
            pApp->ShutDown();
        break;
    default:
        return DefWindowProc(_hwnd, _msg, _wParam, _lParam);
    }
    return 0;
}