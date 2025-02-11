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
    // 카메라에 Draw할 컴포넌트를 Push합니다.
    void PushDrawList(IRenderContext* _renderContext);
    // 카메라에 Outline을 Draw할 컴포넌트를 Push 합니다.
    void PushOutlineDrawList(IRenderContext* _renderContext);
    // 카메라에 Light정보를 Push합니다. 매 업데이트마다 Push해줘야 함.
    void PushLightList(Light* _pLight);
    // 카메라에 그릴 와이어프레임 정보를 Push합니다.
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
    // 렌더 큐의 Draw 작업 수행
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
    // 뷰포트의 자원을 공유받는다.
    /////////////////////////////////////////////
    // 현재 카메라가 속한 Viewport씬
    ViewportScene* mCameraViewport;
    // 현재 카메라가 쓰고 있는 리소스 뷰
    std::shared_ptr<D3DGraphicsViewport>        mMainViewport;
    std::shared_ptr<D3DBitmapRenderTarget>      mMainRenderTarget;
    std::shared_ptr<D3DBitmapRenderTarget>      mDeferredRenderTarget;
private:
    Matrix                  mViewMatrix;
    Matrix                  mProjectionMatrix;

    float                   mFovAngle;
    float                   mProjectionNear;
    float                   mProjectionFar;

    // 직교투영전용 변수
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

    // 카메라 스카이박스
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
