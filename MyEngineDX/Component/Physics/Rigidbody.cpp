#include "pch.h"
#include "Rigidbody.h"
#include "GameManager/GameManager.h"
#include "Physics/PhysicsManager.h"

Component::Rigidbody::Rigidbody(GameObject* _owner, bool _isDynamic) 
	:ComponentBase(_owner)
{
	PxPhysics* physics = GameManager::GetPhysicsManager()->GetPhysics();
	Vector3 pos = ownerObject->transform->GetWorldPosition();

	PxTransform transform = PxTransform(PxVec3(pos.x, pos.y, pos.z));

	if (_isDynamic)
	{
		mRigidActor = physics->createRigidDynamic(transform);
	}
	else
	{
		mRigidActor = physics->createRigidStatic(transform);
	}
}
