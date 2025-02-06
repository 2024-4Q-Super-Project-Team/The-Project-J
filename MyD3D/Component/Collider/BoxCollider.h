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
    // Editor Only
    virtual void EditorUpdate() override;
    virtual void EditorRender() override;
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;
public:
    virtual void DrawObject(Matrix& _view, Matrix& _projection) override {};
    virtual void DrawShadow(Light* _pLight) override {}
    virtual void DrawWire() override;

    virtual MeshResource* GetMesh() { return nullptr; }
    virtual MaterialResource* GetMaterial() { return nullptr; }
public:
    virtual void SetPosition();
    virtual void SetRotation();
    void SetExtents();
    virtual Vector3 GetSize() override;

    void SetPosition(const Vector3& position) { mPosition = position; SetPosition(); }
    void SetRotation(const Vector3& rotation) { mRotation = rotation; SetRotation(); }
    void SetExtents(const Vector3& extents) { mExtents = extents; SetExtents();}
    void SetTrigger(bool isTrigger) { mIsTrigger = isTrigger; SetIsTrigger(); }

private:
    const Vector3 mInitialSize = { 1, 1, 1 };

    DirectX::BoundingOrientedBox mOBB;
    PxBoxGeometry mGeometry;
    Vector3 mExtents;
public:
    virtual void EditorRendering(EditorViewerType _type) override;
};

