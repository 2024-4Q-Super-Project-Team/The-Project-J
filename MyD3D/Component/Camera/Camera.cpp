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
    , mProjectionNear(0.01f)
    , mProjectionFar(1000.0f)
    , mOrthoWidth(10.0f)
    , mOrthoHeight(10.0f)
    , mViewport(nullptr)
{
    mType = eComponentType::CAMERA;
    mViewport = new D3DGraphicsViewport(0.0f, 0.0f, _size.x, _size.y);
}

Camera::~Camera()
{
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

json Camera::Serialize()
{
    json ret;
    ret += {"angle", mFovAngle.GetAngle()};
    ret += {"near", mProjectionNear};
    ret += {"far", mProjectionFar};
    ret += {"type", mProjectionType};

    return ret;
}

json Camera::Deserialize()
{
    return json();
}

void Camera::PushDrawList(RendererComponent* _renderComponent)
{
    auto pMaterial = _renderComponent->GetMaterial();
    eBlendingMode blendMode = eBlendingMode::OPAQUE_BLEND;
    if (pMaterial)
    {
        blendMode = pMaterial->mMaterialResource->mBlendMode;
    }

    mDrawQueue[static_cast<UINT>(blendMode)].push(_renderComponent);
}

void Camera::ExcuteDrawList()
{
    for (auto& drawQueue : mDrawQueue)
    {
        while (!drawQueue.empty())
        {
            drawQueue.front()->DrawCall();
            drawQueue.pop();
        }
    }
}

void Camera::EditorRendering()
{
    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Projection");
        ImGui::SliderFloat("fovAngle##1234", mFovAngle, 1.0f, Degree::MaxDegree);
        ImGui::SliderFloat("Near##1234", &mProjectionNear, 0.1f, 10.0f);
        ImGui::SliderFloat("Far##1234", &mProjectionFar, 1.0f, 100.0f);
    }
}
