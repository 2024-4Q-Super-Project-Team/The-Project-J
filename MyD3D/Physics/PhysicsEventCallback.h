#pragma once

class PhysicsEventCallback : public PxSimulationEventCallback
{
	//This is called when a breakable constraint breaks.
	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
	{
	}
	//This is called during PxScene::fetchResults with the actors which have just been woken up.						
	void onWake(PxActor** actors, PxU32 count)
	{
	}

	////This is called during PxScene::fetchResults with the actors which have just been put to sleep.						
	void onSleep(PxActor** actors, PxU32 count)
	{
	}

	//This is called during PxScene::fetchResults with the current trigger pair events.
	void onTrigger(PxTriggerPair* pairs, PxU32 nbPairs)
	{
		//loop through all trigger-pairs of PhysX simulation
		for (PxU32 i = 0; i < nbPairs; i++)
		{
			const PxTriggerPair& curTriggerPair = pairs[i];

			PxShape* triggerShape = curTriggerPair.triggerShape;
			PxShape* otherShape = curTriggerPair.otherShape;

			Collider* triggerCollider = static_cast<Collider*>(triggerShape->userData);
			Collider* otherCollider = static_cast<Collider*>(otherShape->userData);

			if (!triggerCollider || !otherCollider) return;

			Object* triggerObject = static_cast<Object*>(triggerCollider->gameObject);
			Object* otherObject = static_cast<Object*>(otherCollider->gameObject);

			auto triggerScripts = triggerObject->GetComponents<MonoBehaviour>();
			auto otherScripts = otherObject->GetComponents<MonoBehaviour>();

			const PxPairFlags& cp = pairs[0].status;
				
			// 처음으로 충돌했을 때 
			if (cp & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				for (auto script : triggerScripts)
					script->OnTriggerEnter(triggerCollider, otherCollider);
				for (auto script : otherScripts)
					script->OnTriggerEnter(otherCollider, triggerCollider);
			}
			// 충돌이 끝나는 때
			else if (cp & PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				for (auto script : triggerScripts)
					script->OnTriggerExit(triggerCollider, otherCollider);
				for (auto script : otherScripts)
					script->OnTriggerExit(otherCollider, triggerCollider);
			} //충돌중~
			else
			{
				for (auto script : triggerScripts)
					script->OnTriggerStay(triggerCollider, otherCollider);
				for (auto script : otherScripts)
					script->OnTriggerStay(otherCollider, triggerCollider);
			}
		}
	}

	//The method will be called for a pair of actors if one of the colliding shape pairs requested contact notification.
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
	{
		PxActor* triggerActor = pairHeader.actors[0];
		PxActor* otherActor = pairHeader.actors[1];

		Rigidbody* triggerRigidbody= static_cast<Rigidbody*>(triggerActor->userData);
		Rigidbody* otherRigidbody = static_cast<Rigidbody*>(otherActor->userData);

		Object* triggerObject = static_cast<Object*>(triggerRigidbody->gameObject);
		Object* otherObject = static_cast<Object*>(otherRigidbody->gameObject);

		auto triggerScripts = triggerObject->GetComponents<MonoBehaviour>();
		auto otherScripts = otherObject->GetComponents<MonoBehaviour>();

		const PxContactPairFlags& cp = pairs[0].flags;

		// 처음으로 충돌했을 때 
		if (cp & PxContactPairFlag::eACTOR_PAIR_HAS_FIRST_TOUCH)
		{
			for (auto script : triggerScripts)
				script->OnCollisionEnter(triggerRigidbody, otherRigidbody);
			for (auto script : otherScripts)
				script->OnCollisionEnter(otherRigidbody, triggerRigidbody);
		}
		// 충돌이 끝나는 때
		else if (cp & PxContactPairFlag::eACTOR_PAIR_LOST_TOUCH)
		{
			for (auto script : triggerScripts)
				script->OnCollisionExit(triggerRigidbody, otherRigidbody);
			for (auto script : otherScripts)
				script->OnCollisionExit(otherRigidbody, triggerRigidbody);
		} //충돌중~
		else
		{
			for (auto script : triggerScripts)
				script->OnCollisionStay(triggerRigidbody, otherRigidbody);
			for (auto script : otherScripts)
				script->OnCollisionStay(otherRigidbody, triggerRigidbody);
		}
	}

	virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
	{

	}
};

