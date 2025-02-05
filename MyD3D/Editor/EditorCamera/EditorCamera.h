#pragma once

class RendererComponent;

class EditorCamera
{
public:
    EditorCamera();
public:
    void EditorUpdate();
    void EditorRender();
public:
    void UpdatePosition();
    void UpdateRotation(); 
    void UpdateMatrix();
    void UpdateCameraBuffer();
    void UpdateCamera();
public:
    void PushDrawList(IRenderContext* _renderContext);
    void PushLightList(Light* _lightComponent);
    void PushWireList(IRenderContext* _renderContext);
    void PushOutlineDrawList(IRenderContext* _renderContext);
    void ExcuteDrawList();
public:
    void DrawShadow();
    void DrawObject();
    void DrawWire();
    void DrawOutlineList();
    void DrawSwapChain();
public:
    // 현재 카메라가 쓰고 있는 리소스 뷰
    std::shared_ptr<D3DGraphicsViewport>        mMainViewport;
    std::shared_ptr<D3DBitmapRenderTarget>      mMainRenderTarget;

    Vector3                 mPosition = Vector3(0.0f, 100.0f, -100.0f);
    Matrix                  mRotation = Matrix::Identity;

    Vector3                 mDirection = Vector3::Backward;
    FLOAT                   mCameraMoveSpeed = 1000.0f;
    FLOAT                   mCameraRotateSpeed = 4.0f;
    FLOAT                   mCameraMaxAngle = 1.0f;

    Vector2                 mSize = Vector2::Zero;

    Matrix                  mViewMatrix = Matrix::Identity;
    Matrix                  mProjectionMatrix = Matrix::Identity;

    Degree                  mFovAngle = XM_PIDIV2;
    float                   mProjectionNear = 1.0f;
    float                   mProjectionFar = 30000.0f;

    DrawQueue               mDrawQueue[BLEND_MODE_TYPE_COUNT];
    LightQueue              mSceneLights;

    CameraCBuffer           mCameraCBuffer;
    LightCBuffer			mLightCBuffer;

    UINT                    mDrawedMeshCount;
    UINT                    mDrawedLightCount;
    bool                    mIsSkyBoxRendering = FALSE;
    bool                    mIsColliderRendering = TRUE;
public:
    void EditorRendering(EditorViewerType _viewerType);
};

