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

Camera::Camera(Object* _owner, Vector2 _size)
    : Component(_owner)
    , mProjectionType(ProjectionType::Perspective)
    , mFovAngle(XM_PIDIV2)
    , mProjectionNear(1.0f)
    , mProjectionFar(30000.0f)
    , mOrthoWidth(10.0f)
    , mOrthoHeight(10.0f)
    , mViewport(nullptr)
{
    mType = eComponentType::CAMERA;
    mViewport = new D3DGraphicsViewport(0.0f, 0.0f, _size.x, _size.y);
}

Camera::~Camera()
{
    SAFE_RELEASE(mViewport)
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
        // 카메라 상수버퍼 바인딩
        GraphicsManager::GetConstantBuffer(eCBufferType::Camera)->UpdateGPUResoure(&mCameraCBuffer);
        // 뷰포트 바인딩
        mViewport->Bind();
        // 월드의 오브젝트를 그린다.
        world->Draw(this);
    }

    // 렌더 큐를 처리한다.
    ExcuteDrawList();
}

void Camera::Draw(Camera* _camera)
{
}

void Camera::PostRender()
{
}

void Camera::UpdateCamera()
{
    Vector3 pos = gameObject->transform->position;
    Vector3 dir = gameObject->transform->Forward();
    Vector3 up = Vector3::Up;
    Vector3 at = pos + dir;
    mViewMatrix = XMMatrixLookAtLH(pos, at, up);

    // 가로 세로 비율?
    ViewportManager* vptMng = GameManager::GetViewportManager();
    float aspectRatio = mViewport->GetWidth() / mViewport->GetHeight();

    switch (mProjectionType)
    {
    case Camera::ProjectionType::Perspective:
        mProjectionMatrix = XMMatrixPerspectiveFovLH(
            mFovAngle,
            aspectRatio,
            mProjectionNear,
            mProjectionFar);
        break;
    case Camera::ProjectionType::Orthographic:
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

void Camera::PushDrawList(RendererComponent* _renderComponent)
{
    if (_renderComponent == nullptr) return;
    auto pMaterial = _renderComponent->GetMaterial();
    eBlendingMode blendMode = eBlendingMode::OPAQUE_BLEND;
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
    // 0. 이전 렌더타겟을 저장해놓는다.
    auto RTV = D3DGraphicsRenderer::GetCurrentRTV();
    auto DSV = D3DGraphicsRenderer::GetCurrentDSV();
    // 1. 그림자용 셰이더 바인딩
    GraphicsManager::GetVertexShader(eVertexShaderType::SHADOW)->Bind();
    GraphicsManager::GetPixelShader(ePixelShaderType::PBR)->Reset();
    for (int i = 0; i < mSceneLights.size(); i++)
    {
        // 해당 라이트의 뎁스 뷰, 뷰포트 등을 바인드해준다.
        mSceneLights[i]->GetShadowDSV()->Reset();
        mSceneLights[i]->GetShadowViewport()->Bind();
        // 뎁스 뷰 SetRenderTarget
        D3DGraphicsRenderer::SetRenderTarget(nullptr, mSceneLights[i]->GetShadowDSV());
        // 각 오브젝트에 대한 깊이 버퍼 Draw를 수행한다
        for (auto& drawQueue : mDrawQueue)
        {
            for (auto& drawInfo : drawQueue)
            {
                drawInfo->DrawShadow(mSceneLights[i]);
            }
        }
    }

    // 원래 렌더타겟으로 복구해준다.
    mViewport->Bind();
    D3DGraphicsRenderer::SetRenderTarget(RTV, DSV);
    GraphicsManager::GetVertexShader(eVertexShaderType::STANDARD)->Bind();
    GraphicsManager::GetPixelShader(ePixelShaderType::PBR)->Bind();

    // SRV를 공유하는 텍스쳐가 렌더타겟으로 설정되어있으면 바인딩이 되지 않으므로 여기서 한번에 바인딩 해준다.
    for (int i = 0; i < mSceneLights.size(); i++)
    {
        mSceneLights[i]->GetShadowSRV()->SetBindSlot(24 + i);
        mSceneLights[i]->GetShadowSRV()->Bind();
    }

    // 그리고 실제 Draw 작업을 수행해준다.
    for (auto& drawQueue : mDrawQueue)
    {
        for (auto& drawInfo : drawQueue)
        {
            drawInfo->DrawMesh(this);
        }
        // 그리기 큐를 초기화한다.
        drawQueue.clear();
    }
    // 조명 리스트를 초기화한다.
    mSceneLights.clear();
}

void Camera::EditorRendering()
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    if (ImGui::CollapsingHeader(("Camera" + uid).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Projection");
        ImGui::SliderFloat(("fovAngle" + uid).c_str(), mFovAngle, 1.0f, Degree::MaxDegree);
        ImGui::SliderFloat(("Near" + uid).c_str(), &mProjectionNear, 0.1f, 1000.0f);
        ImGui::SliderFloat(("Far" + uid).c_str(), &mProjectionFar, 1.0f, 100000.0f);
    }
}