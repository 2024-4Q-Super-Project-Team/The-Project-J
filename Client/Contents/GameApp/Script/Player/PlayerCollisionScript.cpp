#include "pch.h"
#include "PlayerCollisionScript.h"
#include "PlayerScript.h"

void PlayerCollisionScript::Start()
{
    mRigidBody = gameObject->GetComponent<Rigidbody>();

    mBoxCollider = gameObject->GetComponent<BoxCollider>();
    mBoxCollider->SetPosition(Vector3(0.0f, 70.0f, 0.0f));
    mBoxCollider->SetExtents(Vector3(0.3f, 0.7f, 0.3f));
    mBoxCollider->SetIsTrigger();

}

void _CALLBACK PlayerCollisionScript::OnTriggerStay(Collider* _origin, Collider* _destination)
{
    mOwnerPlayer->OnTriggerStayCallback(_origin, _destination);
    return void _CALLBACK();
}
