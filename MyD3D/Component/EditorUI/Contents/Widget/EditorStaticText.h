#pragma once
#include "Component/EditorUI/Contents/EditorWidget.h"
#include "Component/EditorUI/Contents/EditorOptional.h"

namespace Editor
{
    class StaticText : public Widget, public Optional
    {
    public:
        explicit StaticText(const char* _text);
        virtual ~StaticText() = default;
    public:
        // 텍스트를 설정
        inline void SetText(const char* _text) { mText = _text; }
        inline auto GetText() { return mText; }
    public:
        virtual void Render() override;
    private:
        const char* mText;          // 텍스트
    };
}
