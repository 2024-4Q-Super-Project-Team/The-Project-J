#include "pch.h"
#include "BoxScript.h"

void BoxScript::Start()
{
    mRigid = gameObject->AddComponent<Rigidbody>();
    mCollider = gameObject->AddComponent<BoxCollider>();
    mRigid->SetMass(15.f);

    mCollider->SetExtents(Vector3(70, 75, 70));
    mCollider->SetPosition(Vector3(0, 40, 0));

    Vector3 rayOriginPos = gameObject->transform->GetWorldPosition() + Vector3(0,-35.f,0);
    mPxRayOrigin = PxVec3(rayOriginPos.x, rayOriginPos.y, rayOriginPos.z);

    mPxRayDirection = PxVec3(0, -1, 0);
}

void BoxScript::Update()
{
   if (mRigid == nullptr) return;
   
   //�߷��� �־�� �ϴ����� �Ǵ��ϱ� ���� �ٴ����� ��� ray 
   Vector3 rayOriginPos = gameObject->transform->GetWorldPosition() + Vector3(0, -1.0f, 0);
   mPxRayOrigin = PxVec3(rayOriginPos.x, rayOriginPos.y, rayOriginPos.z);
   
   GameManager::GetCurrentWorld()->GetPxScene()
       ->raycast(mPxRayOrigin, mPxRayDirection, mMaxDistance, mHitBuffer);
   
   mGravityOn.val = !mHitBuffer.hasBlock;

}

void BoxScript::OnCollisionStay(Rigidbody* box, Rigidbody* player)
{
    if (player->gameObject->GetTag() != L"Player") return; 

    Vector3 playerPos = player->gameObject->transform->GetWorldPosition();
    Vector3 boxPos = box->gameObject->transform->GetWorldPosition();
    Vector3 localDirection = -(boxPos - playerPos);

    // �θ��� ȸ�� ��� �������� (�θ��� ���� ��ȯ ��Ŀ��� ȸ�� �κи� ����)
    Matrix parentWorldMatrix = box->gameObject->transform->GetParent()->GetWorldMatrix();
    Matrix parentRotationMatrix = Matrix::CreateFromQuaternion(Quaternion::CreateFromRotationMatrix(parentWorldMatrix));

    localDirection.y = 0.f;
    if(localDirection.LengthSquared() > 0.f)
        localDirection.Normalize();

    XMVECTOR transformedDirection = XMVector3TransformNormal(localDirection, parentRotationMatrix);
    Vector3 direction = Vector3(transformedDirection);

    displacement.val = direction * mMoveSpeed;

    if (mGravityOn.val)
        displacement.val.y = -mGravitySpeed;
 
    MoveBox(displacement.val * Time::GetScaledDeltaTime());
}

void BoxScript::MoveBox(Vector3 displacement)
{
    gameObject->transform->position += displacement;
}
