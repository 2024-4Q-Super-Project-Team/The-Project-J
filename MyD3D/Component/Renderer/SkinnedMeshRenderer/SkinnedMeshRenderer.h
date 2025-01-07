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
    // 본이나 메쉬 등이 수정되었는가
    bool isDirty = true;

    std::shared_ptr<MeshResource>   mMesh; // 스킨드 메쉬
    Material* mMateiral; // 사용할 머티리얼
    Transform* mRootBone; // 루트 본 트랜스폼

    std::unordered_map<std::wstring, Transform*> mBoneMappingTable; // 본 이름과 트랜스폼 매칭
    BoneMatrixCBuffer mFinalBoneMatrices; // 최종 본 매트릭스 (셰이더로 전달)
    TransformCBuffer  mTransformMatrices;
public:
    virtual void EditorRendering() override;
};