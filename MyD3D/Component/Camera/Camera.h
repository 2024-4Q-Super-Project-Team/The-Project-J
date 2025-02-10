#pragma once
#include "Component/Renderer/Renderer.h"

class D3DBitmapRenderTarget;
class D3DGraphicsViewport;
class IRenderContext;
class Light;
class SkyBox;
class ViewportScene;
class Transform;

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
    , public IRenderContext
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
    // ī�޶� Outline�� Draw�� ������Ʈ�� Push �մϴ�.
    void PushOutlineDrawList(IRenderContext* _renderContext);
    // ī�޶� Light������ Push�մϴ�. �� ������Ʈ���� Push����� ��.
    void PushLightList(Light* _pLight);
    // ī�޶� �׸� ���̾������� ������ Push�մϴ�.
    void PushWireList(IRenderContext* _renderContext);
private:
    void UpdateCamera();
    void UpdateMatrix();
    void UpdateViewport();
    void UpdateZSort();
private:
    void DrawShadowList();
    void DrawForwardList();
    void DrawDeferredList();
    void DrawWireList();
    void DrawOutlineList();
    void DrawParticle();
    void DrawSwapChain();
    // ���� ť�� Draw �۾� ����
    void ExcuteDrawList();
public:
    virtual Vector3                 GetDistanceFromCamera(Camera* _camera) override { return Vector3(0, 0, 0); };
    virtual eBlendModeType          GetBlendMode() override { return eBlendModeType::OPAQUE_BLEND; };
    virtual eRasterizerStateType    GetCullingMode() override { return eRasterizerStateType::NONE_CULLING; };
    virtual void                    DrawObject(Matrix& _view, Matrix& _projection) override {};
    virtual void                    DrawShadow(Light* _pLight) override {};
    virtual void                    DrawWire() override;
public:
    Vector3 GetDistance(Transform* _transform);
public:
    inline void SetProjectionType(ProjectionType _type){ mProjectionType = _type; }
    inline void SetFovAngle(Degree _angle) { mFovAngle = _angle; }
    inline void SetProjectionNear(float _near) { mProjectionNear = _near; }
    inline void SetProjectionFar(float _far) { mProjectionFar = _far; }
    inline void SetSkyBox(SkyBox* _pSkybox) { mSkyBox = _pSkybox; }

    void ZoomToFov(float startFov, float endFov, float duration, Dotween::EasingEffect easingEffect = Dotween::EasingEffect::OutSine);

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

    float                   mFovAngle;
    float                   mProjectionNear;
    float                   mProjectionFar;

    // ������������ ����
    float                   mOrthoWidth;
    float                   mOrthoHeight;

    Vector2                 mSizeScale;
    Vector2                 mOffsetScale;

    D3DGraphicsViewport*    mLocalViewport;
    DrawQueue               mDrawQueue[BLEND_MODE_TYPE_COUNT];
    LightQueue              mSceneLights;

    ProjectionType          mProjectionType;
    CameraRenderType        mCameraRenderType;

    CameraCBuffer           mCameraCBuffer;
	LightCBuffer			mLightCBuffer;

    // ī�޶� ��ī�̹ڽ�
    SkyBox*                 mSkyBox;

private:
    float fovElapsedTime = 0.0f;
    float fovDuration = 0.0f;
    float startFov = 0.0f;
    float endFov = 0.0f;
    bool isZoomingFov = false;
    Dotween::EasingEffect fovEasingEffect;
      
public:
    virtual void EditorRendering(EditorViewerType _viewerType) override;
};
