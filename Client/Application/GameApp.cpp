#include "pch.h"
#include "GameApp.h"
#include "WinProc.h"

#include "Contents/GameApp/World/TestWorld.h"
#include "Contents/EditorApp/World/EditorWorld.h"
#include "ScriptRegister.h"

ViewportScene* GameApp::mMainScene      = nullptr;
ViewportScene* GameApp::mEditorScene    = nullptr;

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
            winDecs.WndClass.lpfnWndProc = WinProc;
            mMainScene = ViewportManager::CreateViewportScene(&winDecs);
            mMainScene->GetIWindow()->SetPositionCenter();
        }
        WorldManager* wrdMng = mMainScene->GetWorldManager();
        if (nullptr == wrdMng) return FALSE;

        World* wolrd = wrdMng->CreateWorld<TestWorld>(L"TestWorld", L"Deafult");
        //wrdMng->LoadWorlds();
        wrdMng->SetActiveWorld(L"TestWorld");
#ifdef _DEBUG
        /////////////////////////////////////////////////////
        // 에디터
        /////////////////////////////////////////////////////
        {
            {
                Display::WindowDesc winDecs;
                winDecs.Size = { 800, WINDOW_HEIGHT + mMainScene->GetIWindow()->GetOffset().y };
                winDecs.Position = { 0,0 };
                winDecs.WndStyle = WS_POPUP | WS_VISIBLE;
                winDecs.WndClass.lpszClassName = EDITOR_TITLE;
                winDecs.WndClass.lpfnWndProc = EditorWinProc;
                winDecs.WndParent = mMainScene->GetIWindow();
                mEditorScene = ViewportManager::CreateViewportScene(&winDecs);
            }
            WorldManager* wrdMng = mEditorScene->GetWorldManager();
            if (nullptr == wrdMng) return FALSE;

            World* wolrd = wrdMng->CreateWorld<EditorWorld>(L"EditorWorld", L"");
            wrdMng->SetActiveWorld(L"EditorWorld");  
        }
        GameApp::EditorRePosition();
#endif // DEBUG
    }
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
        EditorRePosition();
        break;
    case WM_MOVE:
        EditorRePosition();
        break;
    default:
        break;
    }
    return void _CALLBACK();
}

void GameApp::EditorRePosition()
{
    if (mMainScene && mEditorScene)
    {
        auto EditorWindow = mEditorScene->GetIWindow();
        POINT pos = mMainScene->GetIWindow()->GetPosition();
        POINT size = mMainScene->GetIWindow()->GetSize();
        POINT resPos = { pos.x + size.x + 10 , pos.y};
        EditorWindow->SetPosition(resPos);
    }
}
