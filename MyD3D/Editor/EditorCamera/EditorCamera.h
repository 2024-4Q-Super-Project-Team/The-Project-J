#pragma once

class RendererComponent;

class EditorCamera
{
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
    void PushDrawList(RendererComponent* _renderComponent);
    void PushLightList(Light* _lightComponent);
    void ExcuteDrawList();
public:
    void DrawShadow();
    void DrawMesh();
    void DrawSwapChain();
public:
    // 현재 카메라가 쓰고 있는 리소스 뷰
    std::shared_ptr<D3DGraphicsViewport>        mMainViewport;
    std::shared_ptr<D3DBitmapRenderTarget>      mMainRenderTarget;

    FLOAT                   mCameraMoveSpeed = 10.0f;
    FLOAT                   mCameraRotateSpeed = 10.0f;

    Vector2                 mSize = Vector2::Zero;
    Vector3                 mPosition = Vector3(0.0f,100.0f,-100.0f);
    Vector3                 mDirection = Vector3::Backward;
    Vector3                 mUp = Vector3::Up;

    Matrix                  mViewMatrix = Matrix::Identity;
    Matrix                  mProjectionMatrix = Matrix::Identity;

    Degree                  mFovAngle = XM_PIDIV2;
    float                   mProjectionNear = 1.0f;
    float                   mProjectionFar = 30000.0f;

    DrawQueue               mDrawQueue[BLEND_TYPE_COUNT];
    LightQueue              mSceneLights;

    CameraCBuffer           mCameraCBuffer;
    LightCBuffer			mLightCBuffer;

    UINT                    mDrawedMeshCount;
    UINT                    mDrawedLightCount;
    bool                    mIsSkyBoxRendering = TRUE;
public:
    void EditorRendering(EditorViewerType _viewerType);
};

