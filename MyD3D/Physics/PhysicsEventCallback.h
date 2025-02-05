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

				triggerCollider->AddTriggerOther(otherCollider);
				otherCollider->AddTriggerOther(triggerCollider);
			}
			// 충돌이 끝나는 때
			else if (cp & PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				for (auto script : triggerScripts)
					script->OnTriggerExit(triggerCollider, otherCollider);
				for (auto script : otherScripts)
					script->OnTriggerExit(otherCollider, triggerCollider);

				triggerCollider->RemoveTriggerOther(otherCollider);
				otherCollider->RemoveTriggerOther(triggerCollider);
			}
		}
	}

	//The method will be called for a pair of actors if one of the colliding shape pairs requested contact notification.
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
	{
		PxActor* contactActor = pairHeader.actors[0];
		PxActor* otherActor = pairHeader.actors[1];

		Rigidbody* contactRigidbody= static_cast<Rigidbody*>(contactActor->userData);
		Rigidbody* otherRigidbody = static_cast<Rigidbody*>(otherActor->userData);

		Object* contactObject = static_cast<Object*>(contactRigidbody->gameObject);
		Object* otherObject = static_cast<Object*>(otherRigidbody->gameObject);

		auto contactScripts = contactObject->GetComponents<MonoBehaviour>();
		auto otherScripts = otherObject->GetComponents<MonoBehaviour>();

		const PxContactPairFlags& cp = pairs[0].flags;

		// 처음으로 충돌했을 때 
		if (cp & PxContactPairFlag::eACTOR_PAIR_HAS_FIRST_TOUCH)
		{
			for (auto script : contactScripts)
				script->OnCollisionEnter(contactRigidbody, otherRigidbody);
			for (auto script : otherScripts)
				script->OnCollisionEnter(otherRigidbody, contactRigidbody);

			contactRigidbody->AddContactOther(otherRigidbody);
			otherRigidbody->AddContactOther(contactRigidbody);
		}
		// 충돌이 끝나는 때
		else if (cp & PxContactPairFlag::eACTOR_PAIR_LOST_TOUCH)
		{
			for (auto script : contactScripts)
				script->OnCollisionExit(contactRigidbody, otherRigidbody);
			for (auto script : otherScripts)
				script->OnCollisionExit(otherRigidbody, contactRigidbody);

			contactRigidbody->RemoveContactOther(otherRigidbody);
			otherRigidbody->RemoveContactOther(contactRigidbody);
		} 
	}

	virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
	{

	}
};

