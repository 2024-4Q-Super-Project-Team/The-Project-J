#pragma once
#include "Component/Renderer/Renderer.h"

class Object;
class Transform;
class MeshResource;

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
    virtual void DrawMesh(Camera* _camera) override;
    virtual void DrawShadow(Light* _pLight) override;
public:
    virtual void SetMesh(std::shared_ptr<MeshResource> _mesh) override;
    virtual void SetMaterial(std::shared_ptr<MaterialResource> _material) override;
    virtual std::shared_ptr<MeshResource> GetMesh() override;
    virtual Material* GetMaterial() override;
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;
private:
    std::shared_ptr<MeshResource> mMesh;    // 메쉬
    Material* mMateiral;                    // 사용할 머티리얼
	TransformCBuffer mTransformMatrices;    // 트랜스폼 매트릭스 (셰이더로 전달)

    bool isCastShadow = true;               // 그림자 렌더링 여부
public:
    virtual void EditorRendering(EditorViewerType _viewerType) override;
};