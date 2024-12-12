#include "pch.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "Object/Object.h"

BoxCollider::BoxCollider(GameObject* _pOwner) : Collider(_pOwner, COLLIDER_TYPE_BOX)
{
	Vector3 objPos = _pOwner->transform->GetWorldPosition();
	mBox.Center = objPos;
	mBox.Extents.x = mBox.Extents.y = mBox.Extents.z = 3.f;
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

void BoxCollider::Render(GraphicsManager* _graphicsManager)
{
	__super::Render(_graphicsManager);
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

void BoxCollider::Draw(GraphicsManager* _graphicsManager)
{
	
}

void BoxCollider::GetAxis(Vector3& axisX, Vector3& axisY, Vector3& axisZ)
{
	Vector3 localX = Vector3(1.f, 0.f, 0.f);
	Vector3 localY = Vector3(0.f, 1.f, 0.f);
	Vector3 localZ = Vector3(0.f, 0.f, 1.f);

	Quaternion orientation = mBox.Orientation;

	axisX = Vector3::Transform(localX, orientation);
	axisY = Vector3::Transform(localY, orientation);
	axisZ = Vector3::Transform(localZ, orientation);
}
