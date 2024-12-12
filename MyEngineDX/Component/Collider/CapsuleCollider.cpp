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

void CapsuleCollider::FixedUpdate()
{
}

void CapsuleCollider::PreUpdate()
{
}

void CapsuleCollider::Update()
{
}

void CapsuleCollider::PostUpdate()
{
}

void CapsuleCollider::PreRender()
{
}

void CapsuleCollider::Render(GraphicsManager* _graphicsManager)
{
	__super::Render(_graphicsManager);
}

void CapsuleCollider::PostRender()
{
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
	Vector3 closestPoint = FindClosestPointOnSegment(sphere->mSphere.Center, mCylinder.startPoint, mCylinder.endPoint);
	Vector3 closestVector = closestPoint - sphere->mSphere.Center;
	float distanceSq = closestVector.LengthSquared();

	float combinedRadius = mCylinder.radius + sphere->mSphere.Radius;
	return distanceSq <= combinedRadius * combinedRadius;
}

bool CapsuleCollider::Intersects(CapsuleCollider* capsule)
{
	Vector3 A1 = mCylinder.startPoint;
	Vector3 B1 = mCylinder.endPoint;
	float r1 = mCylinder.radius;

	Vector3 A2 = capsule->mCylinder.startPoint;
	Vector3 B2 = capsule->mCylinder.endPoint;
	float r2 = capsule->mCylinder.radius;

	Vector3 d1 = B1 - A1; // 선분 1의 방향 벡터
	Vector3 d2 = B2 - A2; // 선분 2의 방향 벡터
	Vector3 r = A1 - A2;

	float a = d1.Dot(d1);
	float b = d1.Dot(d2);
	float c = d2.Dot(d2);
	float d = d1.Dot(r);
	float e = d2.Dot(r);

	float denom = a * c - b * b;

	float s = (b * e - c * d) / denom;
	float t = (a * e - b * d) / denom;

	s = std::clamp(s, 0.0f, 1.0f);
	t = std::clamp(t, 0.0f, 1.0f);

	Vector3 closestPoint1 = A1 + s * d1;
	Vector3 closestPoint2 = A2 + t * d2;

	Vector3 diff = closestPoint1 - closestPoint2;
	float distSquared = diff.Dot(diff);
	float radiusSum = r1 + r2;

	return distSquared <= radiusSum * radiusSum;
}

void CapsuleCollider::Draw(GraphicsManager* _graphicsManager)
{
}

Vector3 CapsuleCollider::FindClosestPointOnSegment(Vector3 point, Vector3 start, Vector3 end)
{
	Vector3 segmentVector = end - start;
	float segmentLengthSq = segmentVector.LengthSquared();

	if (segmentLengthSq == 0.f) //선분의 시작점, 끝점 같을 때
		return start;

	Vector3 pointVector = point - start;
	float t = pointVector.Dot(segmentVector) / segmentLengthSq;
	t = max(0.f, min(1.f, t));
	 
	return start + t * segmentVector;

}
