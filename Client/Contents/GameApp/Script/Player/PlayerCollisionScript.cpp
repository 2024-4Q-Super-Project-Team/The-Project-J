#include "pch.h"
#include "PlayerCollisionScript.h"
#include "PlayerScript.h"

void PlayerCollisionScript::Start()
{
    mRigidBody = gameObject->GetComponent<Rigidbody>();
    mRigidBody->SetFreezeRotation(true, true, true);
    mRigidBody->SetFreezePosition(true, true, true);
    mRigidBody->SetDisableGravity(true);
    mBoxCollider = gameObject->GetComponent<BoxCollider>();
    mBoxCollider->SetPosition(Vector3(0.0f, 70.0f, 0.0f));
    mBoxCollider->SetExtents(Vector3(30.0f, 70.0f, 30.0f));
    mBoxCollider->SetIsTrigger(true);
}

void PlayerCollisionScript::Update()
{
    if (mOwnerPlayer)
    {
        gameObject->transform->position = mOwnerPlayer->gameObject->transform->position;
    }
}

void _CALLBACK PlayerCollisionScript::OnTriggerStay(Collider* _origin, Collider* _destination)
{
    mOwnerPlayer->OnTriggerStayCallback(_origin, _destination);
    return void _CALLBACK();
}
