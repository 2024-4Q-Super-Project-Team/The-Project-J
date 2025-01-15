#include "pch.h"
#include "Camera.h"
#include "Manager/GameManager.h"
#include "ViewportScene/ViewportManager.h"
#include "ViewportScene/ViewportScene.h"
#include "Graphics/GraphicsManager.h"
#include "World/WorldManager.h"
#include "World/World.h"
#include "ObjectGroup/ObjectGroup.h"
#include "Object/Object.h"

#include "Resource/Graphics/Material/Material.h"
#include "Resource/Graphics/SkyBox/SkyBox.h"

Camera::Camera(Object* _owner, Vector2 _size)
    : Component(_owner)
    , mProjectionType(ProjectionType::Perspective)
    , mCameraRenderType(CameraRenderType::Forward)
    , mWidthScale(0), mHeightScale(0), mOffsetXScale(0), mOffsetYScale(0)
    , mFovAngle(XM_PIDIV2)
    , mProjectionNear(1.0f)
    , mProjectionFar(30000.0f)
    , mOrthoWidth(10.0f)
    , mOrthoHeight(10.0f)
    , mLocalViewport(nullptr)
    , mCameraViewport(nullptr)
{
    mType = eComponentType::CAMERA;
    mLocalViewport = new D3DGraphicsViewport(0.0f, 0.0f, 0.0f, 0.0f);

    mCameraViewport     = ViewportManager::GetActiveViewport();
    mMainViewport       = mCameraViewport->GetMainViewport();
    mMainRenderTarget   = mCameraViewport->GetMainRenderTarget();

    mWidthScale      = 1.0f;
    mHeightScale     = 1.0f;
    mOffsetXScale    = 0.0f;
    mOffsetYScale    = 0.0f;
}

Camera::~Camera()
{
    SAFE_RELEASE(mLocalViewport)
    SAFE_RELEASE(mMainRenderTarget)
}

void Camera::Start()
{
}

void Camera::Tick()
{
}

void Camera::FixedUpdate()
{
}

void Camera::PreUpdate()
{
}

void Camera::Update()
{
}

void Camera::PostUpdate()
{
}

void Camera::PreRender()
{
}

void Camera::Render()
{
    UpdateCamera();
    auto* world = GameManager::GetCurrentWorld();
    if (world)
    {
        mCameraCBuffer.Position.x = gameObject->transform->position.x;
        mCameraCBuffer.Position.y = gameObject->transform->position.y;
        mCameraCBuffer.Position.z = gameObject->transform->position.z;
        mCameraCBuffer.Position.w = 1.0f;
        mCameraCBuffer.View = XMMatrixTranspose(mViewMatrix);
        mCameraCBuffer.Projection = XMMatrixTranspose(mProjectionMatrix);
        mCameraCBuffer.InverseView = DirectX::XMMatrixInverse(nullptr, mViewMatrix);
        mCameraCBuffer.InverseView = XMMatrixTranspose(mCameraCBuffer.InverseView);
        mCameraCBuffer.InverseProjection = DirectX::XMMatrixInverse(nullptr, mProjectionMatrix);
        mCameraCBuffer.InverseProjection = XMMatrixTranspose(mCameraCBuffer.InverseProjection);
        // 카메라 상수버퍼 바인딩
        GraphicsManager::GetConstantBuffer(eCBufferType::Camera)->UpdateGPUResoure(&mCameraCBuffer);
        // 월드의 오브젝트를 그린다.
        world->Draw(this);
    }
}

void Camera::Draw(Camera* _camera)
{
}

void Camera::PostRender()
{
}

void Camera::SetCameraArea(UINT _offsetX, UINT _offsetY, UINT _width, UINT _height)
{
    mWidthScale = _width;
    mHeightScale = _height;
    mOffsetXScale = _offsetY;
    mOffsetYScale = _offsetY;
}

void Camera::SetCameraSize(UINT _width, UINT _height)
{
    mWidthScale = _width;
    mHeightScale = _height;
}

void Camera::SetCameraOffset(UINT _offsetX, UINT _offsetY)
{
    mOffsetXScale = _offsetY;
    mOffsetYScale = _offsetY;
}

void Camera::UpdateCamera()
{
    UpdateMatrix();
    UpdateViewport();
}

void Camera::UpdateMatrix()
{
    if (mWidthScale > 0.0f && mHeightScale > 0.0f)
    {
        Vector3 pos = gameObject->transform->GetWorldPosition();
        Vector3 dir = gameObject->transform->Forward();
        Vector3 up = Vector3::Up;
        Vector3 at = pos + dir;
        mViewMatrix = XMMatrixLookAtLH(pos, at, up);

        FLOAT widthRatio    = (FLOAT)mLocalViewport->GetWidth() * mWidthScale;
        FLOAT heightRatio   = (FLOAT)mLocalViewport->GetHeight() * mHeightScale;

        float aspectRatio   = widthRatio / heightRatio;

        switch (mProjectionType)
        {
        case ProjectionType::Perspective:
            mProjectionMatrix = XMMatrixPerspectiveFovLH(
                mFovAngle,
                aspectRatio,
                mProjectionNear,
                mProjectionFar);
            break;
        case ProjectionType::Orthographic:
            mProjectionMatrix = XMMatrixOrthographicLH(
                mOrthoWidth,
                mOrthoHeight / aspectRatio,
                mProjectionNear,
                mProjectionFar);
            break;
        default:
            break;
        }
    }
}

void Camera::UpdateViewport()
{
    if (mLocalViewport && mMainViewport)
    {
        mLocalViewport->SetWidth(mWidthScale * mMainViewport->GetWidth());
        mLocalViewport->SetHeight(mHeightScale * mMainViewport->GetHeight());
        mLocalViewport->SetOffsetX(mOffsetXScale * mMainViewport->GetWidth());
        mLocalViewport->SetOffsetY(mOffsetYScale * mMainViewport->GetHeight());
    }
}

void Camera::DrawShadow()
{
    GraphicsManager::GetVertexShader(eVertexShaderType::SHADOW)->Bind();
    GraphicsManager::GetPixelShader(ePixelShaderType::G_BUFFER)->Reset();

    for (int i = 0; i < mSceneLights.size(); i++)
    {
        auto rt = mSceneLights[i]->GetShadowRenderTarget();
        auto ShaderSRV = rt->GetSRV(rt->GetDSV());
        ShaderSRV->Reset();
        ShaderSRV->SetBindSlot(24 + i);
        // 해당 라이트의 뎁스 뷰, 뷰포트 등을 바인드해준다.
        mSceneLights[i]->GetShadowRenderTarget()->BeginDraw();
        mSceneLights[i]->GetShadowRenderTarget()->Clear();
        // 각 오브젝트에 대한 깊이 버퍼 Draw를 수행한다
        for (auto& drawQueue : mDrawQueue)
        {
            for (auto& drawInfo : drawQueue)
            {
                drawInfo->DrawShadow(mSceneLights[i]);
            }
        }
        mSceneLights[i]->GetShadowRenderTarget()->EndDraw();
        ShaderSRV->Bind();
    }
}

void Camera::DrawForward()
{
    GraphicsManager::GetVertexShader(eVertexShaderType::STANDARD)->Bind();
    GraphicsManager::GetPixelShader(ePixelShaderType::FOWARD_PBR)->Bind();

    for (int i = 0; i < BLEND_TYPE_COUNT; ++i)
    {
        //GraphicsManager::GetBlendState((eBlendType)i)->Bind();
        for (auto& drawInfo : mDrawQueue[i])
        {
            drawInfo->DrawMesh(this);
        }
        // 그리기 큐를 초기화한다.
        mDrawQueue[i].clear();
    }
}

void Camera::DrawDeferred()
{
    //Deferred Pass
    {   
        mDeferredRenderTarget->BeginDraw();
        mDeferredRenderTarget->Clear();

        // BlendState를 설정해줘야 OM이 rgb값을 a로 훼손시키지 않고 제대로 넣어준다.
        GraphicsManager::GetBlendState(eBlendType::OPAQUE_BLEND)->Bind();
        GraphicsManager::GetVertexShader(eVertexShaderType::STANDARD)->Bind();
        GraphicsManager::GetPixelShader(ePixelShaderType::G_BUFFER)->Bind();

        for (int i = 0; i < BLEND_TYPE_COUNT; ++i)
        {
            for (auto& drawInfo : mDrawQueue[i])
            {
                drawInfo->DrawMesh(this);
            }
            // 그리기 큐를 초기화한다.
            mDrawQueue[i].clear();
        }
        mDeferredRenderTarget->EndDraw();
    }

    // QuadFrame Pass
    GraphicsManager::GetBlendState(eBlendType::OPAQUE_BLEND)->Reset();
    GraphicsManager::GetVertexShader(eVertexShaderType::SPRITE)->Bind();
    GraphicsManager::GetPixelShader(ePixelShaderType::DEFERRED_PBR)->Bind();
    D3DGraphicsDefault::GetQuadFrameVertexBuffer()->Bind();
    D3DGraphicsDefault::GetQuadFrameIndexBuffer()->Bind();

    mDeferredRenderTarget->BindAllSRV();

    D3DGraphicsRenderer::DrawCall(6, 0, 0);

    mDeferredRenderTarget->ResetAllSRV();
}

void Camera::DrawSwapChain()
{
    // QuadFrame Pass
    GraphicsManager::GetVertexShader(eVertexShaderType::SPRITE)->Bind();
    GraphicsManager::GetPixelShader(ePixelShaderType::SPRITE)->Bind();
    D3DGraphicsDefault::GetQuadFrameVertexBuffer()->Bind();
    D3DGraphicsDefault::GetQuadFrameIndexBuffer()->Bind();

    mMainRenderTarget->BindAllSRV();

    D3DGraphicsRenderer::DrawCall(6, 0, 0);

    mMainRenderTarget->ResetAllSRV();
}

void Camera::SetProjectionType(ProjectionType _type)
{
    mProjectionType = _type;
}

void Camera::SetCameraRenderType(CameraRenderType _type)
{
    mCameraRenderType = _type;
    if (_type == CameraRenderType::Forward)
    {
        mDeferredRenderTarget.reset();
    }
    if (_type == CameraRenderType::Deferred)
    {
        mDeferredRenderTarget = mCameraViewport->GetDeferredRenderTarget();
    }
}

json Camera::Serialize()
{
    json ret;

    ret["fov angle"] = mFovAngle.GetAngle();
    ret["near"] = mProjectionNear;
    ret["far"] = mProjectionFar;
    ret["type"] = mProjectionType;
    ret["ortho width"] = mOrthoWidth;
    ret["ortho height"] = mOrthoHeight;

    return ret;
}

D3DBitmapRenderTarget* Camera::GetCurrentRenderTarget()
{
    if (mMainRenderTarget)
    {
        return mMainRenderTarget.get();
    }
    return nullptr;
}

void Camera::PushDrawList(RendererComponent* _renderComponent)
{
    if (_renderComponent == nullptr) return;
    auto pMaterial = _renderComponent->GetMaterial();
    eBlendType blendMode = eBlendType::OPAQUE_BLEND;
    if (pMaterial)
    {
        blendMode = pMaterial->mMaterialResource->mBlendMode;
    }
    mDrawQueue[static_cast<UINT>(blendMode)].push_back(_renderComponent);
}

void Camera::PushLight(Light* _pLight)
{
    if (_pLight == nullptr) return;
    mSceneLights.push_back(_pLight);
}

void Camera::ExcuteDrawList()
{
    if (mWidthScale > 0.0f && mHeightScale > 0.0f)
    {
        if (mMainRenderTarget)
        {
            {
                mMainRenderTarget->BeginDraw();
                mMainRenderTarget->Clear();
                ////////////////////////////////////////////////////
                // 그림자 연산
                ////////////////////////////////////////////////////
                DrawShadow();

                mMainViewport->Bind();  // 메인 윈도우와 대응되는 뷰포트

                // 렌더타입으로 Draw실행
                switch (mCameraRenderType)
                {
                case CameraRenderType::Forward:
                    DrawForward();
                    break;
                case CameraRenderType::Deferred:
                    DrawDeferred();
                    break;
                default:
                    break;
                }

                ////////////////////////////////////////////////////
                // SkyBox Draw
                ////////////////////////////////////////////////////
                // 스카이박스 렌더 (최적화를 위해 마지막에 렌더링)
                if (mSkyBox)
                {
                    mSkyBox->Draw(this);
                }

                mMainRenderTarget->EndDraw();
            }

            mLocalViewport->Bind();  // 카메라 출력될 영역 사이즈
            DrawSwapChain();
        }
    }
    // 조명 리스트를 초기화한다.
    mSceneLights.clear();
}

void Camera::EditorRendering()
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    if (ImGui::CollapsingHeader(("Camera" + uid).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        bool isForward = mCameraRenderType == CameraRenderType::Forward ? true : false;
        if (ImGui::Checkbox("isForward", &isForward))
        {
            if (isForward)
            {
                SetCameraRenderType(CameraRenderType::Forward);
            }
            else
            {
                SetCameraRenderType(CameraRenderType::Deferred);
            }
        }
        FLOAT Area[4] = { mWidthScale, mHeightScale, mOffsetXScale, mOffsetYScale };

        ImGui::Text(("RTVWidth : " + std::to_string(mMainRenderTarget->GetWidth())).c_str());
        ImGui::Text(("RTVHeight : " + std::to_string(mMainRenderTarget->GetHeight())).c_str());
        ImGui::Separator();
        ImGui::Text(("RectWidth : " + std::to_string(mLocalViewport->GetWidth())).c_str());
        if (ImGui::SliderFloat(("WidthScale : " + uid).c_str(), &Area[0], 0.0f, 1.0f))
            mWidthScale = Area[0];

        ImGui::Text(("RectHeight : " + std::to_string(mLocalViewport->GetHeight())).c_str());
        if(ImGui::SliderFloat(("HeightScale : " + uid).c_str(), &Area[1], 0.0f, 1.0f))
            mHeightScale = Area[1];

        ImGui::Text(("RectOffsetX : " + std::to_string(mLocalViewport->GetOffsetX())).c_str());
        if(ImGui::SliderFloat(("OffsetXScale : " + uid).c_str(), &Area[2], 0.0f, 1.0f))
            mOffsetXScale = Area[2];

        ImGui::Text(("RectOffsetY : " + std::to_string(mLocalViewport->GetOffsetY())).c_str());
        if(ImGui::SliderFloat(("OffsetYScale : " + uid).c_str(), &Area[3], 0.0f, 1.0f))
            mOffsetYScale = Area[3];

        ImGui::Separator();

        ImGui::Text("Projection");
        ImGui::SliderFloat(("fovAngle" + uid).c_str(), mFovAngle, 1.0f, Degree::MaxDegree);
        ImGui::SliderFloat(("Near" + uid).c_str(), &mProjectionNear, 0.1f, 1000.0f);
        ImGui::SliderFloat(("Far" + uid).c_str(), &mProjectionFar, 1.0f, 100000.0f);

        if (mCameraRenderType == CameraRenderType::Deferred && mDeferredRenderTarget)
        {
            ImGui::Separator();
            ImGui::Text("Albedo + Opacity ( rgb(Albedo), a(Opacity) )");
            auto CameraRTV = mDeferredRenderTarget->GetRTV();
            auto CameraSRV = mDeferredRenderTarget->GetSRV(CameraRTV);
            ImGui::Image((ImTextureID)CameraSRV->mSRV, ImVec2(200, 200));

            ImGui::Separator();
            ImGui::Text("Normal + Depth ( rgb(Normal), a(Depth) )");
            CameraRTV = mDeferredRenderTarget->GetRTV(1);
            CameraSRV = mDeferredRenderTarget->GetSRV(CameraRTV);
            ImGui::Image((ImTextureID)CameraSRV->mSRV, ImVec2(200, 200));

            ImGui::Separator();
            ImGui::Text("Material ( r(Metalness), g(Roughness), b(Specular), a(AmbientOcclusion) )");
            CameraRTV = mDeferredRenderTarget->GetRTV(2);
            CameraSRV = mDeferredRenderTarget->GetSRV(CameraRTV);
            ImGui::Image((ImTextureID)CameraSRV->mSRV, ImVec2(200, 200));

            ImGui::Separator();
            ImGui::Text("Emessive ( rgb(Emessive) )");
            CameraRTV = mDeferredRenderTarget->GetRTV(3);
            CameraSRV = mDeferredRenderTarget->GetSRV(CameraRTV);
            ImGui::Image((ImTextureID)CameraSRV->mSRV, ImVec2(200, 200));

            ImGui::Separator();
            ImGui::Text("WorldPosition ( rgb(WorldPosition) )");
            CameraRTV = mDeferredRenderTarget->GetRTV(4);
            CameraSRV = mDeferredRenderTarget->GetSRV(CameraRTV);
            ImGui::Image((ImTextureID)CameraSRV->mSRV, ImVec2(200, 200));
        }
        if(mMainRenderTarget)
        {
            ImGui::Separator();
            ImGui::Text("Output");
            auto CameraRTV = mMainRenderTarget->GetRTV();
            auto CameraSRV = mMainRenderTarget->GetSRV(CameraRTV);
            ImGui::Image((ImTextureID)CameraSRV->mSRV, ImVec2(200, 200));
        }
       
    }
}