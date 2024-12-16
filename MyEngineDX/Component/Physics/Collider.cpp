#include "pch.h"
#include "Collider.h"
#include "Component/Physics/Rigidbody.h"
#include "Object/Object.h"
#include "GameManager/GameManager.h"
#include "Physics/PhysicsManager.h"

Component::Collider::Collider(GameObject* _owner)
	:ComponentBase(_owner)
{
	mPhysics = GameManager::GetPhysicsManager()->GetPhysics();

	Rigidbody* rb = ownerObject->GetComponent<Rigidbody>();
	if (rb == nullptr)
	{
		ownerObject->AddComponent<Rigidbody>();
	}

	mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);

#ifdef _DEBUG
	mPrimitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(/*DeviceContext*/);
#endif
}

void Component::Collider::Start()
{

}

void Component::Collider::FixedUpdate()
{
	
}

void Component::Collider::PreUpdate()
{
}

void Component::Collider::Update()
{
}

void Component::Collider::PostUpdate()
{
}

void Component::Collider::PreRender()
{
}

void Component::Collider::Render(GraphicsManager* _graphicsManager)
{
}

void Component::Collider::PostRender()
{
}

void Component::Collider::SetLocalPosition(Vector3 _pos)
{
	PxTransform currentTransform = mShape->getLocalPose();
	mShape->setLocalPose(PxTransform(PxVec3(_pos.x, _pos.y, _pos.z), currentTransform.q));

#ifdef _DEBUG
	mPosition = { _pos.x, _pos.y, _pos.z };
#endif
}

void Component::Collider::SetRotation(Vector3 _rotation)
{
	PxTransform currentTransform = mShape->getLocalPose();
	Quaternion rotQut = Quaternion::CreateFromYawPitchRoll(_rotation.y, _rotation.x, _rotation.z);
	PxQuat pxRot = PxQuat(rotQut.x, rotQut.y, rotQut.z, rotQut.w);
	PxTransform newLocalTransform(currentTransform.p, pxRot);
	mShape->setLocalPose(newLocalTransform);

#ifdef _DEBUG
	mRotation = { pxRot.x, pxRot.y, pxRot.z, pxRot.w };
#endif
}

void Component::Collider::SetIsTrigger(bool _isTrigger)
{
	if (_isTrigger == true)
	{
		mShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false); //충돌 반응 비활성화 
		mShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true); // 트리거 설정
	}
	else
	{
		mShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true); //충돌 반응 활성화 
		mShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false); // 트리거 설정
	}
	mIsTrigger = _isTrigger;
}

