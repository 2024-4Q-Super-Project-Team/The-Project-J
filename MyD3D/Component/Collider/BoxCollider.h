#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
public:
    explicit BoxCollider(Object* _owner);
    virtual ~BoxCollider() {}
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
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;
public:
    virtual void DrawMesh(Camera* _camera) override;
    virtual void DrawShadow(Light* _pLight) override {}
public:
    void SetExtents();

private:
    void UpdateOBB();
private:
    const Vector3 mInitialSize = { 1, 1, 1 };

    DirectX::BoundingOrientedBox mOBB;
    PxBoxGeometry mGeometry;
    Vector3 mExtents;
public:
    virtual void EditorRendering(EditorViewerType _type) override;
};

