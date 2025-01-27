#include "pch.h"
#include "Camera.h"
#include "Manager/GameManager.h"
#include "ViewportScene/ViewportManager.h"
#include "Resource/ResourceManager.h"
#include "Graphics/GraphicsManager.h"
#include "World/WorldManager.h"

#include "ViewportScene/ViewportScene.h"
#include "World/World.h"
#include "Object/Object.h"
#include "SkyBox/SkyBox.h"

Camera::Camera(Object* _owner, Vector2 _size)
    : Component(_owner)
    , mProjectionType(ProjectionType::Perspective)
    , mCameraRenderType(CameraRenderType::Forward)
    , mSizeScale(Vector2::One)
    , mOffsetScale(Vector2::Zero)
    , mFovAngle(XM_PIDIV2)
    , mProjectionNear(1.0f)
    , mProjectionFar(30000.0f)
    , mOrthoWidth(10.0f)
    , mOrthoHeight(10.0f)
    , mLocalViewport(nullptr)
    , mCameraViewport(nullptr)
    , mSkyBox(SkyBox::GetDefaultSkyBox())
{
    SetEID("Camera");
    mType = eComponentType::CAMERA;
    mLocalViewport = new D3DGraphicsViewport(0.0f, 0.0f, 0.0f, 0.0f);
}

Camera::~Camera()
{
    SAFE_RELEASE(mLocalViewport)
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
    UpdateZSort();

    ExcuteDrawList();
}

void Camera::Draw(Camera* _camera)
{
}

void Camera::PostRender()
{
}

void Camera::EditorUpdate()
{
}

void Camera::EditorRender()
{
}

void Camera::SetCameraSize(Vector2 _sizeScale)
{
    mSizeScale = _sizeScale;
}

void Camera::SetCameraOffset(Vector2 _offsetScale)
{
	mOffsetScale = _offsetScale;
}

void Camera::UpdateCamera()
{
    if (mCameraViewport == nullptr)
    {
        mCameraViewport = ViewportManager::GetActiveViewport();
    }
    if (mCameraViewport)
    {
        mMainViewport = mCameraViewport->GetMainViewport();
        mMainRenderTarget = mCameraViewport->GetMainRenderTarget();
        mDeferredRenderTarget = mCameraViewport->GetDeferredRenderTarget();
    }
    UpdateMatrix();
    UpdateViewport();
}

void Camera::UpdateMatrix()
{
    if (mSizeScale.Length() > 0.05f)
    {
        Vector3 pos = gameObject->transform->GetWorldPosition();
        Vector3 dir = gameObject->transform->Forward();
        Vector3 up = Vector3::Up;
        Vector3 at = pos + dir;
        mViewMatrix = XMMatrixLookAtLH(pos, at, up);

        FLOAT widthRatio    = (FLOAT)mLocalViewport->GetWidth() * mSizeScale.x;
        FLOAT heightRatio   = (FLOAT)mLocalViewport->GetHeight() * mSizeScale.y;

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
        mLocalViewport->SetWidth(mSizeScale.x * mMainViewport->GetWidth());
        mLocalViewport->SetHeight(mSizeScale.y * mMainViewport->GetHeight());
        mLocalViewport->SetOffsetX(mOffsetScale.x * mMainViewport->GetWidth());
        mLocalViewport->SetOffsetY(mOffsetScale.y * mMainViewport->GetHeight());
    }
}

void Camera::UpdateZSort()
{
    std::sort(mDrawQueue[(UINT)eBlendModeType::TRANSPARENT_BLEND].begin(),
        mDrawQueue[(UINT)eBlendModeType::TRANSPARENT_BLEND].end(),
        [this](IRenderContext*& a, IRenderContext*& b) {
            return a->GetDistanceFromCamera(this).z < b->GetDistanceFromCamera(this).z; // Z축 기준 내림차순 정렬 (멀리 있는 것부터)
        });
}

void Camera::DrawShadow()
{
    GraphicsManager::GetRasterizerState(eRasterizerStateType::BACKFACE_CULLING)->Bind();
    GraphicsManager::GetVertexShader(eVertexShaderType::SHADOW)->Bind();
    GraphicsManager::GetPixelShader(ePixelShaderType::G_BUFFER)->Reset();

    for (int i = 0; i < mSceneLights.size(); i++)
    {
        if (mSceneLights[i]->GetProperty().UseShadow)
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
}

void Camera::DrawForward()
{
    GraphicsManager::GetVertexShader(eVertexShaderType::STANDARD)->Bind();
    GraphicsManager::GetPixelShader(ePixelShaderType::FOWARD_PBR)->Bind();

    ////////////////////////////////////////////////////////////////
    // OpaQue Blend
    ////////////////////////////////////////////////////////////////
    GraphicsManager::GetBlendState(eBlendStateType::DEFAULT)->Bind();
    for (auto& drawInfo : mDrawQueue[(UINT)eBlendModeType::OPAQUE_BLEND])
    {
        drawInfo->DrawObject(mViewMatrix, mProjectionMatrix);
    }
    mDrawQueue[(UINT)eBlendModeType::OPAQUE_BLEND].clear();
    ////////////////////////////////////////////////////////////////
    // Transparent Blend
    ////////////////////////////////////////////////////////////////
    GraphicsManager::GetBlendState(eBlendStateType::ALPHA)->Bind();
    for (auto& drawInfo : mDrawQueue[(UINT)eBlendModeType::TRANSPARENT_BLEND])
    {
        drawInfo->DrawObject(mViewMatrix, mProjectionMatrix);
    }
    mDrawQueue[(UINT)eBlendModeType::TRANSPARENT_BLEND].clear();

    GraphicsManager::GetBlendState(eBlendStateType::ALPHA)->Reset();
}

void Camera::DrawDeferred()
{
    // Deferred Pass
    // 디퍼드는 Opaque만 디퍼드로 그린다. 알파큐는 포워드로 그려야함
    {   
        mDeferredRenderTarget->BeginDraw();
        mDeferredRenderTarget->Clear();

        // BlendState를 설정해줘야 OM이 rgb값을 a로 훼손시키지 않고 제대로 넣어준다.
        GraphicsManager::GetVertexShader(eVertexShaderType::STANDARD)->Bind();
        GraphicsManager::GetPixelShader(ePixelShaderType::G_BUFFER)->Bind();

        ////////////////////////////////////////////////////////////////
        // OpaQue Blend
        ////////////////////////////////////////////////////////////////
        GraphicsManager::GetBlendState(eBlendStateType::DEFAULT)->Bind();
        for (auto& drawInfo : mDrawQueue[(UINT)eBlendModeType::OPAQUE_BLEND])
        {
            drawInfo->DrawObject(mViewMatrix, mProjectionMatrix);
        }
        mDrawQueue[(UINT)eBlendModeType::OPAQUE_BLEND].clear();
        ////////////////////////////////////////////////////////////////
        // Transparent Blend
        ////////////////////////////////////////////////////////////////
        GraphicsManager::GetBlendState(eBlendStateType::ALPHA)->Bind();
        for (auto& drawInfo : mDrawQueue[(UINT)eBlendModeType::TRANSPARENT_BLEND])
        {
            drawInfo->DrawObject(mViewMatrix, mProjectionMatrix);
        }
        mDrawQueue[(UINT)eBlendModeType::TRANSPARENT_BLEND].clear();

        mDeferredRenderTarget->EndDraw();
        GraphicsManager::GetBlendState(eBlendStateType::ALPHA)->Reset();
    }

    // QuadFrame Pass
    GraphicsManager::GetVertexShader(eVertexShaderType::SPRITE)->Bind();
    GraphicsManager::GetPixelShader(ePixelShaderType::DEFERRED_PBR)->Bind();
    D3DGraphicsDefault::GetQuadFrameVertexBuffer()->Bind();
    D3DGraphicsDefault::GetQuadFrameIndexBuffer()->Bind();

    mDeferredRenderTarget->BindAllSRV();

    D3DGraphicsRenderer::DrawCall(6, 0, 0);

    mDeferredRenderTarget->ResetAllSRV();
}

void Camera::DrawWire()
{
    DebugRenderer::BeginDraw();
    DebugRenderer::UpdateViewProjection(mViewMatrix, mProjectionMatrix);

    for (auto& drawInfo : mDrawQueue[(UINT)eBlendModeType::WIREFRAME_BELND])
    {
        drawInfo->DrawWire();
    }
    mDrawQueue[(UINT)eBlendModeType::WIREFRAME_BELND].clear();

    DebugRenderer::EndDraw();
}

void Camera::DrawSwapChain()
{
    // QuadFrame Pass
    GraphicsManager::GetRasterizerState(eRasterizerStateType::NONE_CULLING)->Bind();
    GraphicsManager::GetVertexShader(eVertexShaderType::SPRITE)->Bind();
    GraphicsManager::GetPixelShader(ePixelShaderType::SPRITE)->Bind();
    D3DGraphicsDefault::GetQuadFrameVertexBuffer()->Bind();
    D3DGraphicsDefault::GetQuadFrameIndexBuffer()->Bind();

    mMainRenderTarget->BindAllSRV();

    D3DGraphicsRenderer::DrawCall(6, 0, 0);

    mMainRenderTarget->ResetAllSRV();
}

void Camera::ExcuteDrawList()
{
    GraphicsManager::GetConstantBuffer(eCBufferType::Light)->UpdateGPUResoure(&mLightCBuffer);
    if (mSizeScale.Length() > 0.05f)
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
                    mSkyBox->Draw(mViewMatrix, mProjectionMatrix, mProjectionFar);
                }

                mMainRenderTarget->EndDraw();
            }

            mLocalViewport->Bind();  // 카메라 출력될 영역 사이즈
            DrawSwapChain();
            ////////////////////////////////////////////////////
            // WireFrame Draw
            ////////////////////////////////////////////////////
            DrawWire();
        }
    }
    // 조명 리스트를 초기화한다.
    mSceneLights.clear();
    mLightCBuffer.NumLight = 0;
}

json Camera::Serialize()
{
    json ret;

    ret["id"] = GiveId();
    ret["name"] = "Camera";
    ret["fov angle"] = mFovAngle;
    ret["near"] = mProjectionNear;
    ret["far"] = mProjectionFar;
    ret["type"] = mProjectionType;
    ret["ortho width"] = mOrthoWidth;
    ret["ortho height"] = mOrthoHeight;

    return ret;
}

void Camera::Deserialize(json& j)
{
    SetId(j["id"].get<unsigned int>());

    if(j.contains("fov angle"))
        mFovAngle = j["fov angle"].get<float>();
    if (j.contains("near"))
        mProjectionNear = j["near"].get<float>();
    if (j.contains("far"))
        mProjectionFar = j["far"].get<float>();
    if (j.contains("type"))
        mProjectionType = static_cast<ProjectionType>(j["type"].get<int>());
    if (j.contains("ortho width"))
        mOrthoWidth = j["ortho width"].get<float>();
    if (j.contains("ortho height"))
        mOrthoHeight = j["ortho height"].get<float>();
}

Vector3 Camera::GetDistance(Transform* _transform)
{
    Matrix cameraSpaceMatrix = mViewMatrix * _transform->GetWorldMatrix();
    return cameraSpaceMatrix.Translation();
}

D3DBitmapRenderTarget* Camera::GetCurrentRenderTarget()
{
    if (mMainRenderTarget)
    {
        return mMainRenderTarget.get();
    }
    return nullptr;
}

void Camera::PushDrawList(IRenderContext* _renderContext)
{
    if (_renderContext == nullptr) return;
    eBlendModeType blendMode = _renderContext->GetBlendMode();
    mDrawQueue[static_cast<UINT>(blendMode)].push_back(_renderContext);
}

void Camera::PushLightList(Light* _pLight)
{
    if (_pLight == nullptr) return;
    if (mLightCBuffer.NumLight < MAX_LIGHT_COUNT)
    {
        mLightCBuffer.LightProp[mLightCBuffer.NumLight] = _pLight->GetProperty();
        mLightCBuffer.NumLight++;
        mSceneLights.push_back(_pLight);
    }
}

void Camera::PushWireList(IRenderContext* _renderContext)
{
    if (_renderContext == nullptr) return;
    eBlendModeType blendMode = eBlendModeType::WIREFRAME_BELND;
    mDrawQueue[static_cast<UINT>(blendMode)].push_back(_renderContext);
}

void Camera::EditorRendering(EditorViewerType _viewerType)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

    const char* renderMode[] = { "Forward", "Deferred" };
    int SelectIndex = (int)mCameraRenderType; // 현재 선택된 항목 (인덱스)

    ImGui::Text("RenderMode : ");
    if (ImGui::Combo((uid + "RenderMode").c_str(), &SelectIndex, renderMode, IM_ARRAYSIZE(renderMode)))
    {
        mCameraRenderType = (CameraRenderType)SelectIndex;
    }

    ImGui::Separator();

    UINT size[2] = {
        mSizeScale.x * mLocalViewport->GetWidth(),
        mSizeScale.y * mLocalViewport->GetHeight()
    };
    UINT offset[2] = {
        mOffsetScale.x * mLocalViewport->GetWidth(),
        mOffsetScale.y * mLocalViewport->GetHeight()
    };
    ImGui::Text(("SizeScale : (" + std::to_string(size[0]) + ", " + std::to_string(size[1]).c_str() + ")").c_str());
    ImGui::SliderFloat2((uid + "SizeScale").c_str(), &mSizeScale.x, 0.0f, 1.0f);
    ImGui::Text(("OffsetScale : (" + std::to_string(offset[0]) + ", " + std::to_string(offset[1]).c_str() + ")").c_str());
    ImGui::SliderFloat2((uid + "OffsetScale").c_str(), &mOffsetScale.x, 0.0f, 1.0f);

    ImGui::Separator();

    ImGui::Text("fovAngle : ");
    ImGui::SliderFloat((uid + "fovAngle").c_str(), &mFovAngle, 1.0f, Degree::MaxDegree);
    ImGui::Text("Near : ");
    ImGui::SliderFloat((uid + "Near").c_str(), &mProjectionNear, 0.1f, 1000.0f);
    ImGui::Text("Far : ");
    ImGui::SliderFloat((uid + "Far").c_str(), &mProjectionFar, 1.0f, 100000.0f);

    if (mCameraRenderType == CameraRenderType::Deferred && mDeferredRenderTarget)
    {
        ImGui::Separator();

        ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_EXTRA);
        if (ImGui::TreeNodeEx(("Deferred View" + uid).c_str(), ImGuiTreeNodeFlags_Selected))
        {
            //ImGui::Text("Albedo + Opacity ( rgb(Albedo), a(Opacity) )");
            auto CameraRTV = mDeferredRenderTarget->GetRTV();
            auto CameraSRV = mDeferredRenderTarget->GetSRV(CameraRTV);
            ImGui::Image((ImTextureID)CameraSRV->mSRV, ImVec2(150, 150));
            ImGui::SameLine();
            //ImGui::Text("Normal + Depth ( rgb(Normal), a(Depth) )");
            CameraRTV = mDeferredRenderTarget->GetRTV(1);
            CameraSRV = mDeferredRenderTarget->GetSRV(CameraRTV);
            ImGui::Image((ImTextureID)CameraSRV->mSRV, ImVec2(150, 150));

            //ImGui::Text("Material ( r(Metalness), g(Roughness), b(Specular), a(AmbientOcclusion) )");
            CameraRTV = mDeferredRenderTarget->GetRTV(2);
            CameraSRV = mDeferredRenderTarget->GetSRV(CameraRTV);
            ImGui::Image((ImTextureID)CameraSRV->mSRV, ImVec2(150, 150));
            ImGui::SameLine();
            //ImGui::Text("Emessive ( rgb(Emessive) )");
            CameraRTV = mDeferredRenderTarget->GetRTV(3);
            CameraSRV = mDeferredRenderTarget->GetSRV(CameraRTV);
            ImGui::Image((ImTextureID)CameraSRV->mSRV, ImVec2(150, 150));

            //ImGui::Text("WorldPosition ( rgb(WorldPosition) )");
            CameraRTV = mDeferredRenderTarget->GetRTV(4);
            CameraSRV = mDeferredRenderTarget->GetSRV(CameraRTV);
            ImGui::Image((ImTextureID)CameraSRV->mSRV, ImVec2(150, 150));

            ImGui::TreePop();
        }
        EDITOR_COLOR_POP(1);
    }
    if (mMainRenderTarget)
    {
        ImGui::Separator();

        ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_EXTRA);
        if (ImGui::TreeNodeEx(("Output View" + uid).c_str(), ImGuiTreeNodeFlags_Selected))
        {
            auto CameraRTV = mMainRenderTarget->GetRTV();
            auto CameraSRV = mMainRenderTarget->GetSRV(CameraRTV);
            ImGui::Image((ImTextureID)CameraSRV->mSRV, ImVec2(150, 150));

            ImGui::TreePop();
        }
        EDITOR_COLOR_POP(1);
    }
}