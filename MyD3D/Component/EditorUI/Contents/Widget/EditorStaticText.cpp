#include "pch.h"
#include "EditorStaticText.h"

namespace Editor
{
    StaticText::StaticText(const char* _text)
        : mText(_text)
    {
        mText = _text;
    }
    void StaticText::Render()
    {
        ImGui::Text(mText);
        TryLineBreak();
    }
}