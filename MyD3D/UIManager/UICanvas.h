#pragma once

#include "UIManager/Widget/WidgetImage.h"
#include "UIManager/Widget/WidgetText.h"

#include "Component/Component.h"
#include "World/World.h"

class UICanvas
    : public Component
{
public:
    explicit UICanvas(Object* _owner);
    virtual ~UICanvas();
public:
    void PushDrawList();
private:
    // À§Á¬ ÄÄÆ÷³ÍÆ®
    std::vector<Widget*> mWidgets;

    template <class T>
    void AddWidget(std::wstring _name)
    {
        static_assert(std::is_base_of<Widget, T>::value, "AddWidget_Fail");
        Object* widgetObject = gameObject->GetOwnerWorld()->CreateObject(gameObject->GetName() + L"_" + _name);
        T* widget = widgetObject->AddComponent<T>();
        widgetObject->transform->SetParent(gameObject->transform);
        mWidgets.push_back(widget);

        return widget;
    }
};

