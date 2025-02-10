#include "pch.h"
#include "Boss_Attack01_Script.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"

#define BOSS_ATTACK_ANIM_RUNNING L"003"
#define BOSS_RAZER_SCALE_Y 500.0f
#define BOSS_LAMP_BONE_NAME L"Bone.005"			// 보스의 램프 본
void Boss_Attack01_Script::Start()
{
	//mAttachBone = FindChildObject(
	//	gameObject->transform->GetParent()->gameObject,
	//	BOSS_LAMP_BONE_NAME
	//);

	mAnimator = gameObject->GetComponent<Animator>();

	mRigidBody = gameObject->GetComponent<Rigidbody>();
	if (mRigidBody == nullptr) mRigidBody = gameObject->AddComponent<Rigidbody>();

	mHitColiider = gameObject->GetComponent<BoxCollider>();
	if(mHitColiider == nullptr) mHitColiider = gameObject->AddComponent<BoxCollider>();
	mHitColiider->SetExtents(Vector3(1.0f, BOSS_RAZER_SCALE_Y, 1.0f));
	mHitColiider->SetIsTrigger(true);

	//mRazerBoneAttacher = gameObject->AddComponent<BoneAttacher>();
	//if (mRazerBoneAttacher == nullptr) mRazerBoneAttacher = gameObject->AddComponent<BoneAttacher>();
	//mRazerBoneAttacher->SetDestBone(mAttachBone->transform);
	//mRazerBoneAttacher->SetOffsetRotation(
	//	Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z)
	//);
}

void Boss_Attack01_Script::Update()
{
	if (isAttack == TRUE)
	{
		mRazerElapsedTime += Time::GetScaledDeltaTime();

		if (mRazerElapsedTime > mRazerTime.val)
		{
			SetAttackEnd();
		}
		else
		{
			FLOAT ratio = mRazerElapsedTime / mRazerTime.val;
			FLOAT razerAngle = Lerp(-mRazerRotate.val, mRazerRotate.val, ratio);

			// 기존 방향 벡터에 추가적인 회전 적용
			Vector3 rightDir = mRootViewDir.Cross(Vector3(0, 1, 0));
			Vector3 adjustedDir = mRootViewDir * cos(razerAngle) + rightDir * sin(razerAngle);
			adjustedDir.Normalize(); // 정규화하여 크기 유지

			gameObject->transform->position = (adjustedDir * mRazerDist.val);
		}
	}
	if (isAttack == FALSE)
	{
		// 시간을 역재생 시켜서 다시 
		if (mRazerElapsedTime > 0.0f)
		{
			mRazerElapsedTime -= Time::GetScaledDeltaTime() * mRazerTime.val;
			if (mRazerElapsedTime <= 0.0f)
			{
				mRazerElapsedTime = 0.0f;
			}
			float ratio = mRazerElapsedTime / mRazerTime.val;
			//gameObject->transform->scale.y = ratio;
		}
	}
}

void _CALLBACK Boss_Attack01_Script::OnTriggerStay(Collider* _origin, Collider* _destination)
{
	if (isAttack == TRUE && _destination->gameObject->GetTag() == L"Player")
	{
		PlayerScript* player = _destination->gameObject->GetComponent<PlayerScript>();
		if (player)
		{
			player->Hit(mRazerDamage.val);
		}
	}
	return void _CALLBACK();
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

void Boss_Attack01_Script::SetRootObject(Object* _root)
{
	mRootObject = _root;
}

void Boss_Attack01_Script::SetRootView(Vector3 _viewDir)
{
	mRootViewDir = _viewDir;
}

void Boss_Attack01_Script::SetRootAngleY(FLOAT _yAngle)
{
	mRootAngleY = _yAngle;
}
