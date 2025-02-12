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

    // 사운드 관련
    mAudioSource = gameObject->AddComponent<AudioSource>();
    ResourceHandle ButtonSoundHandle;
    ButtonSoundHandle.mResourceType = eResourceType::AudioResource;
    ButtonSoundHandle.mMainKey = L"SFX_box";
    ButtonSoundHandle.mPath = L"resource/sound/SFX_box.mp3";
    if (ResourceManager::GetResource<AudioResource>(ButtonSoundHandle) == nullptr)
    {
        ResourceManager::LoadFileFromHandle(ButtonSoundHandle);
    }
    mAudioSource->AddAudio(L"box", ButtonSoundHandle);
}

void BoxScript::Update()
{
   if (mRigid == nullptr) return;
   
   //중력을 주어야 하는지를 판단하기 위해 바닥으로 쏘는 ray 
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

    if (playerPos.y > boxPos.y + 35.f) return;


    // 부모의 회전 행렬 가져오기 (부모의 월드 변환 행렬에서 회전 부분만 추출)
    Matrix parentWorldMatrix = box->gameObject->transform->GetParent()->GetWorldMatrix();
    Matrix parentRotationMatrix = Matrix::CreateFromQuaternion(Quaternion::CreateFromRotationMatrix(parentWorldMatrix));

    localDirection.y = 0.f;
    if (localDirection.LengthSquared() > 0.f)
        localDirection.Normalize();


    XMVECTOR transformedDirection = XMVector3TransformNormal(localDirection, parentRotationMatrix);
    Vector3 direction = Vector3(transformedDirection);

    displacement.val = direction * mMoveSpeed;

    if (mAudioSource->IsPlaying() == false)
        mAudioSource->Play(L"box");
}

void BoxScript::OnCollisionExit(Rigidbody* box, Rigidbody* player)
{
    if (player->gameObject->GetTag() != L"Player") return;

    displacement.val.x = 0.f;
    displacement.val.z = 0.f;

    if (mAudioSource->IsPlaying() == true)
        mAudioSource->Reset();
}

void BoxScript::MoveBox(Vector3 displacement)
{
    gameObject->transform->position += displacement;
}
