#pragma once
#include "Component/Component.h"

class Mesh;

class MeshFilter
    : public Component
{
public:
    explicit MeshFilter(Object* _owner);
    virtual ~MeshFilter();
public:
    virtual void Start() override {};
    virtual void Tick() override {};
    virtual void FixedUpdate() override {};
    virtual void PreUpdate() override {};
    virtual void Update() override {};
    virtual void PostUpdate() override {};
    virtual void PreRender() override {};
    virtual void Render() override {};
    virtual void Draw(Camera* _camera) override {};
    virtual void PostRender() override {};
public:
   // void SetMesh()
private:
    Mesh* mMesh;
};
