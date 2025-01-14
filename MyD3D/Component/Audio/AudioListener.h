#pragma once
#include "Component/Component.h"

class AudioResource;

class AudioListener
    : public Component
{
public:
    explicit AudioListener(Object* _owner);
    virtual ~AudioListener();
public:
    virtual void Start() override;
    virtual void Tick() override;
    virtual void FixedUpdate() override;
    virtual void PreUpdate() override;
    virtual void Update() override;
    virtual void PostUpdate() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void Draw(Camera* _camera) override;
    virtual void PostRender() override;
public:

};

