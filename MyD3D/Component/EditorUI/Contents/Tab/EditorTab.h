#pragma once
#include "Component/EditorUI/Contents/EditorWidget.h"

namespace Editor
{
    class Tab;

    class TabBar : public Widget
    {
    public:
        explicit TabBar(const char* _idLabel);
        virtual ~TabBar() = default;
    public:
        virtual void Render() override;
    public:
        void    AddTab(Tab* _pTab);
        void    SetActiveTab(UINT _tabIndex);
        void    SetActiveTab(Tab* _pTab);
    private:
        const char* mIDLabel;
        Tab*        mActiveTab;
        std::vector<Tab*> mContainer;
    };

    class Tab : public Widget
    {
    public:
        explicit Tab(const char* _mainLabel);
        virtual ~Tab() = default;
    public:
        virtual void Render() override;
    public:
        inline void  AddWidget(Widget* _pWidget) { mContainer.push_back(_pWidget); }
        inline bool& IsActive() { return isActive; }
    protected:
        const char* mMainLabel;
        bool        isActive = false;
        std::vector<Widget*> mContainer;
    };

}


