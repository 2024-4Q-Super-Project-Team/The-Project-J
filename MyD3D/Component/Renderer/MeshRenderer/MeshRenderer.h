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
    // Editor Only
    virtual void EditorUpdate() override;
    virtual void EditorGlobalUpdate() override;
    virtual void EditorRender() override;
public:
    virtual void Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable);
public:
    virtual void DrawObject(Matrix& _view, Matrix& _projection) override;
    virtual void DrawShadow(Light* _pLight) override;
    virtual void DrawWire() override;
public:
    virtual void SetMesh(ResourceHandle _handle) override;
    virtual void SetMesh(MeshResource* _pResource) override;
    virtual void SetMaterial(ResourceHandle _handle) override;
    virtual void SetMaterial(MaterialResource* _pResource) override;
    virtual MeshResource* GetMesh() override;
    virtual MaterialResource* GetMaterial() override;
    virtual eBlendModeType GetBlendMode() override;
    virtual Vector3 GetDistanceFromCamera(Camera* _camera) override;
    virtual eRasterizerStateType GetCullingMode() override;
public:
    // 외곽선 관련 함수
    void SetOutlineColor(Color _color)  { mOutlineCBuffer.outlineColor = _color; }
    void SetOutlineScale(float _offset) { mOutlineCBuffer.outlineOffset = _offset; }
    Color GetOutlineColor()             { return mOutlineCBuffer.outlineColor; }
    float GetOutlineScale()             { return mOutlineCBuffer.outlineOffset; }
public:
    virtual void _CALLBACK OnEnable() override;
    virtual void _CALLBACK OnDisable() override;
    virtual void _CALLBACK OnDestroy() override;
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;
private:
    ResourceHandle      mMeshHandle;
    ResourceHandle      mMaterialHandle;
    MeshResource*       mMesh; 
    MaterialResource*   mMaterial; 

	TransformCBuffer    mTransformMatrices;    // 트랜스폼 매트릭스 (셰이더로 전달)
    MaterialCBuffer     mMatCBuffer;           // 머티리얼 상수 버퍼 (셰이더로 전달)
    OutlineCBuffer      mOutlineCBuffer;

    bool isCastShadow = true;               // 그림자 렌더링 여부
    bool isCastOutline = false;               // 외곽선 렌더링 여부
public:
    virtual void EditorRendering(EditorViewerType _viewerType) override;
    void ShowMaerialProperties(eMaterialMapType _type, const char* _label);
};