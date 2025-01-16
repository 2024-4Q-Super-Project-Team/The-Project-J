#pragma once
#include "Editor/Interface/IEditorObject.h"

namespace Editor
{
    class CheckBox : public IWidget
    {
    public:
        explicit CheckBox(const char* _label, bool* _pValue = nullptr)
        : mLabel(_label), mValue(_pValue) {} 
        virtual ~CheckBox() = default;
    public:
        inline  void SetBool(bool* _pValue) { mValue = _pValue; }
        virtual void Render() override;
    private:
        const char* mLabel;
        bool* mValue;
    };
}

