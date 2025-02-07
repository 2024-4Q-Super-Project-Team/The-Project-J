#include "pch.h"
#include "PlayerCollisionScript.h"
#include "PlayerScript.h"

void PlayerCollisionScript::Start()
{
    gameObject->SetTag(L"Player_Collision");
    mRigidBody = gameObject->GetComponent<Rigidbody>();
    if(mRigidBody == nullptr)
        mRigidBody = gameObject->AddComponent<Rigidbody>();
    mRigidBody->SetFreezeRotation(true, true, true);
    mRigidBody->SetFreezePosition(true, true, true);

    mBoxCollider = gameObject->GetComponent<BoxCollider>();
    if (mBoxCollider == nullptr)
        mBoxCollider = gameObject->AddComponent<BoxCollider>();
    mBoxCollider->SetPosition(Vector3(0.0f, 70.0f, 0.0f));
    mBoxCollider->SetExtents(Vector3(60.0f, 50.0f, 60.0f));
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

void _CALLBACK PlayerCollisionScript::OnTriggerExit(Collider* _origin, Collider* _destination)
{
    mOwnerPlayer->OnTriggerExit(_origin, _destination);
    return void _CALLBACK();
}
