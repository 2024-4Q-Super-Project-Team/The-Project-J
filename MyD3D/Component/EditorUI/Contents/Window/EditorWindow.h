#pragma once
#include "Component/EditorUI/Contents/EditorWidget.h"

namespace Editor
{
    class WindowBar : public Widget
    {
    public:
        explicit WindowBar(const char* _name, Vector2 _size, Vector2 _offset = Vector2::Zero);
        virtual ~WindowBar() = default;
    public:
        virtual void Render() override;
    public:
        inline void  AddWidget(Widget* _pWidget) { mContainer.push_back(_pWidget); }
        inline void  AddFlags(ImGuiWindowFlags _flag) { mFlags |= _flag; }
        inline void  SetOffset(Vector2 _offset)  { mOffset = _offset; }
        inline void  SetSize(Vector2 _size)      { mSize = _size; }
        inline bool& IsOpen() { return mIsOpen; }
    private:
        bool mIsOpen;
        const char* mName;
        Vector2 mOffset;
        Vector2 mSize;
        ImGuiWindowFlags mFlags;
        std::vector<Widget*> mContainer;
    };
}
