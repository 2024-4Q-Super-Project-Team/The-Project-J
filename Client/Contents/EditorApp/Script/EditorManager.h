#pragma once


class EditorManager : public MonoBehaviour
{
public:
    using MonoBehaviour::MonoBehaviour;
    virtual ~EditorManager();
public:
    virtual void Start() override;
    virtual void Tick() override;
    virtual void FixedUpdate() override;
    virtual void PreUpdate() override;
    virtual void Update() override;
    virtual void PostUpdate() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
private:
    void InitializeEditorWidget();
    void CreateMainMenuBar();
    void CreateMainWindowContainer();
    void /* */CreateTestTab(Editor::TabBar* _pSrcTabBar);
    void /* */CreateInspector(Editor::TabBar* _pSrcTabBar);
    void /* */CreateHierarchy(Editor::TabBar* _pSrcTabBar);
    void /* */CreateGameEditor(Editor::TabBar* _pSrcTabBar);

public:
    virtual json Serialize() override { return json(); }
    virtual json Deserialize() override { return json(); }
private:
    EditorUI* mEditor;
    ViewportScene* mEditorViewport;
    ViewportScene* mGameViewport;
public:
    static Editor::TestEditorTab*   mDebugEditor;
    static Editor::Inspector*       mInspector;
    static Editor::Hierarchy*       mHierarchy;
    static Editor::GameEditor*       mGameEditor;
public:
    static inline auto GetInspector() { return mInspector; }
};

