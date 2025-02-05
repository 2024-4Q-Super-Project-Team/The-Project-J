#pragma once
#include "Component/Component.h"
#include "Resource/Resource.h"

class MeshResource;
class MaterialResource;
class Material;
class Light;
class Camera;
class Transform;

class IRenderContext
{
public:
    virtual Vector3                 GetDistanceFromCamera(Camera* _camera) = 0;
    virtual eBlendModeType          GetBlendMode() = 0;
    virtual eRasterizerStateType    GetCullingMode() = 0;
    virtual void                    DrawObject(Matrix& _view, Matrix& _projection) = 0;
    virtual void                    DrawShadow(Light* _pLight) = 0;
    virtual void                    DrawWire() = 0;
};

class RendererComponent
    : public Component
    , public IRenderContext
{
public:
    explicit RendererComponent(Object* _owner);
    virtual ~RendererComponent();
public:
    virtual void Start() = 0;
    virtual void Tick() = 0;
    virtual void FixedUpdate() = 0;
    virtual void PreUpdate() = 0;
    virtual void Update() = 0;
    virtual void PostUpdate() = 0;
    virtual void PreRender() = 0;
    virtual void Render() = 0;
    virtual void Draw(Camera* _camera) = 0;
    virtual void PostRender() = 0;
public:
    virtual void SetMesh(ResourceHandle _handle) = 0;
    virtual void SetMesh(MeshResource* _pResource) = 0;
    virtual void SetMaterial(ResourceHandle _handle) = 0;
    virtual void SetMaterial(MaterialResource* _pResource) = 0;
public:
    virtual MeshResource* GetMesh() = 0;
    virtual MaterialResource* GetMaterial() = 0;
public:
    virtual json Serialize() = 0;
    virtual void Deserialize(json& j) = 0;
protected:
    void UpdateOffsetMatrix();
    Vector3     mOffsetPosition = Vector3::Zero;
    Quaternion  mOffsetRotation = Quaternion::Identity;
    Vector3     mOffsetScale = Vector3::One;
    Matrix      mOffsetMatrix = Matrix::Identity;
};

