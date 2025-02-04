#pragma once
#include "physx/characterkinematic/PxController.h"

class ControllerEventCallback : public PxUserControllerHitReport
{
public:
	std::unordered_map<PxController*, std::unordered_set<PxActor*>> mCollisions;

	void EraseCollideActor(PxController* _controller, PxActor* _actor)
	{
		mCollisions[_controller].erase(_actor);
	}
private:

	virtual void onShapeHit(const PxControllerShapeHit& hit)
	{
		PxActor* triggerActor = hit.controller->getActor();
		PxActor* otherActor = hit.actor;

		Rigidbody* triggerRigidbody = static_cast<Rigidbody*>(triggerActor->userData);
		Rigidbody* otherRigidbody = static_cast<Rigidbody*>(otherActor->userData);

		Object* triggerObject = static_cast<Object*>(triggerRigidbody->gameObject);
		Object* otherObject = static_cast<Object*>(otherRigidbody->gameObject);

		auto triggerScripts = triggerObject->GetComponents<MonoBehaviour>();
		auto otherScripts = otherObject->GetComponents<MonoBehaviour>();


		if (mCollisions.find(hit.controller) == mCollisions.end())
			mCollisions[hit.controller] = {};

		PlayerController* controller = static_cast<PlayerController*>(hit.controller->getUserData());
		controller->mActorsColliding[hit.actor] = true;

		// 처음으로 충돌했을 때 
		if (mCollisions[hit.controller].find(hit.actor) == mCollisions[hit.controller].end())
		{
			mCollisions[hit.controller].insert(hit.actor);

			for (auto script : triggerScripts)
				script->OnCollisionEnter(triggerRigidbody, otherRigidbody);
			for (auto script : otherScripts)
				script->OnCollisionEnter(otherRigidbody, triggerRigidbody);
		}
		else //충돌중 
		{
			for (auto script : triggerScripts)
				script->OnCollisionStay(triggerRigidbody, otherRigidbody);
			for (auto script : otherScripts)
				script->OnCollisionStay(otherRigidbody, triggerRigidbody);
		}
	}
	virtual void onControllerHit(const PxControllersHit& hit) 
	{

	}
	virtual void onObstacleHit(const PxControllerObstacleHit& hit) 
	{

	}

};

//collider controller에서 erase 할 떄 callback의 map에서도 erase 해줘야 함. 
