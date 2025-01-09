#pragma once
#include "Component/Component.h"

class D3DGraphicsRenderTarget;
class D3DGraphicsViewport;
class RendererComponent;
class Light;

using DrawQueue = std::vector<RendererComponent*>;
using LightQueue = std::vector<Light*>;

class Camera
    : public Component
{
    enum class ProjectionType;
public:
    explicit Camera(Object* _owner, Vector2 _size = Vector2::Zero);
    virtual ~Camera();
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
    // ī�޶� Draw�� ������Ʈ�� Push�մϴ�.
    void PushDrawList(RendererComponent* _renderComponent);
    // ī�޶� Light������ Push�մϴ�. �� ������Ʈ���� Push����� ��.
    void PushLight(Light* _pLight);
    // ���� ť�� Draw �۾� ����
    void ExcuteDrawList(); 
private:
    void UpdateCamera();
public:
    inline void SetProjectionType(ProjectionType _type) { mProjectionType = _type; }
    inline void SetFovAngle(Degree _angle) { mFovAngle = _angle; }
    inline void SetProjectionNear(float _near) { mProjectionNear = _near; }
    inline void SetProjectionFar(float _far) { mProjectionFar = _far; }

    inline const Matrix& GetView() { return mViewMatrix; }
    inline const Matrix& GetProjection() { return mProjectionMatrix; }
    inline const float GetProjectionNear() { return mProjectionNear; }
    inline const float GetProjectionFar() { return mProjectionFar; }
    inline const auto* GetViewport() { return mViewport; }
public:
    virtual json Serialize() override;
    virtual json Deserialize() override;
private:
    DrawQueue  mDrawQueue[RENDERING_MODE_COUNT];
    LightQueue mSceneLights;

    Degree  mFovAngle;
    float   mProjectionNear;
    float   mProjectionFar;
    D3DGraphicsViewport* mViewport;
    CameraCBuffer        mCameraCBuffer;
    ProjectionType       mProjectionType;

    Matrix mViewMatrix;
    Matrix mProjectionMatrix;

    // ������������ ����
    float   mOrthoWidth;
    float   mOrthoHeight;
private:
public:
    enum class ProjectionType
    {
        Perspective,
        Orthographic
    };
public:
    virtual void EditorRendering() override;
};

