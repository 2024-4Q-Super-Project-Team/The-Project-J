#pragma once
#include "Component/Renderer/Renderer.h"

class Object;
class Transform;
class MeshResource;

class SkinnedMeshRenderer
    : public RendererComponent
{
public:
    explicit SkinnedMeshRenderer(Object* _owner);
    virtual ~SkinnedMeshRenderer();
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
    virtual void DrawCall() override;
public:
    virtual std::shared_ptr<MeshResource> GetMesh() override;
    virtual Material* GetMaterial() override;
    virtual void SetMesh(std::shared_ptr<MeshResource> _mesh) override;
    virtual void SetMaterial(std::shared_ptr<MaterialResource> _material) override;
    void SetRootBone(Transform* _rootBone);
private:
    void UpdateTable();
    void BoneMapping(Transform* currentBone);
    void CalculateBoneTransform();
private:
    // ���̳� �޽� ���� �����Ǿ��°�
    bool isDirty = true;

    std::shared_ptr<MeshResource>   mMesh; // ��Ų�� �޽�
    Material* mMateiral; // ����� ��Ƽ����
    Transform* mRootBone; // ��Ʈ �� Ʈ������

    std::unordered_map<std::wstring, Transform*> mBoneMappingTable; // �� �̸��� Ʈ������ ��Ī
    BoneMatrixCBuffer mFinalBoneMatrices; // ���� �� ��Ʈ���� (���̴��� ����)
    TransformCBuffer  mTransformMatrices;
public:
    virtual void EditorRendering() override;
};