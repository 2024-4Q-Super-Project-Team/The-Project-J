#pragma once
#include "Component/Component.h"

class D3DBitmapRenderTarget;
class D3DGraphicsViewport;
class RendererComponent;
class Light;
class SkyBox;
class ViewportScene;

using DrawQueue = std::vector<RendererComponent*>;
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
public:
    void SetCameraArea(UINT _offsetX, UINT _offsetY, UINT _width, UINT _height);
    void SetCameraSize(UINT _width, UINT _height);
    void SetCameraOffset(UINT _offsetX, UINT _offsetY);
    // 카메라에 Draw할 컴포넌트를 Push합니다.
    void PushDrawList(RendererComponent* _renderComponent);
    // 카메라에 Light정보를 Push합니다. 매 업데이트마다 Push해줘야 함.
    void PushLight(Light* _pLight);
    // 렌더 큐의 Draw 작업 수행
    void ExcuteDrawList(); 

    //static QueryReSizeRenderTarget();
private:
    void UpdateCamera();
    void GetMainViewport(ViewportScene* _pViewport);
    void GetMainRenderTarget(ViewportScene* _pViewport);
    void GetDeferredRenderTarget(ViewportScene* _pViewport);
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
    inline void SetSkyBox(std::shared_ptr<SkyBox> _pSkybox) { mSkyBox = _pSkybox; }

    inline const Matrix& GetView() { return mViewMatrix; }
    inline const Matrix& GetProjection() { return mProjectionMatrix; }
    inline const float GetProjectionNear() { return mProjectionNear; }
    inline const float GetProjectionFar() { return mProjectionFar; }
    inline const auto* GetViewport() { return mViewport; }

    D3DBitmapRenderTarget* GetCurrentRenderTarget();
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override {}
private:
    // 공유할 리소스(윈도우마다 사이즈별로 하나씩 만들어주고, 뷰포트를 다르게 해서 사용)
    static std::unordered_map<ViewportScene*, std::weak_ptr<D3DGraphicsViewport>>   gCameraMainViewportTable;
    static std::unordered_map<ViewportScene*, std::weak_ptr<D3DBitmapRenderTarget>> gCameraMainRenderTargetTable;
    static std::unordered_map<ViewportScene*, std::weak_ptr<D3DBitmapRenderTarget>> gCameraDeferredRenderTargetTable;

    // 현재 카메라가 속한 Viewport씬
    ViewportScene* mCameraViewport;
    // 현재 카메라가 쓰고 있는 리소스
    std::shared_ptr<D3DGraphicsViewport>        mMainViewport;
    std::shared_ptr<D3DBitmapRenderTarget>      mMainRenderTarget;
    std::shared_ptr<D3DBitmapRenderTarget>      mDeferredRenderTarget;

    DrawQueue               mDrawQueue[BLEND_TYPE_COUNT];
    LightQueue              mSceneLights;

    CameraCBuffer           mCameraCBuffer;
    ProjectionType          mProjectionType;
    CameraRenderType        mCameraRenderType;

    Matrix                  mViewMatrix;
    Matrix                  mProjectionMatrix;

    //float mAspectRatio 종횡비

    Degree                  mFovAngle;
    float                   mProjectionNear;
    float                   mProjectionFar;

    // 직교투영전용 변수
    float                   mOrthoWidth;
    float                   mOrthoHeight;

    D3DGraphicsViewport* mViewport;
    UINT mWidth;
    UINT mHeight;
    UINT mOffsetX;
    UINT mOffsetY;

    // 카메라 스카이박스
    std::shared_ptr<SkyBox> mSkyBox;
public:
    virtual void EditorRendering() override;
};
