#include "pch.h"
#include "EditorInspectorViewer.h"
#include "Object/Object.h"

namespace Editor
{
    IEditorObject* InspectorViewer::mFocusObject = nullptr;

    InspectorViewer::InspectorViewer()
    {
    }

    void InspectorViewer::Render()
    {
         if (mFocusObject)
         {
            mFocusObject->EditorRendering(EditorViewerType::INSPECTOR);
         }
    }

    void InspectorViewer::RenderGizmo()
    {
        if (mFocusObject)
        {
            mFocusObject->EditorRendering(EditorViewerType::GUIZMO);
        }
    }

    void InspectorViewer::SetFocusObject(IEditorObject* _object)
    {
        mFocusObject = _object;
    }

    bool InspectorViewer::IsFocusObject(IEditorObject* _object)
    {
        return mFocusObject == _object;
    }

    IEditorObject* InspectorViewer::GetFocusObject()
    {
        return mFocusObject;
    }
}
