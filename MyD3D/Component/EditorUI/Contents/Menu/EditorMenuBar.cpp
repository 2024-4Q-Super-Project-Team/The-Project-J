#include "pch.h"
#include "EditorMenuBar.h"
#include "Component/EditorUI/EditorUI.h"

namespace Editor
{
    Menu::Menu(const char* _mainLabel)
        : EUID(_mainLabel), isActive(true)
    {
    }
    MenuItem::MenuItem(const char* _mainLabel, const char* _subLabel, std::function<void()> _action)
        : EUID(_mainLabel), mSubLabel(_subLabel), mAction(_action)
    {
    }

    Menu* MenuBar::AddMenu(const char* _mainLabel)
    {
        Menu* pWidget = new Menu(_mainLabel);
        mContainer.push_back(pWidget);
        return pWidget;
    }
    MenuItem* Menu::AddItem(const char* _mainLabel, const char* _subLabel, std::function<void()> _action)
    {
        MenuItem* pWidget = new MenuItem(_mainLabel, _subLabel, _action);
        mContainer.push_back(pWidget);
        return nullptr;
    }

    void MenuBar::Render()
    {
        if (ImGui::BeginMainMenuBar())
        {
            for (auto& pWidget : mContainer)
            {
                pWidget->Render();
            }
            ImGui::EndMainMenuBar();
        }
    }
    void Menu::Render()
    {
        if (ImGui::BeginMenu(mUID.c_str(), isActive))
        {
            for (auto& item : mContainer)
            {
                item->Render();
            }
            ImGui::EndMenu();
        }
    }
    void MenuItem::Render()
    {
        if (ImGui::MenuItem(mUID.c_str(), mSubLabel, isActive)) {
            if (mAction) {
                mAction(); // 클릭 시 액션 실행
            }
        }
    }
}