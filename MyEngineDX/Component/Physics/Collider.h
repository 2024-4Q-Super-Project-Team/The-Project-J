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
		
		//콜라이더의 로컬 포지션을 변경합니다. 
		virtual void SetLocalPosition(Vector3 _pos);
		//콜라이더의 회전값을 변경합니다. 
		virtual void SetRotation(Vector3 _rotation);
		//true이면 충돌 반응 없이 Trigger만 발동합니다.
		void SetIsTrigger(bool _isTrigger);
		bool GetIsTrigger() { return mIsTrigger; }

	private:
		bool mIsTrigger = false;

	protected:
		PxShape* mShape;
		PxPhysics* mPhysics;
		PxMaterial* mMaterial; //(정지마찰, 동적마찰, 반발)

		Rigidbody* mRigidbody;


		//Debug Draw
#ifdef _DEBUG
	protected:
		bool mDebugDraw = false;
		Vector3 mPosition;
		Vector4 mRotation;
		std::unique_ptr<PrimitiveBatch<VertexPositionColor>> mPrimitiveBatch; //도형 그리기용
#endif
	};
}

