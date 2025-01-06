#include "pch.h"
#include "TestEditorTab.h"
#include "Object/Object.h"
#include "Component/Camera/Camera.h"
#include "Component/Light/Light.h"
#include "Component/Renderer/MeshRenderer/MeshRenderer.h"
#include "Component/Renderer/SkinnedMeshRenderer/SkinnedMeshRenderer.h"

namespace Editor
{
    TestEditorTab::TestEditorTab()
        : EUID("Debug")
        , mFocusInspector(nullptr)
    {
    }

    void TestEditorTab::Render()
    {
        if (mCamera)
        {
            mCamera->EditorRendering();
        }
        ImGui::Separator();
        for (auto light : mLights)
        {
            light->gameObject->EditorRendering();
            light->EditorRendering();
        }
        for (auto model : mModels)
        {
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
