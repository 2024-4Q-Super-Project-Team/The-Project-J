#include "pch.h"
#include "GameApp.h"
//#include "Contents/GameApp/World/TestWorld.h"
#include "ScriptRegister.h"

ViewportScene* GameApp::mMainScene = nullptr;
Display::IWindow* GameApp::mGameWindow = nullptr;

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
        // ����
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
        EditorManager::ShowEditorWindow(mMainScene);
        GameManager::SetRunType(eEngineRunType::EDITOR_MODE);
#else
        GameManager::SetRunType(eEngineRunType::GAME_MODE);
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
    SAFE_DELETE(mGameWindow);
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
