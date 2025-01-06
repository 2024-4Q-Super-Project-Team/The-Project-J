#pragma once
class GraphicsManager;
namespace Engine
{
    class ICycleEventHandler
    {
    public:
        virtual void _CALLBACK OnTick() = 0;
        virtual void _CALLBACK OnFixedUpdate() = 0;
        virtual void _CALLBACK OnPreUpdate() = 0;
        virtual void _CALLBACK OnUpdate() = 0;
        virtual void _CALLBACK OnPostUpdate() = 0;
        virtual void _CALLBACK OnPreRender() = 0;
        virtual void _CALLBACK OnRender() = 0;
        virtual void _CALLBACK OnPostRender() = 0;
    };
}
