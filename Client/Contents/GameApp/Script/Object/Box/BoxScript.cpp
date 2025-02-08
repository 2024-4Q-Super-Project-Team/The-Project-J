#include "pch.h"
#include "BoxScript.h"


void BoxScript::Start()
{
    mRigid = gameObject->AddComponent<Rigidbody>();
    mCollider = gameObject->AddComponent<BoxCollider>();
    mRigid->SetMass(15.f);

    mCollider->SetExtents(Vector3(70, 75, 70));
    mCollider->SetPosition(Vector3(0, 35, 0));

    mPxRayDirection = PxVec3(0, -1, 0);

    // �θ��� ȸ�� ��� �������� (�θ��� ���� ��ȯ ��Ŀ��� ȸ�� �κи� ����)
    Matrix parentWorldMatrix = gameObject->transform->GetParent()->GetWorldMatrix();
    Matrix parentRotationMatrix = Matrix::CreateFromQuaternion(Quaternion::CreateFromRotationMatrix(parentWorldMatrix));

    if (mPxRayDirection.magnitudeSquared() > 0.f)
        mPxRayDirection.normalize();

    Vector3 dir = Vector3(mPxRayDirection.x, mPxRayDirection.y, mPxRayDirection.z);

    XMVECTOR transformedDirection = XMVector3TransformNormal(dir, parentRotationMatrix);
    Vector3 direction = Vector3(transformedDirection);

    mPxRayDirection = PxVec3(direction.x, direction.y, direction.z);
}

void BoxScript::Update()
{
   if (mRigid == nullptr) return;
   
   //�߷��� �־�� �ϴ����� �Ǵ��ϱ� ���� �ٴ����� ��� ray 
   Vector3 rayOriginPos = gameObject->transform->GetWorldPosition() + Vector3(0, -0.5f, 0);
   mPxRayOrigin = PxVec3(rayOriginPos.x, rayOriginPos.y, rayOriginPos.z);

   PxQueryFilterData filterData(PxQueryFlag::eSTATIC);
   GameManager::GetCurrentWorld()->GetPxScene()
       ->raycast(mPxRayOrigin, mPxRayDirection, 1.0f, mHitBuffer);
   mGravityOn.val = !mHitBuffer.hasBlock;

   if (mGravityOn.val)
       displacement.val.y = -mGravitySpeed;


   MoveBox(displacement.val * Time::GetScaledDeltaTime());
}

void BoxScript::OnCollisionEnter(Rigidbody* box, Rigidbody* player)
{
    if (player->gameObject->GetTag() != L"Player") return;

    Vector3 playerPos = player->gameObject->transform->GetWorldPosition();
    Vector3 boxPos = box->gameObject->transform->GetWorldPosition();
    Vector3 localDirection = -(boxPos - playerPos);

    // �θ��� ȸ�� ��� �������� (�θ��� ���� ��ȯ ��Ŀ��� ȸ�� �κи� ����)
    Matrix parentWorldMatrix = box->gameObject->transform->GetParent()->GetWorldMatrix();
    Matrix parentRotationMatrix = Matrix::CreateFromQuaternion(Quaternion::CreateFromRotationMatrix(parentWorldMatrix));

    localDirection.y = 0.f;
    if (localDirection.LengthSquared() > 0.f)
        localDirection.Normalize();


    XMVECTOR transformedDirection = XMVector3TransformNormal(localDirection, parentRotationMatrix);
    Vector3 direction = Vector3(transformedDirection);

    displacement.val = direction * mMoveSpeed;

}

void BoxScript::OnCollisionExit(Rigidbody* box, Rigidbody* player)
{
    if (player->gameObject->GetTag() != L"Player") return;

    displacement.val.x = 0.f;
    displacement.val.z = 0.f;
}

void BoxScript::MoveBox(Vector3 displacement)
{
    gameObject->transform->position += displacement;
}
