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
        // ������ ����� ���� ��
    case WM_CREATE:
        break;
        // ������ ������ ������ ��
    case WM_SIZE:
        GameApp::EditorRePosition();
        break;
        // ������ �̵��� ��
    case WM_MOVE: 
        GameApp::EditorRePosition();
        break;
        // �ý��� Ű ������ ���� ���ϰ�..��?
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
        if (_wParam == VK_MENU)
            return 0;
        break;
        // �����찡 ��Ŀ�� �Ǿ��� ��
    case WM_SETFOCUS:
        break;
        // �����찡 ����� ��
    case WM_DESTROY:
        //pApp->DestroyDisplay(_hwnd);
        break;
        // ������ ���� ��� ���� ��ư�� ������ ��
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
        // ������ ����� ���� ��
    case WM_CREATE:
        break;
        // ������ ������ ������ ��
    case WM_SIZE:
        break;
        // ������ �̵��� ��
    case WM_MOVE:
        break;
        // �ý��� Ű ������ ���� ���ϰ�..��?
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
        if (_wParam == VK_MENU)
            return 0;
        break;
        // �����찡 ��Ŀ�� �Ǿ��� ��
    case WM_SETFOCUS:
        break;
        // �����찡 ����� ��
    case WM_DESTROY:
        //pApp->DestroyDisplay(_hwnd);
        break;
        // ������ ���� ��� ���� ��ư�� ������ ��
    case WM_CLOSE:
        if (pApp)
            pApp->ShutDown();
        break;
    default:
        return DefWindowProc(_hwnd, _msg, _wParam, _lParam);
    }
    return 0;
}