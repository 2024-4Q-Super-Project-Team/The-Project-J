#pragma once
#include "Collider.h"
class BoxCollider : public Collider
{
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
    void SetSize(Vector3 _size) { mSize = _size; }


private:
    Vector3 mSize = { 1, 1, 1 };
};

