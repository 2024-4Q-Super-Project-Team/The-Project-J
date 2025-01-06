#include "pch.h"
#include "EditorCollapsingHeader.h"
#include "Component/EditorUI/EditorUI.h"

namespace Editor
{
    CollapsingHeader::CollapsingHeader(const char* _label)
        : EUID(_label)
    {
    }

    void CollapsingHeader::Render()
    {
        if (ImGui::CollapsingHeader(mUID.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (auto& pWidget : mContainer)
            {
                pWidget->Render();
            }
        }
    }
}

