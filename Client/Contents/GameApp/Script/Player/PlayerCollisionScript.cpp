#include "pch.h"
#include "PlayerCollisionScript.h"
#include "PlayerScript.h"

void PlayerCollisionScript::Start()
{
    gameObject->SetTag(L"Player_Collision");
    mRigidBody = gameObject->GetComponent<Rigidbody>();
    if(mRigidBody == nullptr)
        mRigidBody = gameObject->AddComponent<Rigidbody>();

    mCollider = gameObject->GetComponent<SphereCollider>();
    if (mCollider == nullptr)
        mCollider = gameObject->AddComponent<SphereCollider>();
    mCollider->SetRadius(100);
    mCollider->SetIsTrigger(true);
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
