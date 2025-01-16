#pragma once
#include "Editor/Viewer/HierarchyViewer/Hierarchy.h"
#include "Editor/Viewer/InspectorViewer/Inspector.h"
#include "Editor/Viewer/ResourceViewer/EditorResourceView.h"

#include "Widgets/Core/EditorTab.h"
#include "Widgets/Core/EditorWindow.h"
#include "Widgets/Core/EditorMenu.h"

#include "Widgets/Extra/EditorCheckBox.h"
#include "Widgets/Extra/EditorInputVector.h"
#include "Widgets/Extra/EditorStaticText.h"

#define EDITOR_WIDTH 800
#define EDITOR_OFFSET 10

class EditorManager
{
public:
	static void Initialize();
	static void Finalization();
    static void RenderEditor();
public:
    static BOOL ShowEditorWindow(ViewportScene* _targetViewport);
    static BOOL IsRenderView(ViewportScene* _targetViewport);
    static BOOL IsForcusView(ViewportScene* _targetViewport);
    static BOOL EditorReposition();
private:
    static void InitImGui();
    static void InitMainMenuBar();
    static void InitMainWindow();
    static void /* */CreateTestTab(Editor::TabBar* _pSrcTabBar);
    static void /* */CreateInspector(Editor::TabBar* _pSrcTabBar);
    static void /* */CreateHierarchy(Editor::TabBar* _pSrcTabBar);
    static void /* */CreateResourceViewer(Editor::TabBar* _pSrcTabBar);
private:
    static ViewportScene*               mForcusViewport;
    static ViewportScene*               mEditorViewport;
    static std::vector<Editor::IWidget*> mWidgetArray;

    static Editor::EditorResourceView*  mResourceViewer;
    static Editor::Hierarchy*           mHierarchyViewer;
    static Editor::Inspector*           mInspectorViewer;

    /////////////////////////////////////////////////////
    //  Widget Object
    /////////////////////////////////////////////////////
	static Editor::MenuBar*     mMainMenuBar;
	static Editor::WindowBar*   mMainWindowBar_01;
	static Editor::WindowBar*   mMainWindowBar_02;

    /////////////////////////////////////////////////////
	//  ImGui Object
    /////////////////////////////////////////////////////
	static ImGuiContext* mContext;

    static LRESULT CALLBACK EditorWinProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
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