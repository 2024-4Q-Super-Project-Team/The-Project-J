#include "pch.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "CapsuleCollider.h"
#include "Object/Object.h"

SphereCollider::SphereCollider(GameObject* _pOwner) : Collider(_pOwner, COLLIDER_TYPE_SPHERE)
{
	Vector3 objPos = _pOwner->transform->GetWorldPosition();
	mSphere.Center = objPos;
	mSphere.Radius = 3.f;
}

bool SphereCollider::Intersects(BoxCollider* box)
{
	return mSphere.Intersects(box->mBox);
}

bool SphereCollider::Intersects(SphereCollider* sphere)
{
	return mSphere.Intersects(sphere->mSphere);
}

bool SphereCollider::Intersects(CapsuleCollider* capsule)
{
	return capsule->Intersects(this);
}

void SphereCollider::Draw(GraphicsManager* _graphicsManager)
{
}

