#include "pch.h"
#include "EditorMenu.h"

namespace Editor
{
    MenuNode::MenuNode(const char* _label)
        : mLabel(_label), isActive(false)
    {
    }
    MenuItem::MenuItem(const char* _label, std::function<void()> _action)
        : mLabel(_label), mAction(_action), isActive(false)
    {
    }

    MenuNode* MenuBar::AddMenuNode(const char* _label)
    {
        MenuNode* pWidget = new MenuNode(_label);
        mContainer.push_back(pWidget);
        return pWidget;
    }
    MenuItem* MenuNode::AddMenuItem(const char* _label, std::function<void()> _action)
    {
        MenuItem* pWidget = new MenuItem(_label, _action);
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
    void MenuNode::Render()
    {
        std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
        if (ImGui::BeginMenu(uid.c_str(), isActive))
        {
            for (auto& pWidget : mContainer)
            {
                pWidget->Render();
            }
            ImGui::EndMenu();
        }
    }
    void MenuItem::Render()
    {
        std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
        if (ImGui::MenuItem(uid.c_str(), mLabel, isActive)) 
        {
            if (mAction)
            {
                mAction(); // 클릭 시 액션 실행
            }
        }
    }
}