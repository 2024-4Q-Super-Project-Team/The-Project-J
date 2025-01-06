#include "pch.h"
#include "EditorInputVector.h"
#include "Component/EditorUI/EditorUI.h"

namespace Editor
{
    InputVector3::InputVector3(const char* _label, Vector3* _ref)
        : EUID(_label), mRefVector(_ref)
    {
    }
    void InputVector3::Render()
    {
        if (mRefVector)
        {
            ImGui::InputFloat3(mUID.c_str(), &mRefVector->x);
        }
        TryLineBreak();
    }
}