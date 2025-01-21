#pragma once
#include "Editor/EditorCamera/EditorCamera.h"

#include "Editor/CumstomWidget/TabItem/EditorDebug/EditorDebugger.h"

#include "Editor/CumstomWidget/TabItem/Viewer/HierarchyViewer/EditorHierarchyViewer.h"
#include "Editor/CumstomWidget/TabItem/Viewer/InspectorViewer/EditorInspectorViewer.h"
#include "Editor/CumstomWidget/TabItem/Viewer/ResourceViewer/EditorResourceViewer.h"

#include "Editor/CumstomWidget/MenuItem/File/EditorJsonLoader.h"
#include "Editor/CumstomWidget/MenuItem/File/EditorJsonWriter.h"

#include "Editor/CumstomWidget/MenuItem/World/EditorWorldChanger.h"
#include "Editor/CumstomWidget/MenuItem/World/EditorWorldCreator.h"
#include "Editor/CumstomWidget/MenuItem/World/EditorWorldRemover.h"

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
    static void RenderEditorWindow();
public:
    static void EditorUpdate();
    static void EditorRender();
public:
    static BOOL ShowEditorWindow(ViewportScene* _targetViewport);
    static BOOL IsRenderView(ViewportScene* _targetViewport);
    static BOOL IsFocusView(ViewportScene* _targetViewport);
    static BOOL EditorReposition();
private:
    static void InitImGui();
    static void InitMainMenuBar();
    static void InitMainWindow();
    static void /* */CreateDebuggerTab(Editor::TabBar* _pSrcTabBar);
    static void /* */CreateInspector(Editor::TabBar* _pSrcTabBar);
    static void /* */CreateHierarchy(Editor::TabBar* _pSrcTabBar);
    static void /* */CreateResourceViewer(Editor::TabBar* _pSrcTabBar);
public:
    void UpdateIO();
public:
    static inline auto GetFocusViewport()   { return mFocusViewport; }
    static inline auto GetEditorViewport()  { return mEditorViewport; }
    static inline auto GetResourceViewer()  { return mResourceViewer; }
    static inline auto GetHierarchyViewer() { return mHierarchyViewer; }
    static inline auto GetInspectorViewer() { return mInspectorViewer; }
public:
    static EditorCamera                     mEditorCamera;

    static ViewportScene*                   mFocusViewport;
    static ViewportScene*                   mEditorViewport;
    static std::vector<Editor::IWidget*>    mWidgetArray;

    static Editor::EditorDebugger*          mDebbugerTab;
    static Editor::ResourceViewer*          mResourceViewer;
    static Editor::HierarchyViewer*         mHierarchyViewer;
    static Editor::InspectorViewer*         mInspectorViewer;

    static std::vector<std::shared_ptr<Resource>> mResourceContainor;
    /////////////////////////////////////////////////////
    //  Widget Object
    /////////////////////////////////////////////////////
	static Editor::MenuBar*                 mMainMenuBar;
	static Editor::WindowBar*               mMainWindowBar_01;
	static Editor::WindowBar*               mMainWindowBar_02;
    /////////////////////////////////////////////////////
	//  ImGui Object
    /////////////////////////////////////////////////////
	static ImGuiContext* mContext;

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

template <typename T>
struct SerialData : public Serial
{
    T val;

    SerialData(std::string_view _name, MonoBehaviour* mono)
        : Serial(_name)
    {
        mono->AddField(this);
        if (std::is_same<T, Vector3>::value)
            widget = new Editor::InputVector3(_name.data(), &val);

    }
};

#ifdef _DEBUG
#define SerializeField(Type, Name)\
	SerialData<Type> Name##Data = SerialData<Type>(#Name, this);\
	Type Name
#else
#define SerializeField(Type, Name)\
	Type Name
#endif