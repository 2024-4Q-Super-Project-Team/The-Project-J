#pragma once

class EditorWorld : public World
{
public:
    using World::World;
public:
    virtual void _CALLBACK OnCreate()		override;
    virtual void _CALLBACK OnDestroy()		override;
    virtual void _CALLBACK OnTick()		    override;
};

