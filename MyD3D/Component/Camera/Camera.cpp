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
        // ī�޶� ������� ���ε�
        GraphicsManager::GetConstantBuffer(eCBufferType::Camera)->UpdateGPUResoure(&mCameraCBuffer);
        // ����Ʈ ���ε�
        mViewport->Bind();
        // ������ ������Ʈ�� �׸���.
        world->Draw(this);
    }

    // ���� ť�� ó���Ѵ�.
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

    // ���� ���� ����?
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
    // 0. ���� ����Ÿ���� �����س��´�.
    auto RTV = D3DGraphicsRenderer::GetCurrentRTV();
    auto DSV = D3DGraphicsRenderer::GetCurrentDSV();
    // 1. �׸��ڿ� ���̴� ���ε�
    GraphicsManager::GetVertexShader(eVertexShaderType::SHADOW)->Bind();
    GraphicsManager::GetPixelShader(ePixelShaderType::PBR)->Reset();
    for (int i = 0; i < mSceneLights.size(); i++)
    {
        // �ش� ����Ʈ�� ���� ��, ����Ʈ ���� ���ε����ش�.
        mSceneLights[i]->GetShadowDSV()->Reset();
        mSceneLights[i]->GetShadowViewport()->Bind();
        // ���� �� SetRenderTarget
        D3DGraphicsRenderer::SetRenderTarget(nullptr, mSceneLights[i]->GetShadowDSV());
        // �� ������Ʈ�� ���� ���� ���� Draw�� �����Ѵ�
        for (auto& drawQueue : mDrawQueue)
        {
            for (auto& drawInfo : drawQueue)
            {
                drawInfo->DrawShadow(mSceneLights[i]);
            }
        }
    }

    // ���� ����Ÿ������ �������ش�.
    mViewport->Bind();
    D3DGraphicsRenderer::SetRenderTarget(RTV, DSV);
    GraphicsManager::GetVertexShader(eVertexShaderType::STANDARD)->Bind();
    GraphicsManager::GetPixelShader(ePixelShaderType::PBR)->Bind();

    // SRV�� �����ϴ� �ؽ��İ� ����Ÿ������ �����Ǿ������� ���ε��� ���� �����Ƿ� ���⼭ �ѹ��� ���ε� ���ش�.
    for (int i = 0; i < mSceneLights.size(); i++)
    {
        mSceneLights[i]->GetShadowSRV()->SetBindSlot(24 + i);
        mSceneLights[i]->GetShadowSRV()->Bind();
    }

    // �׸��� ���� Draw �۾��� �������ش�.
    for (auto& drawQueue : mDrawQueue)
    {
        for (auto& drawInfo : drawQueue)
        {
            drawInfo->DrawMesh(this);
        }
        // �׸��� ť�� �ʱ�ȭ�Ѵ�.
        drawQueue.clear();
    }
    // ���� ����Ʈ�� �ʱ�ȭ�Ѵ�.
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