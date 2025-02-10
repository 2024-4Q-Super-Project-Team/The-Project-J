#include "pch.h"
#include "Boss_Attack01_Script.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"

#define BOSS_ATTACK_ANIM_RUNNING L"003"
#define BOSS_RAZER_SCALE_Y 500.0f
#define BOSS_LAMP_BONE_NAME L"Bone.005"			// ������ ���� ��
void Boss_Attack01_Script::Start()
{
	mAttachBone = FindChildObject(
		gameObject->transform->GetParent()->gameObject,
		BOSS_LAMP_BONE_NAME
	);

	mAnimator = gameObject->GetComponent<Animator>();

	mRigidBody = gameObject->GetComponent<Rigidbody>();
	if (mRigidBody == nullptr) mRigidBody = gameObject->AddComponent<Rigidbody>();

	mHitColiider = gameObject->GetComponent<BoxCollider>();
	if(mHitColiider == nullptr) mHitColiider = gameObject->AddComponent<BoxCollider>();
	mHitColiider->SetExtents(Vector3(1.0f, BOSS_RAZER_SCALE_Y, 1.0f));
	mHitColiider->SetIsTrigger(true);

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
		// TODO : ��Ʈ�� ���� �˾ƺ���
		//gameObject->transform.Rota
		mRazerElapsedTime += Time::GetScaledDeltaTime();

		if (mRazerElapsedTime > mRazerTime.val)
		{
			SetAttackEnd();
		}
		else
		{
			FLOAT ratio = mRazerElapsedTime / mRazerTime.val;
			FLOAT razerAngle = Lerp(-mRazerRotate.val, mRazerRotate.val, ratio);
			Quaternion quat = Quaternion::CreateFromYawPitchRoll(
				XMConvertToRadians(0),
				XMConvertToRadians(razerAngle),
				XMConvertToRadians(0)
			);
			mRazerBoneAttacher->SetOffsetRotation(quat);
			//gameObject->transform->SetEulerAngles(Vector3(0.0f, razerAngle,0.0f));
		}
	}
	if (isAttack == FALSE)
	{
		// �ð��� ����� ���Ѽ� �ٽ� 
		if (mRazerElapsedTime > 0.0f)
		{
			mRazerElapsedTime -= Time::GetScaledDeltaTime() * mRazerTime.val;
			if (mRazerElapsedTime <= 0.0f)
			{
				mRazerElapsedTime = 0.0f;
				gameObject->SetActive(false);
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