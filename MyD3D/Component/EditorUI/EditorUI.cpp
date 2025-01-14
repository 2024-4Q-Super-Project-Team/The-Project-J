#include "pch.h"
#include "EditorUI.h"
#include "ViewportScene/ViewportManager.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"

#include "ViewportScene/ViewportScene.h"

UINT EditorUI::numEditor = 0;
UINT EditorUI::cnt = 0;

EditorUI::EditorUI(Object* _owner)
    : Component(_owner)
{
    mType = eComponentType::EDITOR_UI;
    numEditor++;
}

EditorUI::~EditorUI()
{
    CleanUp();
    numEditor--;
}

void EditorUI::Start()
{
}

void EditorUI::Tick()
{
}

void EditorUI::FixedUpdate()
{
}

void EditorUI::PreUpdate()
{
}

void EditorUI::Update()
{
}

void EditorUI::PostUpdate()
{
}

void EditorUI::PreRender()
{
}

void EditorUI::Render()
{
    if (mWindow)
    {
        // 컨텍스트 활성화
        ImGui::SetCurrentContext(mContext);

        // IO 상태 업데이트
        UpdateIO();

        // ImGui 프레임 시작
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // 위젯 렌더링
        for (auto& widget : mWidgetContainer)
        {
            widget->Render();
        }

        cnt++;

        // ImGui 렌더링 종료
        ImGui::EndFrame();
        ImGui::Render();
        mDrawData = ImGui::GetDrawData();
        ImGui_ImplDX11_RenderDrawData(mDrawData);
    }
}

void EditorUI::Draw(Camera* _camera)
{
   
}

void EditorUI::PostRender()
{
    
}

void EditorUI::SetFocusWindow(Display::IWindow* _pWindow)
{
    if (_pWindow)
    {
        // 동일한 윈도우 핸들인지 확인
        if (_pWindow == mWindow)
            return;

        // 기존 윈도우 핸들에 대한 클린업
        if (mWindow)
        {
            CleanUp();
        }

        // 새로운 윈도우 핸들 설정 및 컨텍스트 초기화
        mWindow = _pWindow;
        SetUp();
    }
}

json EditorUI::Serialize()
{
    return json();
}

void EditorUI::Deserialize( json& j)
{
}


void EditorUI::AddWidget(Editor::Widget* _pWidget)
{
    mWidgetContainer.push_back(_pWidget);
}

void UpdateMousePositionForWindow(HWND windowHandle)
{
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(windowHandle, &mousePos); // 현재 윈도우의 클라이언트 좌표로 변환

    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}

void EditorUI::UpdateIO()
{
    ImGui::SetCurrentContext(mContext);

    ImGuiIO& io = ImGui::GetIO();
    // 기본 입력 비활성화
    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableSetMousePos;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouse;

    HWND focusHwnd = GetForegroundWindow(); // 현재 포커스된 윈도우 확인
    if (focusHwnd == mWindow->GetHandle()) // 현재 윈도우와 일치할 경우
    {
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
        io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;

        // 마우스 위치 업데이트
        UpdateMousePositionForWindow(mWindow->GetHandle());
    }
    else
    {
        io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX); // 포커스가 없는 경우 마우스 입력 비활성화
    }
}

BOOL EditorUI::SetUp()
{
    ID3D11Device* pDevice = D3DGraphicsDevice::GetDevice();
    ID3D11DeviceContext* pDeviceContext = D3DGraphicsRenderer::GetDevicecontext();

    if (pDevice && pDeviceContext)
    {
        // ImGui 컨텍스트 생성
        IMGUI_CHECKVERSION();
        mContext = ImGui::CreateContext();
        ImGui::SetCurrentContext(mContext);
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr;
        io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesKorean());
        // ImGui 초기화
        ImGui_ImplWin32_Init(mWindow->GetHandle());
        ImGui_ImplDX11_Init(pDevice, pDeviceContext);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void EditorUI::CleanUp()
{
    if (mContext)
    {
        ImGui::SetCurrentContext(mContext);
        ImGui::DestroyContext(mContext); // 고유 컨텍스트 삭제
        mContext = nullptr;
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    mWindow = nullptr;
}
