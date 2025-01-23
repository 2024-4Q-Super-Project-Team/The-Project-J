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
    virtual void _CALLBACK OnWindowMessage(ViewportScene* _pViewport, UINT _msg, WPARAM _wParam, LPARAM _lParam) override final;

public:
    static ViewportScene* mMainScene;
    static Display::IWindow* mGameWindow;
};

