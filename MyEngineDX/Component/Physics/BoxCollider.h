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

		//�ݶ��̴��� ���� �������� �����մϴ�. 
		virtual void SetLocalPosition(Vector3 _pos) override;
		//�ݶ��̴��� ȸ������ �����մϴ�. 
		virtual void SetRotation(Vector3 _rotation) override;
		//�ڽ��� ũ�⸦ �����մϴ�. 
		void SetExtents(Vector3 _extents) { mExtents = _extents; }

	private:
		Vector3 mExtents = { 1.f, 1.f, 1.f };

#ifdef _DEBUG

	private:
		BoundingOrientedBox mDebugBox;
#endif
	};
}

