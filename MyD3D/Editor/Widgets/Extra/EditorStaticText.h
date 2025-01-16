#pragma once
#include "Editor/Interface/IEditorObject.h"

namespace Editor
{
    class StaticText : public IWidget
    {
    public:
        explicit StaticText(const char* _label);
        virtual ~StaticText() = default;
    public:
        // �ؽ�Ʈ�� ����
        inline void SetText(const char* _label) { mText = _label; }
        inline auto GetText() { return mText; }
    public:
        virtual void Render() override;
    private:
        const char* mText;          // �ؽ�Ʈ
    };
}
