#include "pch.h"
#include "EditorUID.h"
#include "Component/EditorUI/EditorUI.h"

namespace Editor
{
    EUID::EUID(const char* _label)
        : mUID(std::string(_label) + "##" + EditorUI::GetUID())
    {
    }
    void EUID::ReSetUID(const char* _label)
    {
        mUID = std::string(_label) + "##" + EditorUI::GetUID();
    }
}

