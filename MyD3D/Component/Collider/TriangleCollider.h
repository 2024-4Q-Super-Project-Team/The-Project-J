#pragma once
#include "Collider.h"
class TriangleCollider : public Collider
{
public:
    explicit TriangleCollider(Object* _owner);
    virtual ~TriangleCollider();
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
    virtual void EditorGlobalUpdate() override;
    virtual void EditorRender() override;
public:
    virtual void _CALLBACK OnEnable() override;
    virtual void _CALLBACK OnDisable() override;
    virtual void _CALLBACK OnDestroy() override;
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
private:
    PxTriangleMeshGeometry mTriangleGeom;
    MeshResource* mMesh;
    ResourceHandle mMeshHandle;
    PxTriangleMesh* mTmesh;
public:
    virtual void EditorRendering(EditorViewerType _type) override;
};


