#pragma once
#include "Component/Component.h"

using namespace DirectX;

namespace Component
{
	class Rigidbody;

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
		virtual void SetLocalPosition(Vector3 _pos);
		//�ݶ��̴��� ȸ������ �����մϴ�. 
		virtual void SetRotation(Vector3 _rotation);
		//true�̸� �浹 ���� ���� Trigger�� �ߵ��մϴ�.
		void SetIsTrigger(bool _isTrigger);
		bool GetIsTrigger() { return mIsTrigger; }

	private:
		bool mIsTrigger = false;

	protected:
		PxShape* mShape;
		PxPhysics* mPhysics;
		PxMaterial* mMaterial; //(��������, ��������, �ݹ�)

		Rigidbody* mRigidbody;


		//Debug Draw
#ifdef _DEBUG
	protected:
		bool mDebugDraw = false;
		Vector3 mPosition;
		Vector4 mRotation;
		std::unique_ptr<PrimitiveBatch<VertexPositionColor>> mPrimitiveBatch; //���� �׸����
#endif
	};
}

