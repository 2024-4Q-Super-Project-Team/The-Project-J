#pragma once
#include "Component/EditorUI/Contents/EditorWidget.h"
#include "Component/EditorUI/Contents/EditorOptional.h"
#include "Component/EditorUI/Contents/EditorUID.h"

namespace Editor
{
    class InputVector3 
        : public Widget, public Optional, public EUID
    {
    public:
        explicit InputVector3(const char* _label, Vector3* _ref = nullptr);
        virtual ~InputVector3() = default;
    public:
        inline void SetVector(Vector3* _pVector) { mRefVector = _pVector; }
    public:
        virtual void Render() override;
    private:
        Vector3*    mRefVector;
    };
}