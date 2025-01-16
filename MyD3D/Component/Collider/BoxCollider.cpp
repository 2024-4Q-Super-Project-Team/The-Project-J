#include "pch.h"
#include "BoxCollider.h"
#include "Physics/PhysicsManager.h"

void BoxCollider::Start()
{
	Collider::Start();

	mShape = GameManager::GetPhysicsManager()->GetPhysics()
		->createShape(PxBoxGeometry(PxVec3(mSize.x, mSize.y, mSize.z)), *GameManager::GetPhysicsManager()->GetDefaultMaterial());
	mShape->userData = this;
	mShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
}

void BoxCollider::Tick()
{
}

void BoxCollider::FixedUpdate()
{
}

void BoxCollider::PreUpdate()
{
}

void BoxCollider::Update()
{
}

void BoxCollider::PostUpdate()
{
}

void BoxCollider::PreRender()
{
}

void BoxCollider::Render()
{
}

void BoxCollider::Draw(Camera* _camera)
{
}

void BoxCollider::PostRender()
{
}
