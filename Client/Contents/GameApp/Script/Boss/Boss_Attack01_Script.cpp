#include "pch.h"
#include "Boss_Attack01_Script.h"

#define BOSS_ATTACK_ANIM_RUNNING L"003"
#define BOSS_RAZER_SCALE_Y 500.0f
#define BOSS_LAMP_BONE_NAME L"Bone.005"			// 보스의 램프 본
void Boss_Attack01_Script::Start()
{
	mAttachBone = FindChildObject(
		gameObject->transform->GetParent()->gameObject,
		BOSS_LAMP_BONE_NAME
	);

	mAnimator = gameObject->GetComponent<Animator>();

	mHitColiider = gameObject->GetComponent<BoxCollider>();
	if(mHitColiider == nullptr) mHitColiider = gameObject->AddComponent<BoxCollider>();
	mHitColiider->SetExtents(Vector3(1.0f, BOSS_RAZER_SCALE_Y, 1.0f));

	mRazerBoneAttacher = gameObject->AddComponent<BoneAttacher>();
	if (mRazerBoneAttacher == nullptr) mRazerBoneAttacher = gameObject->AddComponent<BoneAttacher>();
	mRazerBoneAttacher->SetDestBone(mAttachBone->transform);
	//mRazerBoneAttacher->SetOffsetRotation(
	//	Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z)
	//);
}

void Boss_Attack01_Script::Update()
{
	if (isAttack == TRUE)
	{
		// TODO : 도트윈 사용법 알아보자
		//gameObject->transform.Rota
		mRazerElapsedTime += Time::GetScaledDeltaTime();

		if (mRazerElapsedTime > mRazerTime.val)
		{
			SetAttackEnd();
		}
		else
		{
			//float ratio = mRazerElapsedTime / mRazerTime.val;
			//float razerAngle = mRazerRotate.val * ratio;
			//gameObject->transform->SetEulerAngles(Vector3(0.0f, razerAngle,0.0f));
		}
	}
	if (isAttack == FALSE)
	{
		// 시간을 역재생 시켜서 다시 
		if (mRazerElapsedTime > 0.0f)
		{
			if (mRazerElapsedTime <= 0.0f)
			{
				mRazerElapsedTime = 0.0f;
			}
			float ratio = mRazerElapsedTime / mRazerTime.val;
			gameObject->transform->scale.y = ratio;
		}
	}
}

void Boss_Attack01_Script::SetAttackStart()
{
	isAttack = TRUE;
	mAnimator->Play();
	mRazerElapsedTime = 0.0f;
}

void Boss_Attack01_Script::SetAttackEnd()
{
	isAttack = FALSE;
}
