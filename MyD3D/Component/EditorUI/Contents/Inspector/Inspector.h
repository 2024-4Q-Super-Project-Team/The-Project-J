#pragma once
#include "Component/EditorUI/Contents/EditorWidget.h"
#include "Component/EditorUI/Contents/EditorUID.h"

class Object;

namespace Editor
{
    class Inspector : public Widget, public EUID
    {
    public:
        explicit Inspector();
        virtual ~Inspector() = default;
    public:
        virtual void Render() override;
    public:
        void SetFocusObject(Object* _object);
        Object* GetFocusObject();
        void AddComponent(const std::string _name, Object* _object);
    private:
        Object* mFocusObject;
    };
}
