#pragma once
#include "Component/Component.h"
#include "Component/EditorUI/Contents/Menu/EditorMenuBar.h"
#include "Component/EditorUI/Contents/Tab/EditorTab.h"
#include "Component/EditorUI/Contents/Window/EditorWindow.h"
#include "Component/EditorUI/Contents/CollapsingHeader/EditorCollapsingHeader.h"

#include "Component/EditorUI/Contents/TestTab/TestEditorTab.h"
#include "Component/EditorUI/Contents/Inspector/Inspector.h"
#include "Component/EditorUI/Contents/Hierarchy/Hierarchy.h"
#include "Component/EditorUI/Contents/GameEditor/GameEditor.h"
#include "Editor/ResourceTab/EditorResourceView.h"

#include "Component/EditorUI/Contents/Widget/EditorCheckBox.h"
#include "Component/EditorUI/Contents/Widget/EditorInputVector.h"
#include "Component/EditorUI/Contents/Widget/EditorStaticText.h"

class GraphicsManager;
class WorldManager;
class ViewportScene;

namespace Editor
{
    class Widget;
}

// 비트 플래그 정의
#define EDITOR_RENDERING_MODE_WHITE         0x1
#define EDITOR_RENDERING_MODE_AUTOSIZE      0x2
#define EDITOR_RENDERING_MODE_ALL           0xFFFFFFFF

namespace Display
{
    class IWindow;
}
class EditorUI
    : public Component
{
public:
    explicit EditorUI(Object* _owner);
    virtual ~EditorUI();
public:
    static std::string GetUID() { 
        static UINT UID = 0; 
        return std::to_string(UID++);
    }
    static UINT numEditor;
    static UINT cnt;
public:
    virtual void Start() override;
    virtual void Tick() override;
    virtual void FixedUpdate() override;
    virtual void PreUpdate() override;
    virtual void Update() override;
    virtual void PostUpdate() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void Draw(Camera* _camera) override;
    virtual void PostRender() override;
public:
    // RenderingMode 설정 및 확인 메서드
    inline void SetRenderingMode(UINT flags) { RenderingMode = flags; }
    inline void AddRenderingMode(UINT flags) { RenderingMode |= flags; }
    inline void RemoveRenderingMode(UINT flags) { RenderingMode &= ~flags; }
    inline bool IsRenderingModeEnabled(UINT flag) const { return (RenderingMode & flag) != 0; }
public:
    void AddWidget(Editor::Widget* _pWidget);
    // 에디터를 출력할 윈도우
    void SetFocusWindow(Display::IWindow* _pWindow);
    inline Display::IWindow* GetWindow()    { return mWindow; }

public:
    virtual json Serialize() override;
    virtual void Deserialize( json& j) override;
private:
    std::vector<Editor::Widget*> mWidgetContainer;
private:
    ImGuiContext*       mContext;
    ImDrawData*         mDrawData;
    Display::IWindow*   mWindow;
    
    UINT                RenderingMode;
private:
    void UpdateIO();
    BOOL SetUp();
    void CleanUp();
};

