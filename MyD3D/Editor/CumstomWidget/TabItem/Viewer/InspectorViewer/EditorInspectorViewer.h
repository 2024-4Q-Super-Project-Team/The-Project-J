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
    public:
        void SetFocusObject(IEditorObject* _object);
        IEditorObject* GetFocusObject();
    private:
        IEditorObject* mFocusObject;
        bool mbComponentChoosing = false;

    };
}
