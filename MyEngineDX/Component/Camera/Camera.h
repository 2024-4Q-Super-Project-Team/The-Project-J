#pragma once
#include "Component/Component.h"

namespace Component
{

	class Camera
		: public ComponentBase
	{
		enum class ProjectionType;
	public:
		Camera(Object* _owner, Vector2 _size = Vector2::Zero);
		virtual ~Camera();

		virtual void FixedUpdate() override;
		virtual void PreUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void PreRender() override;
		virtual void Render(Graphics::IRenderer* _renderer) override;
		virtual void PostRender() override;
		

		void SetProjectionType(ProjectionType type) { m_projectionType = type; }
		void SetFovAngle(Degree _angle) { fovAngle = _angle; }
		void SetProjectionNear(float _near) { projectionNear = _near; }
		void SetProjectionFar(float _far) { projectionFar = _far; }
		void SetProjectionFar(float _near, float _far) { projectionNear = _near; projectionFar = _far; }

		Degree& GetFovAngle() { return fovAngle; }
		float& GetProjectionNear() { return projectionNear; }
		float& GetProjectionFar() { return projectionFar; }
		Matrix& GetViewMatrix() { return viewMatrix; }
		Matrix& GetProjectionMatrix() { return projectionMatrix; }

		Vector2 GetViewportSize() { return Vector2(mViewport.Width, mViewport.Height); }
		void SetViewportSize(Vector2 _size) { mViewport.Width = _size.x; mViewport.Height = _size.y; }
	private:
		D3D11_VIEWPORT mViewport;
		ProjectionType m_projectionType;

		Matrix viewMatrix;
		Matrix projectionMatrix;

		Degree  fovAngle;
		float   projectionNear;
		float   projectionFar;

		float   orthoWidth;
		float   orthoHeight;
	public:
		// 투영 타입
		enum class ProjectionType
		{
			Perspective,
			Orthographic
		};
	};
}