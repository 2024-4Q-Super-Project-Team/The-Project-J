#include "pch.h"
#include "EditorManager.h"
#include "ViewportScene/ViewportManager.h"
#include "Manager/GameManager.h"
#include "ViewportScene/ViewportScene.h"

EditorCamera                    EditorManager::mEditorCamera;

ViewportScene* EditorManager::mFocusViewport = nullptr;
ViewportScene* EditorManager::mEditorViewport = nullptr;

std::vector<Editor::IWidget*>   EditorManager::mWidgetArray;

Editor::EditorDebugger* EditorManager::mDebbugerTab = nullptr;
Editor::ResourceViewer* EditorManager::mResourceViewer = nullptr;
Editor::HierarchyViewer* EditorManager::mHierarchyViewer = nullptr;
Editor::InspectorViewer* EditorManager::mInspectorViewer = nullptr;

Editor::MenuBar* EditorManager::mMainMenuBar = nullptr;
Editor::WindowBar* EditorManager::mMainWindowBar_01 = nullptr;
Editor::WindowBar* EditorManager::mMainWindowBar_02 = nullptr;

ImGuiContext* EditorManager::mFocusContext;
ImGuiContext* EditorManager::mEditorContext;

static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

void EditorManager::Initialize()
{
    mResourceViewer = new Editor::ResourceViewer();
    mHierarchyViewer = new Editor::HierarchyViewer();
    mInspectorViewer = new Editor::InspectorViewer();

    InitMainMenuBar();
    InitMainWindow();

    // 컨텍스트 활성화
    ImGui::SetCurrentContext(mEditorContext);
}

void EditorManager::Finalization()
{
}

void EditorManager::EditorWindowRender()
{
    if (mEditorViewport)
    {
        ImGui::SetCurrentContext(mEditorContext);
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        UpdateIO(mEditorContext);

        // 위젯 렌더링
        for (auto& widget : mWidgetArray)
        {
            widget->Render();
        }

        EditorDragNDrop::Render();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    }
}

void EditorManager::EditorUpdate()
{
    mEditorCamera.EditorUpdate();
}

void EditorManager::EditorCameraRender()
{
    mEditorCamera.EditorRender();
}

void EditorManager::EditorFocusRender()
{
    if (mFocusViewport)
    {
        EditorManager::EditorCameraRender();
        ImGui::SetCurrentContext(mFocusContext);
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();

        UpdateIO(mFocusContext);

        // 기즈모 모드 변경 버튼 테스트
        // 추후 더 이쁘게 변경
        if (ImGui::Begin("Gizmo Mode"))
        {
            if (ImGui::Button("Translate") || Input::IsKeyDown('G'))
            {
                mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
            }
            if (ImGui::Button("Rotate") || Input::IsKeyDown('R'))
            {
                mCurrentGizmoOperation = ImGuizmo::ROTATE;
            }
            if (ImGui::Button("Scale") || Input::IsKeyDown('S'))
            {
                mCurrentGizmoOperation = ImGuizmo::SCALE;
            }
            ImGui::End();
        }

        mInspectorViewer->RenderGizmo();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}

void EditorManager::UpdateIO(ImGuiContext* _context)
{
    ImGui::SetCurrentContext(_context);
    ImGuiIO& io = ImGui::GetIO();
    // 기본 입력 비활성화
    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableSetMousePos;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
    // 현재 포커스된 윈도우 확인
    HWND focusHwnd = GetFocus();
    // 대상 컨텍스트가 게임씬의 컨텍스트일 때
    if (_context == mFocusContext)
    {
        // 게임 씬의 윈도우가 포커싱인지 확인
        if (focusHwnd == mFocusViewport->GetIWindow()->GetHandle())
        { // 맞다면 io를 활성화시킨다.
            POINT mousePos;
            GetCursorPos(&mousePos);
            ScreenToClient(mFocusViewport->GetIWindow()->GetHandle(), &mousePos); // 현재 윈도우의 클라이언트 좌표로 변환
            io.MousePos = ImVec2(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
            io.MouseDown[0] = Input::IsMouseHold(Mouse::LEFT);
            io.MouseDown[1] = Input::IsMouseHold(Mouse::RIGHT);
            io.MouseDown[2] = Input::IsMouseHold(Mouse::MID);

            io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse; // 입력 활성화
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
        }
    }
    // 아니라면 에디터 윈도우
    else
    {
        // 에디터의 윈도우가 포커싱인지 확인
        if (focusHwnd == mEditorViewport->GetIWindow()->GetHandle())
        {// 맞다면 io를 활성화시킨다.
            POINT mousePos;
            GetCursorPos(&mousePos);
            ScreenToClient(mEditorViewport->GetIWindow()->GetHandle(), &mousePos); // 현재 윈도우의 클라이언트 좌표로 변환
            io.MousePos = ImVec2(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
            io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        }
    }
}

BOOL EditorManager::ShowEditorWindow(ViewportScene* _targetViewport)
{
    if (_targetViewport)
    {
        mFocusViewport = _targetViewport;
        Display::IWindow* mDestWindow = mFocusViewport->GetIWindow();
        POINT Destsize = mFocusViewport->GetIWindow()->GetSize();
        POINT Destpos = mFocusViewport->GetIWindow()->GetPosition();

        mMainWindowBar_01->SetSize(Vector2(EDITOR_WIDTH / 2, Destsize.y));
        mMainWindowBar_02->SetSize(Vector2(EDITOR_WIDTH / 2, Destsize.y));
        mMainWindowBar_02->SetOffset(Vector2(EDITOR_WIDTH / 2, 0));

        mHierarchyViewer->SetFocusWorldManager(_targetViewport->GetWorldManager());
        mHierarchyViewer->SetFocusInspector(mInspectorViewer);
        //mDebugEditor->SetFocusInspector(mInspector);

        Display::WindowDesc winDecs;
        winDecs.Size = { EDITOR_WIDTH , Destsize.y };
        winDecs.Position = { Destpos.x + Destsize.x , Destpos.y };
        winDecs.WndStyle = WS_POPUP | WS_VISIBLE;
        winDecs.WndClass.lpszClassName = L"EditorWindow";
        winDecs.WndClass.lpfnWndProc = EditorManager::EditorWinProc;
        winDecs.WndParent = mDestWindow;
        mEditorViewport = ViewportManager::CreateViewportScene(&winDecs);

        EditorReposition();

        InitEditorImGui();
        InitFocusImGui();

        return TRUE;
    }
    return FALSE;
}

BOOL EditorManager::IsRenderView(ViewportScene* _targetViewport)
{
    return mEditorViewport == _targetViewport;
}

BOOL EditorManager::IsFocusView(ViewportScene* _targetViewport)
{
    return mFocusViewport == _targetViewport;
}

void EditorManager::InitImGui(ImGuiContext* _context, HWND _hwnd)
{
    ImGui::SetCurrentContext(_context);
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init(_hwnd);
    ImGui_ImplDX11_Init(D3DGraphicsDevice::GetDevice(), D3DGraphicsRenderer::GetDevicecontext());
}

void EditorManager::InitEditorImGui()
{
    // ImGui ���ؽ�Ʈ ����
    IMGUI_CHECKVERSION();
    mEditorContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(mEditorContext);
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
    io.IniFilename = nullptr;
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesKorean());
    InitImGui(mEditorContext, mEditorViewport->GetIWindow()->GetHandle());
}

void EditorManager::InitFocusImGui()
{
    // ImGui 컨텍스트 생성
    IMGUI_CHECKVERSION();
    mFocusContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(mFocusContext);
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
    io.IniFilename = nullptr;
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesKorean());
    // ImGui 초기화
    ImGui_ImplWin32_Init(mFocusViewport->GetIWindow()->GetHandle());
    ImGui_ImplDX11_Init(D3DGraphicsDevice::GetDevice(), D3DGraphicsRenderer::GetDevicecontext());
}

void EditorManager::InitMainMenuBar()
{
    // 메뉴바 및 메뉴, 메뉴아이템 생성
    mMainMenuBar = new Editor::MenuBar();
    mWidgetArray.push_back(mMainMenuBar);


    if (Input::IsMouseDown(Mouse::RIGHT))
    {
        if (GameManager::GetRunType() == eEngineRunType::GAME_MODE)
        {
            GameManager::SetRunType(eEngineRunType::EDITOR_MODE);
        }
        else
        {
            GameManager::SetRunType(eEngineRunType::GAME_MODE);
        }
    }

    {   // File MenuTab
        Editor::MenuNode* pMenu_01 = new Editor::MenuNode("File");
        mMainMenuBar->AddMenuNode(pMenu_01);
        pMenu_01->AddMenuItem(new Editor::JsonWriter());
        pMenu_01->AddMenuItem(new Editor::JsonLoader());
    }
    {   // World MenuTab
        Editor::MenuNode* pMenu_02 = new Editor::MenuNode("World");
        mMainMenuBar->AddMenuNode(pMenu_02);
        pMenu_02->AddMenuItem(new Editor::WorldChanger());
        pMenu_02->AddMenuItem(new Editor::WorldCreator());
        pMenu_02->AddMenuItem(new Editor::WorldRemover());
    }
    {
        Editor::MenuNode* pMenu_03 = new Editor::MenuNode("Option");
        mMainMenuBar->AddMenuNode(pMenu_03);
    }
}

void EditorManager::InitMainWindow()
{
    {
        mMainWindowBar_01 = new Editor::WindowBar("Editor_Bar_01", Vector2(EDITOR_WIDTH / 2, 0));
        mMainWindowBar_01->AddFlags(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        {
            // 탭바 생성, 인스펙터
            Editor::TabBar* pTabBar = new Editor::TabBar("TabBar_01");
            mMainWindowBar_01->AddWidget(pTabBar);

            CreateDebuggerTab(pTabBar);
            CreateHierarchy(pTabBar);
            CreateResourceViewer(pTabBar);
        }
    }
    {
        mMainWindowBar_02 = new Editor::WindowBar("Editor_Bar_02", Vector2(EDITOR_WIDTH / 2, 0), Vector2(EDITOR_WIDTH / 2, 0));
        mMainWindowBar_02->AddFlags(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        {
            // 탭바 생성, 하이라키
            Editor::TabBar* pTabBar = new Editor::TabBar("TabBar_02");
            mMainWindowBar_02->AddWidget(pTabBar);

            CreateInspector(pTabBar);
        }
    }

    mWidgetArray.push_back(mMainWindowBar_01);
    mWidgetArray.push_back(mMainWindowBar_02);

}
void EditorManager::CreateDebuggerTab(Editor::TabBar* _pSrcTabBar)
{
    {   // 인스펙터 탭
        mDebbugerTab = new Editor::EditorDebugger();
        Editor::TabNode* pDebuggerTab = new Editor::TabNode("Debug");
        pDebuggerTab->AddWidget(mDebbugerTab);
        _pSrcTabBar->AddTab(pDebuggerTab);
    }
}

void EditorManager::CreateInspector(Editor::TabBar* _pSrcTabBar)
{
    {   // 인스펙터 탭
        mInspectorViewer = new Editor::InspectorViewer();
        Editor::TabNode* pInspectorBar = new Editor::TabNode("Inspector");
        pInspectorBar->AddWidget(mInspectorViewer);
        _pSrcTabBar->AddTab(pInspectorBar);
    }
}

void EditorManager::CreateHierarchy(Editor::TabBar* _pSrcTabBar)
{
    {   // 하이라키 탭
        mHierarchyViewer = new Editor::HierarchyViewer();
        Editor::TabNode* pInspectorBar = new Editor::TabNode("Hierarchy");
        pInspectorBar->AddWidget(mHierarchyViewer);
        _pSrcTabBar->AddTab(pInspectorBar);
    }
}

void EditorManager::CreateResourceViewer(Editor::TabBar* _pSrcTabBar)
{
    {   // 리소스 뷰어 탭
        mResourceViewer = new Editor::ResourceViewer();
        Editor::TabNode* pBar = new Editor::TabNode("Resource Viewer");
        pBar->AddWidget(mResourceViewer);
        _pSrcTabBar->AddTab(pBar);
    }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT EditorManager::EditorWinProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(_hwnd, _msg, _wParam, _lParam))
    {
        return true;
    }
    switch (_msg)
    {
    case WM_CREATE:
        DragAcceptFiles(_hwnd, TRUE);
        break;
    case WM_DROPFILES:  // File Drag&Drop
    {
        // 드롭된 파일들에 대한 처리
        HDROP hDrop = (HDROP)_wParam;

        // 드롭된 파일의 개수
        UINT numFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);

        std::vector<std::wstring> pathArr;
        pathArr.reserve(numFiles);

        for (size_t i = 0; i < numFiles; ++i) {
            // 각 파일의 절대경로를 얻음
            wchar_t absPath[MAX_PATH];
            DragQueryFile(hDrop, i, absPath, MAX_PATH);
            //절대 경로를 상대 경로로 변환
            std::wstring relPath;
            if (S_OK == Helper::ABSPath_To_RelativePath(absPath, relPath))
            {
                pathArr.push_back(relPath);
            }
            Display::Console::Log("Drag&Drop File : ", relPath, '\n');
        }
        ProcessDragFile(pathArr);

        // 메모리 해제
        DragFinish(hDrop);
        break;
    }
    case WM_SIZE:
        break;
    case WM_MOVE:
        break;
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
        if (_wParam == VK_MENU)
            return 0;
        break;
    case WM_SETFOCUS:
        break;
    case WM_DESTROY:
        break;
    case WM_CLOSE:
        break;
    default:
        return DefWindowProc(_hwnd, _msg, _wParam, _lParam);
    }
    return 0;
}

void EditorManager::ShowPopUp()
{
    // 이전 위젯을 호버링중일 때 && 마우스 좌클릭을 했을 때
    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        ImGui::SetNextWindowPos(ImGui::GetMousePos());
        ImGui::OpenPopup("WidgetClickMenu"); // 이름을 통일시킴
    }
    // 팝업 메뉴 정의
    if (ImGui::BeginPopup("WidgetClickMenu")) { // OpenPopup의 이름과 일치해야 함
        if (ImGui::MenuItem("Option 1")) {
            // Option 1 선택 시 동작
        }
        if (ImGui::MenuItem("Option 2")) {
            // Option 2 선택 시 동작
        }
        if (ImGui::MenuItem("Option 3")) {
            // Option 3 선택 시 동작
        }
        ImGui::EndPopup();
    }
}


BOOL EditorManager::EditorReposition()
{
    // 에디터 윈도우 위치 조정
    if (mFocusViewport && mEditorViewport)
    {
        // 대상 윈도우의 위치 및 사이즈를 가져와서 에디터 윈도우를 리포지션한다.
        POINT size = mFocusViewport->GetIWindow()->GetSize();
        POINT pos = mFocusViewport->GetIWindow()->GetPosition();
        POINT resPos = { pos.x + size.x + EDITOR_OFFSET , pos.y };
        return mEditorViewport->GetIWindow()->SetPosition(resPos);
    }
    return FALSE;
}

#define REGISTER_AND_ALLOC_RESOUCE(type) \
ResourceHandle handle = { eResourceType::type, fileName, L"", _pathArr[i] };\
ResourceManager::RegisterResourceHandle(handle);\
ResourceManager::Alloc_Resource<type>(handle);\

BOOL EditorManager::ProcessDragFile(std::vector<std::wstring>& _pathArr)
{
    for (size_t i = 0; i < _pathArr.size(); ++i)
    {
        std::wstring fileExt;
        std::wstring fileName;
        Helper::GetExtFromFilePath(_pathArr[i], fileExt);
        Helper::GetFileNameFromFilePath(_pathArr[i], fileName);
        if (fileExt == L".fbx" || fileExt == L".FBX")
        {
            REGISTER_AND_ALLOC_RESOUCE(FBXModelResource);
        }
        if (fileExt == L".png" || fileExt == L".jpg" || fileExt == L".dds" || fileExt == L".tga")
        {
            REGISTER_AND_ALLOC_RESOUCE(Texture2DResource);
        }
        if (fileExt == L".ogg" || fileExt == L".mp3" || fileExt == L".wav")
        {
            REGISTER_AND_ALLOC_RESOUCE(AudioResource);
        }
    }
    return TRUE;
}

void EditorManager::SetGizmoOperation(ImGuizmo::OPERATION operation)
{
    mCurrentGizmoOperation = operation;
}

ImGuizmo::OPERATION EditorManager::GetGizmoOperation()
{
    return mCurrentGizmoOperation;
}
