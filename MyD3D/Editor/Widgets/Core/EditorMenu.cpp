#include "pch.h"
#include "EditorMenu.h"

namespace Editor
{
    MenuNode::MenuNode(const char* _label)
        : mLabel(_label)
    {
    }
    MenuItem::MenuItem(const char* _label, std::function<void()> _action)
        : mLabel(_label), mAction(_action), isActive(false)
    {
    }

    void MenuItem::AddSubMenuItem(IWidget* _pWidget)
    {
        mContainer.push_back(_pWidget);
    }

    void MenuBar::AddMenuNode(IWidget* _pWidget)
    {
        mContainer.push_back(_pWidget);
    }
    void MenuNode::AddMenuItem(IWidget* _pWidget)
    {
        mContainer.push_back(_pWidget);
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
        if (ImGui::BeginMenu((mLabel + uid).c_str()))
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
        if (ImGui::MenuItem((mLabel + uid).c_str(), mLabel))
        {
            if (mAction)
            {
                mAction(); // 클릭 시 액션 실행
            }
        }
    }
}