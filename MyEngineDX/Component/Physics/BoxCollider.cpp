#include "pch.h"
#include "BoxCollider.h"
#include "GameManager/GameManager.h"
#include "Component/Physics/Rigidbody.h"
#include "DebugDraw/DebugDraw.h"

Component::BoxCollider::BoxCollider(GameObject* _owner)
	:Collider(_owner)
{

}

void Component::BoxCollider::Start()
{
	__super::Start();

	PxBoxGeometry box(mExtents.x, mExtents.y, mExtents.z);
	mShape = mPhysics->createShape(box, *mMaterial, true); //shape을 생성합니다. 
	mRigidbody->GetRigidActor()->attachShape(*mShape); //shape을 rigidActor에 연결합니다. 


#ifdef _DEBUG
	Vector3 center = ownerObject->transform->GetWorldPosition();
	mDebugBox = BoundingOrientedBox(center, mExtents, mRotation);
#endif
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
#ifdef _DEBUG

	DX::Draw(mBatch, mDebugBox, Colors::Green);

	//TODO: 충돌시 빨간색 표시
#endif
}

void Component::BoxCollider::PostRender()
{
}

void Component::BoxCollider::SetLocalPosition(Vector3 _pos)
{
	__super::SetLocalPosition(_pos);

#ifdef _DEBUG
	mDebugBox.Center = _pos;
#endif
}

void Component::BoxCollider::SetRotation(Vector3 _rotation)
{
	__super::SetLocalPosition(_rotation);

#ifdef _DEBUG
	mDebugBox.Orientation = Quaternion(_rotation);
#endif
}

