#include "pch.h"
#include "EditorManager.h"
#include "ViewportScene/ViewportManager.h"
#include "Manager/GameManager.h"
#include "ViewportScene/ViewportScene.h"


ViewportScene*                  EditorManager::mForcusViewport = nullptr;
ViewportScene*                  EditorManager::mEditorViewport = nullptr;

std::vector<Editor::IWidget*>   EditorManager::mWidgetArray;
Editor::EditorResourceView*     EditorManager::mResourceViewer = nullptr;
Editor::Hierarchy*              EditorManager::mHierarchyViewer = nullptr;
Editor::Inspector*              EditorManager::mInspectorViewer = nullptr;

Editor::MenuBar*                EditorManager::mMainMenuBar = nullptr;
Editor::WindowBar*              EditorManager::mMainWindowBar_01 = nullptr;
Editor::WindowBar*              EditorManager::mMainWindowBar_02 = nullptr;

ImGuiContext*                   EditorManager::mContext;

void EditorManager::Initialize()
{
	mResourceViewer = new Editor::EditorResourceView();
	mHierarchyViewer = new Editor::Hierarchy();
	mInspectorViewer = new Editor::Inspector();
    
    InitMainMenuBar();
    InitMainWindow();

    // ���ؽ�Ʈ Ȱ��ȭ
    ImGui::SetCurrentContext(mContext);
}

void EditorManager::Finalization()
{
}

void EditorManager::RenderEditor()
{
    if (mForcusViewport && mEditorViewport)
    {  
        // ���ؽ�Ʈ Ȱ��ȭ
        ImGui::SetCurrentContext(mContext);
        // IO ���� ������Ʈ

        // ImGui ������ ����
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // ���� ������
        for (auto& widget : mWidgetArray)
        {
            widget->Render();
        }

        EditorDragNDrop::Render();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}

BOOL EditorManager::ShowEditorWindow(ViewportScene* _targetViewport)
{
    if (_targetViewport)
    {
        mForcusViewport = _targetViewport;
		Display::IWindow* mDestWindow = mForcusViewport->GetIWindow();
        POINT Destsize = mForcusViewport->GetIWindow()->GetSize();
        POINT Destpos = mForcusViewport->GetIWindow()->GetPosition();
        
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
        winDecs.WndClass.lpfnWndProc = EditorWinProc;
        winDecs.WndParent = mDestWindow;
        mEditorViewport = ViewportManager::CreateViewportScene(&winDecs);

        EditorReposition();

        InitImGui();

        return TRUE;
    }
    return FALSE;
}

BOOL EditorManager::IsRenderView(ViewportScene* _targetViewport)
{
    return mEditorViewport == _targetViewport;
}

BOOL EditorManager::IsForcusView(ViewportScene* _targetViewport)
{
    return mForcusViewport == _targetViewport;
}

void EditorManager::InitImGui()
{
    // ImGui ���ؽ�Ʈ ����
    IMGUI_CHECKVERSION();
    mContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(mContext);
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
    io.IniFilename = nullptr;
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesKorean());
    // ImGui �ʱ�ȭ
    ImGui_ImplWin32_Init(mEditorViewport->GetIWindow()->GetHandle());
    ImGui_ImplDX11_Init(D3DGraphicsDevice::GetDevice(), D3DGraphicsRenderer::GetDevicecontext());
}

void EditorManager::InitMainMenuBar()
{
    // �޴��� �� �޴�, �޴������� ����
    mMainMenuBar = new Editor::MenuBar();

    Editor::MenuNode* pMenu_01 = mMainMenuBar->AddMenuNode("Menu");
    Editor::MenuNode* pMenu_02 = mMainMenuBar->AddMenuNode("System");
    Editor::MenuNode* pMenu_03 = mMainMenuBar->AddMenuNode("Option");

    pMenu_01->AddMenuItem("Ctrl + 1", []() {Display::Console::Log("click!");});
    pMenu_01->AddMenuItem("Ctrl + 2", []() {Display::Console::Log("click!");});
    pMenu_01->AddMenuItem("Ctrl + 3", []() {Display::Console::Log("click!");});
    pMenu_01->AddMenuItem("Ctrl + 4", []() {Display::Console::Log("click!");});

    pMenu_02->AddMenuItem("Ctrl + 1", []() {Display::Console::Log("click!");});
    pMenu_02->AddMenuItem("Ctrl + 2");
    pMenu_02->AddMenuItem("Ctrl + 3");
    pMenu_02->AddMenuItem("Ctrl + 4");

	mWidgetArray.push_back(mMainMenuBar);
}

void EditorManager::InitMainWindow()
{
    {
        mMainWindowBar_01 = new Editor::WindowBar("Editor_Bar_01", Vector2(EDITOR_WIDTH / 2, 0));
        mMainWindowBar_01->AddFlags(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        {
            // �ǹ� ����, �ν�����
            Editor::TabBar* pTabBar = new Editor::TabBar("TabBar_01");
            mMainWindowBar_01->AddWidget(pTabBar);

            CreateTestTab(pTabBar);
            CreateHierarchy(pTabBar);
            CreateResourceViewer(pTabBar);
        }
    }
    {
        mMainWindowBar_02 = new Editor::WindowBar("Editor_Bar_02", Vector2(EDITOR_WIDTH / 2, 0), Vector2(EDITOR_WIDTH / 2, 0));
        mMainWindowBar_02->AddFlags(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        {
            // �ǹ� ����, ���̶�Ű
            Editor::TabBar* pTabBar = new Editor::TabBar("TabBar_02");
            mMainWindowBar_02->AddWidget(pTabBar);

            CreateInspector(pTabBar);
        }
    }

	mWidgetArray.push_back(mMainWindowBar_01);
	mWidgetArray.push_back(mMainWindowBar_02);

}
void EditorManager::CreateTestTab(Editor::TabBar* _pSrcTabBar)
{
    //{   // �׽�Ʈ�� ��
    //    mDebugEditor = new Editor::TestEditorTab();
    //    Editor::Tab* pInspectorBar = new Editor::Tab("Debug");
    //    pInspectorBar->AddWidget(mDebugEditor);
    //    _pSrcTabBar->AddTab(pInspectorBar);
    //}
}

void EditorManager::CreateInspector(Editor::TabBar* _pSrcTabBar)
{
    {   // �ν����� ��
        mInspectorViewer = new Editor::Inspector();
        Editor::TabNode* pInspectorBar = new Editor::TabNode("Inspector");
        pInspectorBar->AddWidget(mInspectorViewer);
        _pSrcTabBar->AddTab(pInspectorBar);
    }
}

void EditorManager::CreateHierarchy(Editor::TabBar* _pSrcTabBar)
{
    {   // ���̶�Ű ��
        mHierarchyViewer = new Editor::Hierarchy();
        Editor::TabNode* pInspectorBar = new Editor::TabNode("Hierarchy");
        pInspectorBar->AddWidget(mHierarchyViewer);
        _pSrcTabBar->AddTab(pInspectorBar);
    }
}

void EditorManager::CreateResourceViewer(Editor::TabBar* _pSrcTabBar)
{
    {   // ���ҽ� ��� ��
        mResourceViewer = new Editor::EditorResourceView();
        Editor::TabNode* pBar = new Editor::TabNode("Resource Viewer");
        pBar->AddWidget(mResourceViewer);
        _pSrcTabBar->AddTab(pBar);
    }
}

void EditorManager::UpdateIO()
{
    HWND editorHwnd = mEditorViewport->GetIWindow()->GetHandle();
    ImGuiIO& io = ImGui::GetIO();
    {   // ������ �������� ���콺 ��ǥ�� ��ȯ
        POINT mousePos;
        GetCursorPos(&mousePos);
        ScreenToClient(editorHwnd, &mousePos);
        io.MousePos = ImVec2(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
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
        break;
    case WM_SIZE:
        break;
    case WM_MOVE:
		if (mForcusViewport) EditorReposition();
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
    // ���� ������ ȣ�������� �� && ���콺 ��Ŭ���� ���� ��
    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        ImGui::SetNextWindowPos(ImGui::GetMousePos());
        ImGui::OpenPopup("WidgetClickMenu"); // �̸��� ���Ͻ�Ŵ
    }
    // �˾� �޴� ����
    if (ImGui::BeginPopup("WidgetClickMenu")) { // OpenPopup�� �̸��� ��ġ�ؾ� ��
        if (ImGui::MenuItem("Option 1")) {
            // Option 1 ���� �� ����
        }
        if (ImGui::MenuItem("Option 2")) {
            // Option 2 ���� �� ����
        }
        if (ImGui::MenuItem("Option 3")) {
            // Option 3 ���� �� ����
        }
        ImGui::EndPopup();
    }
}


BOOL EditorManager::EditorReposition()
{
	// ������ ������ ��ġ ����
    if (mForcusViewport)
    {
        // ��� �������� ��ġ �� ����� �����ͼ� ������ �����츦 ���������Ѵ�.
        POINT size = mForcusViewport->GetIWindow()->GetSize();
        POINT pos = mForcusViewport->GetIWindow()->GetPosition();
        POINT resPos = { pos.x + size.x + EDITOR_OFFSET , pos.y };
        return mEditorViewport->GetIWindow()->SetPosition(resPos);
    }
    return FALSE;
}
