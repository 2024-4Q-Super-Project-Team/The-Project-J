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
    // ī�޶� Draw�� ������Ʈ�� Push�մϴ�.
    void PushDrawList(RendererComponent* _renderComponent);
    // ī�޶� Light������ Push�մϴ�. �� ������Ʈ���� Push����� ��.
    void PushLight(Light* _pLight);
    // ���� ť�� Draw �۾� ����
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
    // ������ ���ҽ�(�����츶�� ������� �ϳ��� ������ְ�, ����Ʈ�� �ٸ��� �ؼ� ���)
    static std::unordered_map<ViewportScene*, std::weak_ptr<D3DGraphicsViewport>>   gCameraMainViewportTable;
    static std::unordered_map<ViewportScene*, std::weak_ptr<D3DBitmapRenderTarget>> gCameraMainRenderTargetTable;
    static std::unordered_map<ViewportScene*, std::weak_ptr<D3DBitmapRenderTarget>> gCameraDeferredRenderTargetTable;

    // ���� ī�޶� ���� Viewport��
    ViewportScene* mCameraViewport;
    // ���� ī�޶� ���� �ִ� ���ҽ�
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

    //float mAspectRatio ��Ⱦ��

    Degree                  mFovAngle;
    float                   mProjectionNear;
    float                   mProjectionFar;

    // ������������ ����
    float                   mOrthoWidth;
    float                   mOrthoHeight;

    D3DGraphicsViewport* mViewport;
    UINT mWidth;
    UINT mHeight;
    UINT mOffsetX;
    UINT mOffsetY;

    // ī�޶� ��ī�̹ڽ�
    std::shared_ptr<SkyBox> mSkyBox;
public:
    virtual void EditorRendering() override;
};
