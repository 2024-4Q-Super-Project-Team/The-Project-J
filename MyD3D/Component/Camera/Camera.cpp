#include "pch.h"
#include "Camera.h"
#include "Manager/GameManager.h"
#include "ViewportScene/ViewportManager.h"
#include "Graphics/GraphicsManager.h"
#include "World/WorldManager.h"
#include "ViewportScene/ViewportScene.h"
#include "World/World.h"
#include "ObjectGroup/ObjectGroup.h"
#include "Object/Object.h"

Camera::Camera(Object* _owner, Vector2 _size)
    : Component(_owner)
    , mProjectionType(ProjectionType::Perspective)
    , mFovAngle(XM_PIDIV2)
    , mProjectionNear(0.01f)
    , mProjectionFar(1000.0f)
    , mOrthoWidth(10.0f)
    , mOrthoHeight(10.0f)
{
    mType = eComponentType::CAMERA;
    mViewport.TopLeftX = 0;
    mViewport.TopLeftY = 0;
    mViewport.Width = _size.x;
    mViewport.Height = _size.y;
    mViewport.MinDepth = 0.0f;
    mViewport.MaxDepth = 1.0f;
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
    GraphicsManager::BindViewport(&mViewport);
    auto* world = GameManager::GetCurrentWorld();
    if (world)
    {
        CameraCBuffer ca;
        ca.Position.x = gameObject->transform->position.x;
        ca.Position.y = gameObject->transform->position.y;
        ca.Position.z = gameObject->transform->position.z;
        ca.Position.w = 1.0f;
        GraphicsManager::UpdateConstantBuffer(eCBufferType::Camera, &ca);
        world->Draw(this);
    }
    
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
    ViewportManager* vptMng =  GameManager::GetViewportManager();
    float aspectRatio = mViewport.Width / mViewport.Height;

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
