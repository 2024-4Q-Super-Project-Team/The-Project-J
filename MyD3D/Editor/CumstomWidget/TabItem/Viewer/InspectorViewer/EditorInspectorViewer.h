#pragma once
#include "Editor/Interface/IEditorObject.h"

class Object;

namespace Editor
{
    class InspectorViewer : public IWidget
    {
    public:
        explicit InspectorViewer();
        virtual ~InspectorViewer() = default;
    public:
        virtual void Render() override;
        void RenderGizmo();
    public:
        static void SetFocusObject(IEditorObject* _object);
        static bool IsFocusObject(IEditorObject* _object);
        static IEditorObject* GetFocusObject();
    private:
        static IEditorObject* mFocusObject;
        bool mbComponentChoosing = false;

    };
}
