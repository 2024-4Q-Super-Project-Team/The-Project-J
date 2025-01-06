#pragma once
#include "Component/EditorUI/Contents/EditorWidget.h"

namespace Editor
{
    class Empty : public Widget
    {
    public:
        explicit Empty();
        virtual ~Empty() = default;
    public:
        virtual void Render();
    };
}


