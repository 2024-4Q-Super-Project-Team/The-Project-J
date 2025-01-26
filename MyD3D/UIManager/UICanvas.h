#pragma once

#include "UIManager/Widget/WidgetImage.h"
#include "UIManager/Widget/WidgetText.h"

#include "Component/Component.h"

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
    void AddWidget(std::wstring _id)
    {
        static_assert(std::is_base_of<Widget, T>::value, "AddWidget_Fail");
        T* widget = new T();
        widget->SetParents();
        mWidgets.push_back(widget);

        return widget;
    }
};

