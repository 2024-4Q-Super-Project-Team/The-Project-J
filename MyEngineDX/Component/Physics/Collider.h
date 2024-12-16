#pragma once
#include "Component/Component.h"

namespace Component
{
	class Collider : public ComponentBase
	{
	public:
		Collider(GameObject* _owner);
		virtual ~Collider() {}
	public:
		virtual void Start();
		virtual void FixedUpdate() override;
		virtual void PreUpdate() override;
		virtual void Update() override;
		virtual void PostUpdate() override;
		virtual void PreRender() override;
		virtual void Render(GraphicsManager* _graphicsManager) override;
		virtual void PostRender() override;
		
		//�ݶ��̴��� ���� �������� �����մϴ�. 
		void SetLocalPosition(Vector3 pos);
		//�ݶ��̴��� ȸ������ �����մϴ�. 
		void SetRotation(Vector3 rotation);

	protected:
		PxShape* mShape;
		PxPhysics* mPhysics;

		PxMaterial* mRed;
		PxMaterial* mGreen;

	};
}

