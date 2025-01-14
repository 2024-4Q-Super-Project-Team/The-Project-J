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
            std::string cmpBtnName = "AddComponent##" + std::to_string(reinterpret_cast<uintptr_t>(mFocusObject));
            bool buttonClicked = ImGui::Button(cmpBtnName.c_str());
            if (buttonClicked) 
                mbComponentChoosing = true;
            if (mbComponentChoosing)
                ShowComponentList();

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

    void Inspector::ShowComponentList()
    {
        ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos());
        ImGui::OpenPopup("Component List");
        if (ImGui::BeginPopup("Component List"))
        {
            std::vector<std::string_view>& names = ComponentFactory::GetComopnentNames();

            for (const std::string_view& name : names)
            {
                if (ImGui::Selectable(((std::string)name).c_str()))
                {
                    CREATE_EDITOR_COMPONENT(name, mFocusObject);
                    mbComponentChoosing = false;
                }
            }

            if (ImGui::IsMouseClicked(1))
            {
                mbComponentChoosing = false;
            }

            ImGui::EndPopup();
        }
       
    }
}
