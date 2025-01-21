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
    // 카메라에 Draw할 컴포넌트를 Push합니다.
    void PushDrawList(IRenderContext* _renderContext);
    // 카메라에 Light정보를 Push합니다. 매 업데이트마다 Push해줘야 함.
    void PushLight(Light* _pLight);
    // 렌더 큐의 Draw 작업 수행
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

    Degree                  mFovAngle;
    float                   mProjectionNear;
    float                   mProjectionFar;

    // 직교투영전용 변수
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

    // 카메라 스카이박스
    SkyBox*                 mSkyBox;
public:
    virtual void EditorRendering(EditorViewerType _viewerType) override;
};
