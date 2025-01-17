#include "pch.h"
#include "EditorTab.h"

namespace Editor
{
    TabBar::TabBar(const char* _label)
        : mLabel(_label), mActiveTab(nullptr)
    {
    }

    void TabBar::Render()
    {
        if (ImGui::BeginTabBar(mLabel))
        {
            for (auto& pWidget : mContainer)
            {
                pWidget->Render();
            }
        }
        ImGui::EndTabBar();
    }

    void TabBar::AddTab(TabNode* _pTab)
    {
        mContainer.push_back(_pTab);
    }

    void TabBar::SetActiveTab(UINT _tabIndex)
    {
        mActiveTab->IsActive() = false;
        mContainer[_tabIndex]->IsActive() = true;
    }

    void TabBar::SetActiveTab(TabNode* _pTab)
    {
        mActiveTab->IsActive() = false;
        _pTab->IsActive() = true;
    }

    TabNode::TabNode(const char* _label)
        : mLabel(_label)
    {
    }

    void TabNode::Render()
    {
        if (ImGui::BeginTabItem(mLabel))
        {
            for (auto& pWidget : mContainer)
            {
                pWidget->Render();
            }
            ImGui::EndTabItem();
        }
    }
    void TabNode::AddWidget(IWidget* _pWidget)
    {
        mContainer.push_back(_pWidget);
    }
    bool& TabNode::IsActive()
    {
        return isActive;
    }
}