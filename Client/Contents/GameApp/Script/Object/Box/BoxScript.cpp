#include "pch.h"
#include "BoxScript.h"

void BoxScript::Start()
{
    mRigid = gameObject->AddComponent<Rigidbody>();
    mRigid->SetIsDynamic(true);
    mRigid = gameObject->GetComponent<Rigidbody>();
    mCollider = gameObject->AddComponent<BoxCollider>();
    

    mCollider->SetExtents(Vector3(150, 150, 150));
    mCollider->SetPosition(Vector3(0, 35, 0));

    Vector3 rayOriginPos = gameObject->transform->GetWorldPosition() + Vector3(0,-35.1f,0);
    mPxRayOrigin = PxVec3(rayOriginPos.x, rayOriginPos.y, rayOriginPos.z);

    mPxRayDirection = PxVec3(0, -1, 0);

    mRigid->SetFreezeRotation(true, true, true);
}

void BoxScript::Update()
{
    if (mRigid == nullptr) return;

    Vector3 rayOriginPos = gameObject->transform->GetWorldPosition() + Vector3(0, -35.1f, 0);
    mPxRayOrigin = PxVec3(rayOriginPos.x, rayOriginPos.y, rayOriginPos.z);

    GameManager::GetCurrentWorld()->GetPxScene()
        ->raycast(mPxRayOrigin, mPxRayDirection, mMaxDistance, mHitBuffer);
  
    if (mHitBuffer.hasBlock)
    {
        mRigid->SetDisableGravity(true);
    }
    else
    {
        mRigid->SetDisableGravity(false);
    }

}
