#include "pch.h"
#include "ViewportManager.h"
#include "Application/Application.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"
#include "Editor/EditorManager.h"
#include "ViewportScene/ViewportScene.h"


ViewportScene* ViewportManager::mActiveViewport = nullptr;
Display::IDisplayDevice* ViewportManager::mDisplayDevice = nullptr;
std::vector<ViewportScene*> ViewportManager::mViewportScenes;

BOOL ViewportManager::Initialize()
{
    HRESULT hr = Display::CreateIDisplayDevice(
        GameManager::GetApplication()->GetHInstance()
        , &mDisplayDevice);
    if (FAILED(hr))
    {
        throw std::runtime_error("Hresult Failed to CreateIDisplayDevice");
    }
    return TRUE;
}

void ViewportManager::Run()
{
    for (auto& vp : mViewportScenes)
    {
        mActiveViewport = vp;
        Tick();
        FixedUpdate();
        PreUpdate();
        Update();
        PostUpdate();
        PreRender();
        Render();
        PostRender();
    }
    mActiveViewport = nullptr;
}

void ViewportManager::Finalization()
{
    SAFE_DELETE_ARRAY(mViewportScenes);
    mDisplayDevice->Release();
}

void ViewportManager::Tick()
{
    mActiveViewport->Tick();
}

void ViewportManager::FixedUpdate()
{
    static float counter = 0.0f;

    counter += Time::GetUnScaledDeltaTime();
    while (counter >= GameManager::GetFixedUpdateTick())
    {
        counter -= GameManager::GetFixedUpdateTick();
        mActiveViewport->FixedUpdate();
    }
}

void ViewportManager::PreUpdate()
{
    mActiveViewport->PreUpdate();
}

void ViewportManager::Update()
{
    mActiveViewport->Update();
}

void ViewportManager::PostUpdate()
{
    mActiveViewport->PostUpdate();
}

void ViewportManager::PreRender()
{
    mActiveViewport->PreRender();
}

void ViewportManager::Render()
{
    mActiveViewport->Render();
}

void ViewportManager::PostRender()
{
    mActiveViewport->PostRender();
}

ViewportScene* ViewportManager::CreateViewportScene(Display::WindowDesc* _pWinDesc)
{
    _pWinDesc->WndClass.lpfnWndProc = ViewportManager::WinProc;

    Display::IWindow* pWindow = nullptr;
    D3DHwndRenderTarget* pSwapChain = nullptr;
    if (FAILED(mDisplayDevice->CreateWindowDisplay(_pWinDesc, &pWindow)))
    {
        throw std::runtime_error("Hresult Failed to ViewportScene::CreateViewport...CreateWindowDisplay()");
    }
    pSwapChain = new D3DHwndRenderTarget(pWindow->GetHandle());
    if (pWindow && pSwapChain)
    {
        const WCHAR* winName = _pWinDesc->WndClass.lpszClassName;
        ViewportScene* pViewport = new ViewportScene(winName, pWindow, pSwapChain);
        mViewportScenes.push_back(pViewport);
        mActiveViewport = pViewport;
        return pViewport;
    }
    return nullptr;
}

ViewportScene* ViewportManager::GetViewportSceneFromWindowName(std::wstring _winName)
{
    auto itr = FIND_CONTAINER(mViewportScenes,
        [&_winName](const ViewportScene* _pViewport)
        {
            return (_pViewport->GetName() == _winName);
        });
    if (FIND_SUCCESS(itr, mViewportScenes))
    {
        return (*itr);
    }
    else
    {
        return nullptr;
    }
}

ViewportScene* ViewportManager::GetViewportSceneFromHwnd(HWND _hWnd)
{
    auto itr = FIND_CONTAINER(mViewportScenes,
        [&_hWnd](ViewportScene* _pViewport)
        {
            return (_pViewport->GetIWindow()->GetHandle() == _hWnd);
        });
    if (FIND_SUCCESS(itr, mViewportScenes))
    {
        return (*itr);
    }
    else
    {
        return nullptr;
    }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK ViewportManager::WinProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(_hwnd, _msg, _wParam, _lParam))
    {
        return true;
    }

    auto pViewport = GetViewportSceneFromHwnd(_hwnd);

    switch (_msg)
    {
    case WM_CREATE:
        break;
    case WM_SIZE:
        if (pViewport)
        {
            pViewport->ResizeSharedResourceView();
        }
        break;
    case WM_MOVE:
        if (pViewport)
        {
            if (EditorManager::IsForcusView(pViewport))
                EditorManager::EditorReposition();
        }
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
    // Application CallBack
    if (GameManager::GetApplication() && pViewport)
    {
        GameManager::GetApplication()->OnWindowMessage(
            pViewport,
            _msg,
            _wParam,
            _lParam
        );
    }
    return 0;
}