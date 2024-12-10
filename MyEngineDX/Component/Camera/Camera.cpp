#include "pch.h"
#include "Camera.h"
#include "Object\Object.h"

namespace Component
{
	Camera::Camera(Object* _owner, Vector2 _size) : Component(_owner)
		, m_projectionType(ProjectionType::Perspective)
		, fovAngle(XM_PIDIV2)
		, projectionNear(0.01f)
		, projectionFar(100.0f)
		, orthoHeight(10.0f)
		, orthoWidth(10.0f)
	{
		mType = ComponentType::Camera;
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
	void Camera::FixedUpdate()
	{
	}
	void Camera::PreUpdate()
	{
	}
	void Camera::Update()
	{
	}
	void Camera::LateUpdate()
	{
	}
	void Camera::PreRender()
	{
		Vector3 pos = ownerObject->transform->position;
		Vector3 dir = ownerObject->transform->Forward();
		Vector3 up = Vector3(0, 1, 0);
		Vector3 at = pos + dir;
		viewMatrix = XMMatrixLookAtLH(pos, at, up);

		float aspectRatio = Window::WindowSystem::mainWindow->GetWindowSize().x /
			Window::WindowSystem::mainWindow->GetWindowSize().y;
		if (m_projectionType == ProjectionType::Perspective)
		{
			projectionMatrix = XMMatrixPerspectiveFovLH(
				fovAngle,
				aspectRatio,
				projectionNear,
				projectionFar);
		}
		else if (m_projectionType == ProjectionType::Orthographic)
		{
			projectionMatrix = XMMatrixOrthographicLH(
				orthoWidth,
				orthoHeight / aspectRatio,
				projectionNear,
				projectionFar);
		}
		Graphics::D3DGraphicsDevice::SetViewports(1, &mViewport);

	}
	void Camera::Render(Graphics::IRenderer* _renderer)
	{
	}
	void Camera::PostRender()
	{
	}
}