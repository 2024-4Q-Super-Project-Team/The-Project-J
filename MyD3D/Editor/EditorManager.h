#pragma once
#include "Editor/EditorCamera/EditorCamera.h"

#include "Editor/CumstomWidget/GuizmoItem/GuizmoHandler.h"

#include "Editor/CumstomWidget/TabItem/EditorDebug/EditorDebugger.h"

#include "Editor/CumstomWidget/TabItem/Viewer/HierarchyViewer/EditorHierarchyViewer.h"
#include "Editor/CumstomWidget/TabItem/Viewer/InspectorViewer/EditorInspectorViewer.h"
#include "Editor/CumstomWidget/TabItem/Viewer/ResourceViewer/EditorResourceViewer.h"

#include "Editor/CumstomWidget/MenuItem/File/EditorJsonLoader.h"
#include "Editor/CumstomWidget/MenuItem/File/EditorJsonWriter.h"

#include "Editor/CumstomWidget/MenuItem/World/EditorWorldChanger.h"
#include "Editor/CumstomWidget/MenuItem/World/EditorWorldCreator.h"
#include "Editor/CumstomWidget/MenuItem/World/EditorWorldRemover.h"
#include "Editor/CumstomWidget/MenuItem/World/EditorStartWorldSetter.h"

#include "Widgets/Core/EditorTab.h"
#include "Widgets/Core/EditorWindow.h"
#include "Widgets/Core/EditorMenu.h"

#include "Widgets/Extra/EditorCheckBox.h"
#include "Widgets/Extra/EditorInputVector.h"
#include "Widgets/Extra/EditorStaticText.h"

#include "Editor/Handler/EditorDragNDrop.h"

#define EDITOR_WIDTH 800
#define EDITOR_OFFSET 10

class EditorManager
{
public:
    static void Initialize();
    static void Finalization();
    static void Render(); // 게임모드의 렌더
    static void EditorUpdate();
    static void EditorRender(); // 에디터모드의 렌더
    static void EditorWindowRender();
    static void FocusWindowRender();
public:
    static BOOL ShowEditorWindow(ViewportScene* _targetViewport);
    static BOOL IsRenderView(ViewportScene* _targetViewport);
    static BOOL IsFocusView(ViewportScene* _targetViewport);
    static BOOL EditorReposition();
private:
    static void InitImGui(ImGuiContext* _context, HWND _hwnd);
    static void InitEditorImGui();
    static void InitFocusImGui();
    static void InitMainMenuBar();
    static void InitMainWindow();
    static void /* */CreateDebuggerTab(Editor::TabBar* _pSrcTabBar);
    static void /* */CreateInspector(Editor::TabBar* _pSrcTabBar);
    static void /* */CreateHierarchy(Editor::TabBar* _pSrcTabBar);
    static void /* */CreateResourceViewer(Editor::TabBar* _pSrcTabBar);
public:
    static void UpdateIO(ImGuiContext* _context);
public:
    static inline auto GetFocusViewport() { return mFocusViewport; }
    static inline auto GetEditorViewport() { return mEditorViewport; }
    static inline auto GetResourceViewer() { return mResourceViewer; }
    static inline auto GetHierarchyViewer() { return mHierarchyViewer; }
    static inline auto GetInspectorViewer() { return mInspectorViewer; }
public:
    static EditorCamera                             mEditorCamera;
    static ViewportScene*                           mFocusViewport;
    static ViewportScene*                           mEditorViewport;
    static std::vector<std::shared_ptr<Resource>>   mResourceContainor;
    /////////////////////////////////////////////////////
    //  Widget Object
    /////////////////////////////////////////////////////
    static std::vector<Editor::IWidget*>    mWidgetArray;

    static Editor::MenuBar*         mMainMenuBar;
    static Editor::WindowBar*       mMainWindowBar_01;
    static Editor::WindowBar*       mMainWindowBar_02;

    static Editor::GuizmoHandler*   mGuizmoHandler;

    static Editor::EditorDebugger*  mDebbugerTab;
    static Editor::ResourceViewer*  mResourceViewer;
    static Editor::HierarchyViewer* mHierarchyViewer;
    static Editor::InspectorViewer* mInspectorViewer;
    /////////////////////////////////////////////////////
    //  ImGui Object
    /////////////////////////////////////////////////////
    static ImGuiContext* mFocusContext;
    static ImGuiContext* mEditorContext;

    static LRESULT CALLBACK EditorWinProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
public:
    static void ShowPopUp();
};

struct Serial {
    Serial(std::string_view _key)
        : key(_key) {
    }
    std::string key;
    Editor::IWidget* widget = nullptr;
};
// 기본 템플릿 구조체 정의
template <typename T>
struct SerialData : public Serial {
public:
    T val;
    SerialData(std::string_view _name, MonoBehaviour* mono, T _initial)
        : Serial(_name), val(_initial)
    { mono->AddField(this); }
};

// 매크로 정의
#define SERIALDATA_TEMPLATE(Type, Class)         \
template <>                                      \
class SerialData<Type> : public Serial {         \
public:                                          \
    Type val;                                    \
    SerialData(std::string_view _name, MonoBehaviour* mono, Type _initial) \
        : Serial(_name), val(_initial) {         \
        mono->AddField(this);                    \
        widget = new Class(_name.data(), &val);  \
    }                                            \
};

SERIALDATA_TEMPLATE(bool, Editor::CheckBox);
SERIALDATA_TEMPLATE(INT, Editor::DragInt);
SERIALDATA_TEMPLATE(FLOAT, Editor::DragFloat);
SERIALDATA_TEMPLATE(Vector2, Editor::DragVector2);
SERIALDATA_TEMPLATE(Vector3, Editor::DragVector3);
SERIALDATA_TEMPLATE(Vector4, Editor::DragVector4);

#ifdef _DEBUG
#define SerializeField(Type, Name, Init)\
	SerialData<Type> Name## = SerialData<Type>(#Name, this, Init);
#else
#define SerializeField(Type, Name)\
	Type Name
#endif