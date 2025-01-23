#pragma once

class Widget;
class Camera;

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
    
    template <class T>
    T* AddWidget(std::wstring _id);

    // TODO : find 함수 만들기
    // auto FindWidget(std::wstring _id);
private:
    std::vector<Widget*> mWidgetContainer;

};

template<class T>
T* UISystem::AddWidget(std::wstring _id)
{
    static_assert(std::is_base_of<Widget, T>::value, "AddWidget_Fail");
    T* widget = new T();
    widget->SetID(_id);
    widget->SetOwner(this);
    mWidgetContainer.push_back(widget);

    return widget;
}
