#include "pch.h"
#include "Inspector.h"
#include "Object/Object.h"

#include "Component/EditorUI/Contents/Widget/EditorEmpty.h"
#include "Component/EditorUI/Contents/Widget/EditorCheckBox.h"
#include "Component/EditorUI/Contents/Widget/EditorInputVector.h"
#include "Component/EditorUI/Contents/Widget/EditorStaticText.h"

namespace Editor
{
    Inspector::Inspector()
        : EUID("Inspector")
        , mFocusObject(nullptr)
    {
    }
    void Inspector::Render()
    {
         if (mFocusObject)
        {
            mFocusObject->EditorRendering();
            ImGui::Separator();
            //컴포넌트 추가 버튼
            //ImGui::SameLine();
            std::string objBtnName = "AddComponent##" + std::to_string(reinterpret_cast<uintptr_t>(mFocusObject));
            bool buttonClicked = ImGui::Button(objBtnName.c_str());

            if (buttonClicked)
            {
                AddComponent("MeshRenderer", mFocusObject);
            }
            for (auto compArr : mFocusObject->mComponentArray)
            {
                for (auto comp : compArr)
                {
                    comp->EditorRendering();
                    ImGui::Separator();
                }
            }
        }
    }
    void Inspector::SetFocusObject(Object* _object)
    {
        mFocusObject = _object;
    }
    Object* Inspector::GetFocusObject()
    {
        return mFocusObject;
    }

    void Inspector::AddComponent(const std::string _name, Object* _object)
    {
        CREATE_COMPONENT(_name, _object);
    }
}
