#include "pch.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"

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

void BoxCollider::Render(GraphicsManager* _graphicsManager)
{
}

void BoxCollider::PostRender()
{
}

bool BoxCollider::Intersects(BoxCollider* box)
{
	return mBox.Intersects(box->mBox);
}

bool BoxCollider::Intersects(SphereCollider* sphere)
{
	return mBox.Intersects(sphere->mSphere);
}

bool BoxCollider::Intersects(CapsuleCollider* capsule)
{
	return capsule->Intersects(this);
}
