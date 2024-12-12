#include "pch.h"
#include "CapsuleCollider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

bool CapsuleCollider::Intersects(BoxCollider* box)
{
	//TODO : Box-Capsule
	return false;
}

bool CapsuleCollider::Intersects(SphereCollider* sphere)
{
	//TODO : Sphere-Capsule
	return false;
}

bool CapsuleCollider::Intersects(CapsuleCollider* capsule)
{
	//TODO : Capsule-Capsule
	return false;
}
