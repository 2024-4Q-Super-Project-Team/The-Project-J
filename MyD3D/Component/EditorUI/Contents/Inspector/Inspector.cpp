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
}
