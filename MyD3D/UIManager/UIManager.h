#pragma once

#include "UIManager/Widget/WidgetCheckBox.h"
#include "UIManager/Widget/WidgetText.h"
#include "UIManager/Widget/WidgetButton.h"
#include "UIManager/Widget/WidgetImage.h"

class Widget;
class Camera;

class UIManager
{
public:
    static BOOL Initialize();
    static void Finalization();
public:
    static void Tick();
    static void Update();
    static void PostUpate();
    static void Render();

    static void SetFocusViewport(ViewportScene* _pViewport);
public:
    static std::vector<Widget*> GetWidgets() { return mWidgetContainer; };
    
    template <class T>
    static T* AddWidget(std::wstring _id, std::wstring _filepath)
    {
        static_assert(std::is_base_of<Widget, T>::value, "AddWidget_Fail");
        T* widget = new T();
        widget->SetID(_id);
        widget->SetFilepath(_filepath);
        mWidgetContainer.push_back(widget);

        return widget;
    }

    static SpriteBatch* pSpriteBatch;

protected:
    // TODO : find 함수 만들기
    // auto FindWidget(std::wstring _id);
private:
    static ViewportScene* mFocusViewport;

    static Vector2  mFocusScreen;
    static Vector2  mCurrScreen;
    static Vector2  mScale;

    static std::vector<Widget*> mWidgetContainer;

    static void SetScale() { mScale = mCurrScreen / mFocusScreen; }
};