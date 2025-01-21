#pragma once

class UISystem
{
public:
    UISystem();
    ~UISystem();
public:
    void Start();
    void Tick();
    void FixedUpdate();
    void PreUpdate();
    void Update();
    void PostUpdate();
    void PreRender();
    void Render();
    void Draw(Camera* _pCamera);
    void PostRender();

public:
    std::vector<Widget*> GetWidgets() { return mWidgetContainer; };
    void AddWidget(Widget* _pWidget) {};

private:
    std::vector<Widget*> mWidgetContainer;

};