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


void Collider::AddShapeToRigidbody()
{
	mRefRigidbody = gameObject->GetComponent<Rigidbody>();
	if (mRefRigidbody)
		mRefRigidbody->AddShape(mShape);

	SetIsTrigger();
	SetPosition();
	SetRotation();
}


void Collider::SetIsTrigger()
{
	if (mIsTrigger == true)
	{
		mShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false); //�浹 ���� ��Ȱ��ȭ 
		mShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true); // Ʈ���� ����
	}
	else
	{
		mShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true); //�浹 ���� Ȱ��ȭ 
		mShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false); // Ʈ���� ����
	}
}



