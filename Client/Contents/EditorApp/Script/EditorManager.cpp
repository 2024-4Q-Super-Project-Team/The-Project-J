#include "pch.h"
#include "EditorManager.h"

Editor::TestEditorTab*  EditorManager::mDebugEditor = nullptr;
Editor::Inspector*      EditorManager::mInspector = nullptr;
Editor::Hierarchy*      EditorManager::mHierarchy = nullptr;
Editor::GameEditor*      EditorManager::mGameEditor = nullptr;

EditorManager::~EditorManager()
{
}

void EditorManager::Start()
{
    mEditor = gameObject->AddComponent<EditorUI>();
    mGameViewport = ViewportManager::GetViewportSceneFromWindowName(WINDOW_TITLE);
    mEditorViewport = ViewportManager::GetViewportSceneFromWindowName(EDITOR_TITLE);
    if (mEditorViewport)
        mEditor->SetFocusWindow(mEditorViewport->GetIWindow());

    InitializeEditorWidget();
}

void EditorManager::Tick()
{
}

void EditorManager::FixedUpdate()
{
}

void EditorManager::PreUpdate()
{
}

void EditorManager::Update()
{
}

void EditorManager::PostUpdate()
{
}

void EditorManager::PreRender()
{
}

void EditorManager::Render()
{
}

void EditorManager::PostRender()
{
}

void EditorManager::InitializeEditorWidget()
{
    // 메뉴
    CreateMainMenuBar();
    // 윈도우컨테이너
    CreateMainWindowContainer();
}

void EditorManager::CreateMainMenuBar()
{
    // 메뉴바 및 메뉴, 메뉴아이템 생성
    Editor::MenuBar* pMenuBar = new Editor::MenuBar();
    Editor::Menu* pMenu_01 = pMenuBar->AddMenu("Menu");
    Editor::Menu* pMenu_02 = pMenuBar->AddMenu("System");
    Editor::Menu* pMenu_03 = pMenuBar->AddMenu("Option");

    auto pItem_01_01 = pMenu_01->AddItem("Elem_01", "Ctrl + 1", []() {Display::Console::Log("click!");});
    auto pItem_01_02 = pMenu_01->AddItem("Elem_02", "Ctrl + 2", []() {Display::Console::Log("click!");});
    auto pItem_01_03 = pMenu_01->AddItem("Elem_03", "Ctrl + 3", []() {Display::Console::Log("click!");});
    auto pItem_01_04 = pMenu_01->AddItem("Elem_04", "Ctrl + 4", []() {Display::Console::Log("click!");});

    auto pItem_02_01 = pMenu_02->AddItem("Elem_01", "Ctrl + 1", []() {Display::Console::Log("click!");});
    auto pItem_02_02 = pMenu_02->AddItem("Elem_02", "Ctrl + 2");
    auto pItem_02_03 = pMenu_02->AddItem("Elem_03", "Ctrl + 3");
    auto pItem_02_04 = pMenu_02->AddItem("Elem_04", "Ctrl + 4");

    mEditor->AddWidget(pMenuBar);
}

void EditorManager::CreateMainWindowContainer()
{
    // 윈도우컨테이너 생성
    Editor::WindowBar* pWindowRect;
    {
        POINT size = mEditor->GetWindow()->GetSize();
        pWindowRect = new Editor::WindowBar("Editor_Bar_01", Vector2(size.x / 2, size.y));
        pWindowRect->AddFlags(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        mEditor->AddWidget(pWindowRect);
        {
            // 탭바 생성, 인스펙터
            Editor::TabBar* pTAbBar = new Editor::TabBar("TabBar_01");
            CreateTestTab(pTAbBar);
            CreateHierarchy(pTAbBar);
            CreateGameEditor(pTAbBar);
            pWindowRect->AddWidget(pTAbBar);
        }
    }
    {
        POINT size = mEditor->GetWindow()->GetSize();
        pWindowRect = new Editor::WindowBar("Editor_Bar_02", Vector2(size.x / 2, size.y), Vector2(size.x / 2, 0));
        pWindowRect->AddFlags(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        mEditor->AddWidget(pWindowRect);
        {
            // 탭바 생성, 하이라키
            Editor::TabBar* pTAbBar = new Editor::TabBar("TabBar_02");
            CreateInspector(pTAbBar);
            pWindowRect->AddWidget(pTAbBar);
        }
    }

    mHierarchy->SetFocusWorldManager(mGameViewport->GetWorldManager());
    mGameEditor->SetFocusWorldManager(mGameViewport->GetWorldManager());
    mHierarchy->SetFocusInspector(mInspector);

    mDebugEditor->SetFocusInspector(mInspector);

    mGameEditor->SetFocusInspector(mInspector);
}
void EditorManager::CreateTestTab(Editor::TabBar* _pSrcTabBar)
{
    {   // 테스트용 탭
        mDebugEditor = new Editor::TestEditorTab();
        Editor::Tab* pInspectorBar = new Editor::Tab("Debug");
        pInspectorBar->AddWidget(mDebugEditor);
        _pSrcTabBar->AddTab(pInspectorBar);
    }
}

void EditorManager::CreateInspector(Editor::TabBar* _pSrcTabBar)
{
    {   // 인스펙터 탭
        mInspector = new Editor::Inspector();
        Editor::Tab* pInspectorBar = new Editor::Tab("Inspector");
        pInspectorBar->AddWidget(mInspector);
        _pSrcTabBar->AddTab(pInspectorBar);
    }
}

void EditorManager::CreateHierarchy(Editor::TabBar* _pSrcTabBar)
{
    {   // 하이라키 탭
        mHierarchy = new Editor::Hierarchy();
        Editor::Tab* pInspectorBar = new Editor::Tab("Hierarchy");
        pInspectorBar->AddWidget(mHierarchy);
        _pSrcTabBar->AddTab(pInspectorBar);
    }
}

void EditorManager::CreateGameEditor(Editor::TabBar* _pSrcTabBar)
{
    {   // 게임 에디터 탭
        mGameEditor = new Editor::GameEditor();
        Editor::Tab* pInspectorBar = new Editor::Tab("GameEditor");
        pInspectorBar->AddWidget(mGameEditor);
        _pSrcTabBar->AddTab(pInspectorBar);
    }
}

json EditorManager::Serialize()
{
    return json();
}

void EditorManager::Deserialize( json& j)
{
}

