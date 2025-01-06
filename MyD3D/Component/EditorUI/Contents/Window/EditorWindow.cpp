#include "pch.h"
#include "EditorWindow.h"


namespace Editor
{

    WindowBar::WindowBar(const char* _name, Vector2 _size, Vector2 _offset)
        : mName(_name), mSize(_size), mOffset(_offset), mIsOpen(true), mFlags(0)
    {
    }

    void WindowBar::Render()
    {
        float gOffsetHeight = ImGui::GetFrameHeight();
        ImGui::SetNextWindowPos(ImVec2(mOffset.x, mOffset.y + gOffsetHeight));
        ImGui::SetNextWindowSize(ImVec2(mSize.x, mSize.y - gOffsetHeight));
        ImGui::Begin(mName, &mIsOpen, mFlags);
        for (auto& pWidget : mContainer)
        {
            pWidget->Render();
        }
        ImGui::End();
    }
}
