#include "pch.h"
#include "EditorDebugger.h"
#include "Editor/EditorManager.h"

Editor::EditorDebugger::EditorDebugger()
{
}

void Editor::EditorDebugger::Render()
{
    std::string str;
    str = std::to_string(Time::GetFps());
    ImGui::Text("%s", str.c_str());
    ImGui::Text("\nDisplay Memory");
    D3DGraphicsDevice::GetDisplayMemoryInfo(str);
    ImGui::Text("%s", str.c_str());
    ImGui::Text("\nProcess Memory");
    D3DGraphicsDevice::GetVirtualMemoryInfo(str);
    ImGui::Text("%s", str.c_str());

    ImGui::Text("\nDebug Draw Mode");
    ImGui::Checkbox("##debugdraw", &Collider::bDrawMode);

    EditorManager::mEditorCamera.EditorRendering(EditorViewerType::DEFAULT);
}
