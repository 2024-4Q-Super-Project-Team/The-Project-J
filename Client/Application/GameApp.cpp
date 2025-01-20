#include "pch.h"
#include "GameApp.h"
#include "Contents/GameApp/World/TestWorld.h"
#include "ScriptRegister.h"

ViewportScene* GameApp::mMainScene = nullptr;
ViewportScene* GameApp::mEditorScene = nullptr;

BOOL GameApp::OnPreInitialize()
{
    ScriptRegister sr;
    sr.Register();
    return TRUE;
}

BOOL GameApp::OnPostInitialize()
{
    Display::Console::OpenConsole(L"Debug", { 0,0 }, { 300,200 });
    {
        /////////////////////////////////////////////////////
        // 게임
        /////////////////////////////////////////////////////
        {
            Display::WindowDesc winDecs;
            winDecs.Size = { WINDOW_WIDTH, WINDOW_HEIGHT };
            winDecs.WndStyle = WS_OVERLAPPEDWINDOW;
            winDecs.WndClass.lpszClassName = WINDOW_TITLE;
            mMainScene = ViewportManager::CreateViewportScene(&winDecs);
            mMainScene->GetIWindow()->SetPositionCenter();
        }
       
#ifdef _DEBUG
        /////////////////////////////////////////////////////
        // 에디터
        /////////////////////////////////////////////////////
        EditorManager::ShowEditorWindow(mMainScene);
#endif // DEBUG
    }

    WorldManager* wrdMng = mMainScene->GetWorldManager();
    wrdMng->LoadWorlds();

    return TRUE;
}

void GameApp::OnPreFinalization()
{

}

void GameApp::OnPostFinalization()
{
}

void _CALLBACK GameApp::OnWindowMessage(ViewportScene* _pViewport, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
    switch (_msg)
    {
    case WM_SIZE:
        break;
    case WM_MOVE:
        break;
    default:
        break;
    }
    return void _CALLBACK();
}
