#pragma once
#include "Editor/Interface/IEditorObject.h"

class Object;

namespace Editor
{
    class Inspector : public IWidget
    {
    public:
        explicit Inspector();
        virtual ~Inspector() = default;
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
