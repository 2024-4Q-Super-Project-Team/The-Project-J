#pragma once
#include "Component/Component.h"

class MeshResource;
class MaterialResource;
class Material;
class Light;
class Camera;

class IRenderContext
{
public:
    virtual void DrawMesh(Camera* _camera) = 0;
    virtual void DrawShadow(Light* _pLight) = 0;
};

class RendererComponent
    : public Component
    , public IRenderContext
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
public:
    virtual void SetMesh(std::shared_ptr<MeshResource> _mesh) = 0;
    virtual void SetMaterial(std::shared_ptr<MaterialResource> _material) = 0;
    virtual std::shared_ptr<MeshResource> GetMesh() = 0;
    virtual Material* GetMaterial() = 0;
public:
    virtual void DrawMesh(Camera* _camera) = 0;
    virtual void DrawShadow(Light* _pLight) = 0;
public:
    virtual json Serialize() = 0;
    virtual void Deserialize(json& j) = 0;

};

