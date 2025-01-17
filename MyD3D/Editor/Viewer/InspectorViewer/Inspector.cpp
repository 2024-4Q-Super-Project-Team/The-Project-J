#include "pch.h"
#include "Inspector.h"
#include "Object/Object.h"

namespace Editor
{
    Inspector::Inspector()
        : mFocusObject(nullptr)
    {
    }
    void Inspector::Render()
    {
         if (mFocusObject)
        {
            mFocusObject->EditorRendering();
        }
    }
    void Inspector::SetFocusObject(IEditorObject* _object)
    {
        mFocusObject = _object;
    }
    IEditorObject* Inspector::GetFocusObject()
    {
        return mFocusObject;
    }
}
