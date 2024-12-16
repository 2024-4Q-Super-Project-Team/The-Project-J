#include "pch.h"
#include "BoxCollider.h"
#include "GameManager/GameManager.h"
#include "Physics/PhysicsManager.h"


Component::BoxCollider::BoxCollider(GameObject* _owner)
	:Collider(_owner)
{

}

void Component::BoxCollider::Start()
{
	__super::Start();

	PxBoxGeometry box(1.f, 1.f, 1.f);
	mShape = mPhysics->createShape(box, *mGreen, true);

}

void Component::BoxCollider::FixedUpdate()
{
}

void Component::BoxCollider::PreUpdate()
{
}

void Component::BoxCollider::Update()
{
}

void Component::BoxCollider::PostUpdate()
{
}

void Component::BoxCollider::PreRender()
{
}

void Component::BoxCollider::Render(GraphicsManager* _graphicsManager)
{
}

void Component::BoxCollider::PostRender()
{
}

