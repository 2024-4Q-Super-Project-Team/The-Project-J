#pragma once
#include "Editor/Interface/IEditorObject.h"

namespace Editor
{
    class TabNode;

    class TabBar : public IWidget
    {
    public:
        explicit TabBar(const char* _label);
        virtual ~TabBar() = default;
    public:
        virtual void Render() override;
    public:
        void AddTab(TabNode* _pTab);
        void SetActiveTab(UINT _tabIndex);
        void SetActiveTab(TabNode* _pTab);
    private:
        const char*             mLabel;
        TabNode*                mActiveTab;
        std::vector<TabNode*>   mContainer;
    };

    class TabNode : public IWidget
    {
    public:
        explicit TabNode(const char* _label);
        virtual ~TabNode() = default;
    public:
        virtual void Render() override;
    public:
        void  AddWidget(IWidget* _pWidget);
        bool& IsActive();
    protected:
        const char*             mLabel;
        bool                    isActive = false;
        std::vector<IWidget*>    mContainer;
    };

}


