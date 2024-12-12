#include "pch.h"
#include "CapsuleCollider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "Object/Object.h"

CapsuleCollider::CapsuleCollider(GameObject* _pOwner)
	: Collider(_pOwner, COLLIDER_TYPE_CAPSULE)
{
	//초기값 설정
	Vector3 objPos = _pOwner->transform->GetWorldPosition();
	mCylinder.endPoint = objPos + Vector3(0.f, 0.f, 10.f);
	mCylinder.startPoint = objPos + Vector3(0.f, 0.f, -10.f);
	mCylinder.radius = 2;

	mSpheres[0].Center = mCylinder.startPoint;
	mSpheres[1].Center = mCylinder.endPoint;
	mSpheres[0].Radius = mSpheres[1].Radius = mCylinder.radius;
}

bool CapsuleCollider::Intersects(BoxCollider* box)
{
	// 
	//두 구와 박스의 충돌 
	if (mSpheres[0].Intersects(box->mBox)) return true;
	if (mSpheres[1].Intersects(box->mBox)) return true;

	//실린더와 박스의 충돌 

	//실린더 축과 높이 계산 
	Vector3 cylinderAxis = mCylinder.endPoint - mCylinder.startPoint;
	float cylinderHeight = cylinderAxis.Length();
	cylinderAxis.Normalize();

	//박스 중심 -> 실린더 중심 축 벡터 
	Vector3 cylinderCenter = (mCylinder.startPoint + mCylinder.endPoint) / 2;
	Vector3 delta = cylinderCenter - box->ownerObject->transform->GetWorldPosition();

	//박스 축에 투영한다. 
	Vector3 boxAxisX, boxAxisY, boxAxisZ;
	box->GetAxis(boxAxisX, boxAxisY, boxAxisZ);

	float distX = fabs(delta.Dot(boxAxisX));
	float distY = fabs(delta.Dot(boxAxisY));
	float distZ = fabs(delta.Dot(boxAxisZ));

	//실린더 반경과 높이 투영 
	float radiusProjectionX = mCylinder.radius + fabs(cylinderAxis.Dot(boxAxisX)) * (cylinderHeight * 0.5f);
	float radiusProjectionY = mCylinder.radius + fabs(cylinderAxis.Dot(boxAxisY)) * (cylinderHeight * 0.5f);
	float radiusProjectionZ = mCylinder.radius + fabs(cylinderAxis.Dot(boxAxisZ)) * (cylinderHeight * 0.5f);

	return (distX <= box->mBox.Extents.x + radiusProjectionX) &&
		(distY <= box->mBox.Extents.y + radiusProjectionY) &&
		(distZ <= box->mBox.Extents.z + radiusProjectionZ);
}

bool CapsuleCollider::Intersects(SphereCollider* sphere)
{
	//TODO : Sphere-Capsule

	
	DirectX::BoundingSphere sphere1;
	


	return false;
}

bool CapsuleCollider::Intersects(CapsuleCollider* capsule)
{
	//TODO : Capsule-Capsule
	return false;
}
