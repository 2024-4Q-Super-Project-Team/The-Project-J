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
    virtual void DrawMesh(Camera* _camera) override;
    virtual void DrawShadow(Light* _pLight) override;
public:
    virtual std::shared_ptr<MeshResource> GetMesh() override;
    virtual Material* GetMaterial() override;
    virtual void SetMesh(ResourceHandle _handle) override;
    virtual void SetMaterial(ResourceHandle _handle) override;
    void         SetRootBone(Transform* _rootBone);
private:
    void UpdateTable();
    void BoneMapping(Transform* currentBone);
    void CalculateBoneTransform();
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;
private:
    // 본이나 메쉬 등이 수정되었는가
    bool isDirty = true;

    ResourceHandle                  mMeshHandle;
    std::shared_ptr<MeshResource>   mMesh;  
    ResourceHandle                  mMaterialaHandle;
    Material*                       mMateiral;   
    Transform*                      mRootBone; 

    std::unordered_map<std::wstring, Transform*> mBoneMappingTable; // 본 이름과 트랜스폼 매칭
    BoneMatrixCBuffer mFinalBoneMatrices; // 최종 본 매트릭스 (셰이더로 전달)
	TransformCBuffer  mTransformMatrices; // 트랜스폼 매트릭스 (셰이더로 전달)

	bool isCastShadow = true;
public:
    virtual void EditorRendering(EditorViewerType _viewerType) override;
};