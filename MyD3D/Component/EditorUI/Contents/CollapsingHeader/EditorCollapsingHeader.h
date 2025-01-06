#pragma once
#include "Component/EditorUI/Contents/EditorWidget.h"
#include "Component/EditorUI/Contents/EditorUID.h"

namespace Editor
{
    class CollapsingHeader
        : public Widget, public EUID
    {
    public:
        explicit CollapsingHeader(const char* _label);
        virtual ~CollapsingHeader() = default;
    public:
        virtual void Render() override;
    public:
        inline void  AddWidget(Widget* _pWidget) { mContainer.push_back(_pWidget); }
    private:
        std::vector<Widget*> mContainer;
    };
}


