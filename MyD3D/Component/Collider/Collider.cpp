#include "pch.h"
#include "Collider.h"
#include "Component/Collider/Rigidbody.h"

bool Collider::bDrawMode = false;

Collider::Collider(Object* _owner) : Component(_owner)
{
	mType = eComponentType::COLLDIER;

	mIsTrigger = false;
}

Collider::~Collider()
{
	if (mShape)
	{
		mShape->release();
	}
}

void Collider::Start()
{
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

Vector3 Collider::GetDistanceFromCamera(Camera* _camera)
{
	return _camera->GetDistance(gameObject->transform);
}

void Collider::SetLocalPosition()
{
	PxTransform currentTransform = mShape->getLocalPose();
	mShape->setLocalPose(PxTransform(PxVec3(mPosition.x, mPosition.y, mPosition.z), currentTransform.q));
}

void Collider::SetRotation()
{
	PxTransform currentTransform = mShape->getLocalPose();
	mQuatRotation = Quaternion::CreateFromYawPitchRoll(mRotation.y, mRotation.x, mRotation.z);
	PxQuat pxRot;
	memcpy_s(&pxRot, sizeof(float) * 4, &mQuatRotation, sizeof(float) * 4);
	mShape->setLocalPose(PxTransform(currentTransform.p, pxRot));
}

void Collider::AddShapeToRigidbody()
{
	Rigidbody* rigidbody = gameObject->GetComponent<Rigidbody>();
	if (rigidbody)
		rigidbody->AddShape(mShape);

	SetIsTrigger();
	SetLocalPosition();
	SetRotation();
}


void Collider::SetIsTrigger()
{
	if (mIsTrigger == true)
	{
		mShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false); //충돌 반응 비활성화 
		mShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true); // 트리거 설정
	}
	else
	{
		mShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true); //충돌 반응 활성화 
		mShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false); // 트리거 설정
	}
}



