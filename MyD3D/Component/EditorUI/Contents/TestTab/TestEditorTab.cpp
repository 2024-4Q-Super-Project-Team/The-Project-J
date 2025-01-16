#include "pch.h"
#include "TestEditorTab.h"
#include "Object/Object.h"
#include "Component/Camera/Camera.h"
#include "Component/Light/Light.h"
#include "Component/Renderer/MeshRenderer/MeshRenderer.h"
#include "Component/Renderer/SkinnedMeshRenderer/SkinnedMeshRenderer.h"

#include "Manager/GameManager.h"
#include "ViewportScene/ViewportScene.h"

namespace Editor
{
    TestEditorTab::TestEditorTab()
        : EUID("Debug")
        , mFocusInspector(nullptr)
    {
    }

    void TestEditorTab::Render()
    {
        std::string str = "FPS : ";
        str += std::to_string(Time::GetFps());
        ImGui::Text("%s", str.c_str());
        ImGui::Text("\nDisplay Memory");
        D3DGraphicsDevice::GetDisplayMemoryInfo(str);
        ImGui::Text("%s", str.c_str());
        ImGui::Text("\nProcess Memory");
        D3DGraphicsDevice::GetVirtualMemoryInfo(str);
        ImGui::Text("%s", str.c_str());

        if (mCamera)
        {
            mCamera->EditorRendering();
        }

        for (auto light : mLights)
        {
            ImGui::NewLine();
            ImGui::Separator();
            light->gameObject->EditorRendering();
            light->EditorRendering();
        }

        for (auto model : mModels)
        {
            ImGui::NewLine();
            ImGui::Separator();
            model->gameObject->EditorRendering();
            model->EditorRendering();
        }
    }

    void TestEditorTab::SetCamera(Camera* _camera)
    {
        mCamera = _camera;
    }

    void TestEditorTab::AddLight(Light* _light)
    {
        mLights.push_back(_light);
    }

    void TestEditorTab::AddRenderModel(RendererComponent* _renderer)
    {
        mModels.push_back(_renderer);
    }

    void TestEditorTab::SetFocusInspector(Inspector* _pInspector)
    {
        mFocusInspector = _pInspector;
    }

}
