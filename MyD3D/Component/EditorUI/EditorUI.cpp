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
        // ���ؽ�Ʈ Ȱ��ȭ
        ImGui::SetCurrentContext(mContext);

        // IO ���� ������Ʈ
        UpdateIO();

        // ImGui ������ ����
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // ���� ������
        for (auto& widget : mWidgetContainer)
        {
            widget->Render();
        }

        cnt++;

        // ImGui ������ ����
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
        // ������ ������ �ڵ����� Ȯ��
        if (_pWindow == mWindow)
            return;

        // ���� ������ �ڵ鿡 ���� Ŭ����
        if (mWindow)
        {
            CleanUp();
        }

        // ���ο� ������ �ڵ� ���� �� ���ؽ�Ʈ �ʱ�ȭ
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
    ScreenToClient(windowHandle, &mousePos); // ���� �������� Ŭ���̾�Ʈ ��ǥ�� ��ȯ

    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}

void EditorUI::UpdateIO()
{
    ImGui::SetCurrentContext(mContext);

    ImGuiIO& io = ImGui::GetIO();
    // �⺻ �Է� ��Ȱ��ȭ
    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableSetMousePos;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouse;

    HWND focusHwnd = GetForegroundWindow(); // ���� ��Ŀ���� ������ Ȯ��
    if (focusHwnd == mWindow->GetHandle()) // ���� ������� ��ġ�� ���
    {
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
        io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;

        // ���콺 ��ġ ������Ʈ
        UpdateMousePositionForWindow(mWindow->GetHandle());
    }
    else
    {
        io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX); // ��Ŀ���� ���� ��� ���콺 �Է� ��Ȱ��ȭ
    }
}

BOOL EditorUI::SetUp()
{
    ID3D11Device* pDevice = D3DGraphicsDevice::GetDevice();
    ID3D11DeviceContext* pDeviceContext = D3DGraphicsRenderer::GetDevicecontext();

    if (pDevice && pDeviceContext)
    {
        // ImGui ���ؽ�Ʈ ����
        IMGUI_CHECKVERSION();
        mContext = ImGui::CreateContext();
        ImGui::SetCurrentContext(mContext);
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr;
        io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesKorean());
        // ImGui �ʱ�ȭ
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
        ImGui::DestroyContext(mContext); // ���� ���ؽ�Ʈ ����
        mContext = nullptr;
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    mWindow = nullptr;
}
