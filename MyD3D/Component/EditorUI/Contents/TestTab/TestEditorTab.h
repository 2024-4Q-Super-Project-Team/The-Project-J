#pragma once
#include "Component/EditorUI/Contents/EditorWidget.h"
#include "Component/EditorUI/Contents/EditorUID.h"

class Object;
class Light;
class Camera;
class RendererComponent;
class MeshRenderer;
class SkinnedMeshRenderer;

namespace Editor
{
    class Inspector;

    class TestEditorTab : public Widget, public EUID
    {
    public:
        explicit TestEditorTab();
        virtual ~TestEditorTab() = default;
    public:
        virtual void Render() override;
    public:
        void SetCamera(Camera* _camera);
        void AddLight(Light* _light);
        void AddRenderModel(RendererComponent* _model);
    public:
        void SetFocusInspector(Inspector* _pInspector);
    private:
        Camera*                          mCamera;
        std::vector<Light*>              mLights;
        std::vector<RendererComponent*>  mModels;
        // 참조하는 인스펙터 창.
        Inspector* mFocusInspector;
    };
}