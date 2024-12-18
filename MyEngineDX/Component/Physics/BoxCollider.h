#pragma once
#include "Collider.h"
using namespace DirectX;

namespace Component
{
	class BoxCollider : public Collider
	{
	public:
		BoxCollider(GameObject* _owner);
		virtual ~BoxCollider() {}
	public:
		virtual void Start() override;
		virtual void Tick() override;
		virtual void FixedUpdate() override;
		virtual void PreUpdate() override;
		virtual void Update() override;
		virtual void PostUpdate() override;
		virtual void PreRender() override;
		virtual void Render(GraphicsManager* _graphicsManager) override;
		virtual void PostRender() override;

		//콜라이더의 로컬 포지션을 변경합니다. 
		virtual void SetLocalPosition(Vector3 _pos) override;
		//콜라이더의 회전값을 변경합니다. 
		virtual void SetRotation(Vector3 _rotation) override;
		//박스의 크기를 변경합니다. 
		void SetExtents(Vector3 _extents) { mExtents = _extents; }

	private:
		Vector3 mExtents = { 1.f, 1.f, 1.f };

#ifdef _DEBUG

	private:
		BoundingOrientedBox mDebugBox;
#endif
	};
}

