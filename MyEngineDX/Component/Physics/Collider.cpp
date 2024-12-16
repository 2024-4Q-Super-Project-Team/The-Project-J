#include "pch.h"
#include "Collider.h"
#include "Component/Physics/Rigidbody.h"
#include "Object/Object.h"
#include "GameManager/GameManager.h"
#include "Physics/PhysicsManager.h"

Component::Collider::Collider(GameObject* _owner)
	:ComponentBase(_owner)
{
	mPhysics = GameManager::GetPhysicsManager()->GetPhysics();
}

void Component::Collider::Start()
{
	Rigidbody* rb = ownerObject->GetComponent<Rigidbody>();
	if (rb == nullptr)
	{
		ownerObject->AddComponent<Rigidbody>();
	}

	mRed = mPhysics->createMaterial(1.f, 0.f, 0.f);
	mGreen = mPhysics->createMaterial(0.f, 1.f, 0.f);

}

void Component::Collider::FixedUpdate()
{
	
}

void Component::Collider::PreUpdate()
{
}

void Component::Collider::Update()
{
}

void Component::Collider::PostUpdate()
{
}

void Component::Collider::PreRender()
{
}

void Component::Collider::Render(GraphicsManager* _graphicsManager)
{
}

void Component::Collider::PostRender()
{
}

void Component::Collider::SetLocalPosition(Vector3 pos)
{
	PxTransform currentTransform = mShape->getLocalPose();
	mShape->setLocalPose(PxTransform(PxVec3(pos.x, pos.y, pos.z), currentTransform.q));
}

void Component::Collider::SetRotation(Vector3 rotation)
{
	PxTransform currentTransform = mShape->getLocalPose();
	Quaternion rotQut = Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
	PxQuat pxRot = PxQuat(rotQut.x, rotQut.y, rotQut.z, rotQut.w);
	PxTransform newLocalTransform(currentTransform.p, pxRot);
	mShape->setLocalPose(newLocalTransform);
}

