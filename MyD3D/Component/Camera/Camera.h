#pragma once
#include "Component/Component.h"

class D3DBitmapRenderTarget;
class D3DGraphicsViewport;
class IRenderContext;
class Light;
class SkyBox;
class ViewportScene;

using DrawQueue = std::vector<IRenderContext*>;
using LightQueue = std::vector<Light*>;

enum class ProjectionType
{
    Perspective,
    Orthographic
};

enum class CameraRenderType
{
    Forward,
    Deferred,
};

class Camera
    : public Component
{
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
    // Editor Only
    virtual void EditorUpdate() override;
    virtual void EditorRender() override;
public:
    void SetCameraSize(Vector2 _sizeScale);
    void SetCameraOffset(Vector2 _offsetScale);
    // ī�޶� Draw�� ������Ʈ�� Push�մϴ�.
    void PushDrawList(IRenderContext* _renderContext);
    // ī�޶� Light������ Push�մϴ�. �� ������Ʈ���� Push����� ��.
    void PushLight(Light* _pLight);
    // ���� ť�� Draw �۾� ����
    void ExcuteDrawList(); 
private:
    void UpdateCamera();
    void UpdateMatrix();
    void UpdateViewport();
private:
    void DrawShadow();
    void DrawForward();
    void DrawDeferred();
    void DrawSwapChain();
public:
    void SetProjectionType(ProjectionType _type);
    void SetCameraRenderType(CameraRenderType _type);
public:
    inline void SetFovAngle(Degree _angle) { mFovAngle = _angle; }
    inline void SetProjectionNear(float _near) { mProjectionNear = _near; }
    inline void SetProjectionFar(float _far) { mProjectionFar = _far; }
    inline void SetSkyBox(SkyBox* _pSkybox) { mSkyBox = _pSkybox; }

    inline const Matrix& GetView() { return mViewMatrix; }
    inline const Matrix& GetProjection() { return mProjectionMatrix; }
    inline const float GetProjectionNear() { return mProjectionNear; }
    inline const float GetProjectionFar() { return mProjectionFar; }
    inline const auto* GetViewport() { return mLocalViewport; }

    D3DBitmapRenderTarget* GetCurrentRenderTarget();
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;
private:
    /////////////////////////////////////////////
    // ����Ʈ�� �ڿ��� �����޴´�.
    /////////////////////////////////////////////
    // ���� ī�޶� ���� Viewport��
    ViewportScene* mCameraViewport;
    // ���� ī�޶� ���� �ִ� ���ҽ� ��
    std::shared_ptr<D3DGraphicsViewport>        mMainViewport;
    std::shared_ptr<D3DBitmapRenderTarget>      mMainRenderTarget;
    std::shared_ptr<D3DBitmapRenderTarget>      mDeferredRenderTarget;
private:
    Matrix                  mViewMatrix;
    Matrix                  mProjectionMatrix;

    Degree                  mFovAngle;
    float                   mProjectionNear;
    float                   mProjectionFar;

    // ������������ ����
    float                   mOrthoWidth;
    float                   mOrthoHeight;

    Vector2                 mSizeScale;
    Vector2                 mOffsetScale;

    D3DGraphicsViewport*    mLocalViewport;
    DrawQueue               mDrawQueue[BLEND_TYPE_COUNT];
    LightQueue              mSceneLights;

    ProjectionType          mProjectionType;
    CameraRenderType        mCameraRenderType;

    CameraCBuffer           mCameraCBuffer;
	LightCBuffer			mLightCBuffer;

    // ī�޶� ��ī�̹ڽ�
    SkyBox*                 mSkyBox;
public:
    virtual void EditorRendering(EditorViewerType _viewerType) override;
};
