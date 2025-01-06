#pragma once
#include "Component/Component.h"

class RendererComponent
    : public Component
{
public:
    explicit RendererComponent(Object* _owner);
    virtual ~RendererComponent();
public:
    virtual void Start() = 0;
    virtual void Tick() = 0;
    virtual void FixedUpdate() = 0;
    virtual void PreUpdate() = 0;
    virtual void Update() = 0;
    virtual void PostUpdate() = 0;
    virtual void PreRender() = 0;
    virtual void Render() = 0;
    virtual void Draw(Camera* _camera) = 0;
    virtual void PostRender() = 0;
private:

};

