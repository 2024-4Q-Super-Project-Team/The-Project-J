#include "pch.h"
#include "Collider.h"
#include "Component/Collider/Rigidbody.h"

void Collider::Start()
{
	Rigidbody* rigidbody = gameObject->GetComponent<Rigidbody>();
	if (rigidbody)
		rigidbody->AddShape(mShape);
}

void Collider::Tick()
{
}

void Collider::FixedUpdate()
{
}

void Collider::PreUpdate()
{
}

void Collider::Update()
{
}

void Collider::PostUpdate()
{
}

void Collider::PreRender()
{
}

void Collider::Render()
{
}

void Collider::Draw(Camera* _camera)
{
}

void Collider::PostRender()
{
}

void Collider::SetLocalPosition(Vector3 pos)
{
	PxTransform currentTransform = mShape->getLocalPose();
	mShape->setLocalPose(PxTransform(PxVec3(pos.x, pos.y, pos.z), currentTransform.q));
}

void Collider::SetRotation(Vector3 rotation)
{
	PxTransform currentTransform = mShape->getLocalPose();
	Quaternion rotQuat = Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
	PxQuat pxRot;
	memcpy_s(&pxRot, 4, &rotQuat, 4);
	mShape->setLocalPose(PxTransform(currentTransform.p, pxRot));
}

void Collider::SetIsTrigger(bool isTrigger)
{
	if (isTrigger == true)
	{
		mShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false); //충돌 반응 비활성화 
		mShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true); // 트리거 설정
	}
	else
	{
		mShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true); //충돌 반응 활성화 
		mShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false); // 트리거 설정
	}
	mIsTrigger = isTrigger;
}

json Collider::Serialize()
{
	return json();
}

void Collider::Deserialize(json& j)
{
}
