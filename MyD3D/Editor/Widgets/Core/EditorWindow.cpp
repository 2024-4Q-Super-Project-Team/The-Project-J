#include "pch.h"
#include "EditorWindow.h"


namespace Editor
{

    WindowBar::WindowBar(const char* _label, Vector2 _size, Vector2 _offset)
        : mLabel(_label), mSize(_size), mOffset(_offset), mIsOpen(true), mFlags(0)
    {
    }

    void WindowBar::Render()
    {
        std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

        float gOffsetHeight = ImGui::GetFrameHeight();
        ImGui::SetNextWindowPos(ImVec2(mOffset.x, mOffset.y + gOffsetHeight));
        ImGui::SetNextWindowSize(ImVec2(mSize.x, mSize.y - gOffsetHeight));

        ImGui::Begin((mLabel + uid).c_str(), &mIsOpen, mFlags);
        for (auto& pWidget : mContainer)
        {
            pWidget->Render();
        }
        ImGui::End();
    }
}
