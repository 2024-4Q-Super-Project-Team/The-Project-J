#pragma once

class ViewportScene;

class GameApp : public Application
{
public:
    using Application::Application;
protected:
    virtual BOOL OnPreInitialize() override final;
    virtual BOOL OnPostInitialize() override final;
    virtual void OnPreFinalization() override final;
    virtual void OnPostFinalization() override final;
public:
    static ViewportScene* mMainScene;
    static ViewportScene* mEditorScene;
    static void EditorRePosition();
};

