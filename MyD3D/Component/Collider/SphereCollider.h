#pragma once
#include "Collider.h"
class SphereCollider : public Collider
{
public:
    explicit SphereCollider(Object* _owner);
    virtual ~SphereCollider() {}
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
    // Editor Only
    virtual void EditorUpdate() override;
    virtual void EditorRender() override;
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;
public:
    virtual void DrawObject(Matrix& _view, Matrix& _projection) override;
    virtual void DrawShadow(Light* _pLight) override {}
    virtual void DrawWire() override;

    virtual MeshResource* GetMesh() { return nullptr; }
    virtual MaterialResource* GetMaterial() { return nullptr; }
    float GetRadius() { return mRadius; }
public:
    virtual void SetPosition();
    virtual void SetRotation();
    void SetRadius();
    void SetRadius(float _radius) {  mRadius = _radius; SetRadius(); }
private:
    const float mInitialRadius = 1.f;

    DirectX::BoundingSphere mBS;
    PxSphereGeometry mGeometry;
    float mRadius;
public:
    virtual void EditorRendering(EditorViewerType _type) override;
};

