#include "pch.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "CapsuleCollider.h"

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
