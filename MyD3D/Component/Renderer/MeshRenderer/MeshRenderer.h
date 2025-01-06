#pragma once
#include "Component/Renderer/Renderer.h"

class Object;
class Transform;
class MeshResource;
class MaterialResource;
class Material;

class MeshRenderer
    : public RendererComponent
{
public:
    explicit MeshRenderer(Object* _owner);
    virtual ~MeshRenderer();
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
    virtual void Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable);
public:
    void SetMesh(std::shared_ptr<MeshResource> _mesh);
    void SetMaterial(std::shared_ptr<MaterialResource> _material);
private:
    std::shared_ptr<MeshResource>   mMesh;          // ��Ų�� �޽�
    Material*                       mMateiral;      // ����� ��Ƽ����
    TransformCBuffer                mTransformMatrices;
public:
    virtual void EditorRendering() override;
};