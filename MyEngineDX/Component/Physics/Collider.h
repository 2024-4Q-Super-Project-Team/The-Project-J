#pragma once
#include "Component/Component.h"

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
		virtual void Tick() override = 0;
		virtual void FixedUpdate() override = 0;
		virtual void PreUpdate() override = 0;
		virtual void Update() override = 0;
		virtual void PostUpdate() override = 0;
		virtual void PreRender() override = 0;
		virtual void Render(GraphicsManager* _graphicsManager) override = 0;
		virtual void PostRender() override = 0;
		
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
		DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* mBatch;
#endif
	};
}

