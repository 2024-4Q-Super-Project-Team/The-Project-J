#include "pch.h"
#include "EditorTab.h"

namespace Editor
{
    TabBar::TabBar(const char* _idLabel)
        : mIDLabel(_idLabel), mActiveTab(nullptr)
    {
    }

    void TabBar::Render()
    {
        if (ImGui::BeginTabBar(mIDLabel))
        {
            for (auto& pWidget : mContainer)
            {
                pWidget->Render();
            }
        }
        ImGui::EndTabBar();
    }

    void TabBar::AddTab(Tab* _pTab)
    {
        mContainer.push_back(_pTab);
    }

    void TabBar::SetActiveTab(UINT _tabIndex)
    {
        mActiveTab->IsActive() = false;
        mContainer[_tabIndex]->IsActive() = true;
    }

    void TabBar::SetActiveTab(Tab* _pTab)
    {
        mActiveTab->IsActive() = false;
        _pTab->IsActive() = true;
    }

    Tab::Tab(const char* _mainLabel)
        : mMainLabel(_mainLabel)
    {
    }

    void Tab::Render()
    {
        if (ImGui::BeginTabItem(mMainLabel))
        {
            for (auto& pWidget : mContainer)
            {
                pWidget->Render();
            }
            ImGui::EndTabItem();
        }
    }
}