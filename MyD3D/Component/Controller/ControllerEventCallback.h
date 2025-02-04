#pragma once
#include "physx/characterkinematic/PxController.h"

class ControllerEventCallback : public PxUserControllerHitReport
{
public:
	std::unordered_map<PxController*, std::unordered_set<PxActor*>> mCollisions;

	void EraseCollideActor(PxController* _controller, PxActor* _actor)
	{
		auto& actors = mCollisions[_controller];
		actors.erase(_actor);
	}
private:

	virtual void onShapeHit(const PxControllerShapeHit& hit)
	{
		PxActor* hitActor = hit.controller->getActor();
		PxActor* otherActor = hit.actor;

		Rigidbody* hitRigidbody = static_cast<Rigidbody*>(hitActor->userData);
		Rigidbody* otherRigidbody = static_cast<Rigidbody*>(otherActor->userData);

		Object* hitObject = static_cast<Object*>(hitRigidbody->gameObject);
		Object* otherObject = static_cast<Object*>(otherRigidbody->gameObject);

		auto hitScripts = hitObject->GetComponents<MonoBehaviour>();
		auto otherScripts = otherObject->GetComponents<MonoBehaviour>();


		if (mCollisions.find(hit.controller) == mCollisions.end())
			mCollisions[hit.controller] = {};

		PlayerController* controller = static_cast<PlayerController*>(hit.controller->getUserData());
		controller->mActorsColliding[hit.actor] = true;

		// 처음으로 충돌했을 때 
		if (mCollisions[hit.controller].find(hit.actor) == mCollisions[hit.controller].end())
		{
			mCollisions[hit.controller].insert(hit.actor);

			for (auto script : hitScripts)
				script->OnCollisionEnter(hitRigidbody, otherRigidbody);
			for (auto script : otherScripts)
				script->OnCollisionEnter(otherRigidbody, hitRigidbody);
		}
		else //충돌중 
		{
			for (auto script : hitScripts)
				script->OnCollisionStay(hitRigidbody, otherRigidbody);
			for (auto script : otherScripts)
				script->OnCollisionStay(otherRigidbody, hitRigidbody);
		}
	}
	virtual void onControllerHit(const PxControllersHit& hit) 
	{

	}
	virtual void onObstacleHit(const PxControllerObstacleHit& hit) 
	{

	}

};
