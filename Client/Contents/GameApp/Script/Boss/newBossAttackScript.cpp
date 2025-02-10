#include "pch.h"
#include "newBossAttackScript.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"

void newBossAttackScript::Start()
{
	mAnimator = gameObject->GetComponent<Animator>();

	mRigidBody = gameObject->GetComponent<Rigidbody>();
	if (mRigidBody == nullptr) mRigidBody = gameObject->AddComponent<Rigidbody>();

	mHitColiider = gameObject->GetComponent<BoxCollider>();
	if (mHitColiider == nullptr) mHitColiider = gameObject->AddComponent<BoxCollider>();
	mHitColiider->SetExtents(mColliderExternt);
	mHitColiider->SetIsTrigger(true);

	gameObject->transform->scale = mOriginScale;
}

void newBossAttackScript::Update()
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
			
			gameObject->transform->position = mSpawnPosition;
			gameObject->transform->position.x += Lerp(mRazerRange.val, -mRazerRange.val, ratio);
		}
	}
	if (isAttack == FALSE)
	{
		//// 시간을 역재생 시켜서 다시 
		//if (mRazerElapsedTime > 0.0f)
		//{
		//	mRazerElapsedTime -= Time::GetScaledDeltaTime() * mRazerTime.val;
		//	if (mRazerElapsedTime <= 0.0f)
		//	{
		//		mRazerElapsedTime = 0.0f;
		//	}
		//	float ratio = mRazerElapsedTime / mRazerTime.val;
		//	gameObject->transform->scale.y = ratio;
		//}
	}
}

void _CALLBACK newBossAttackScript::OnTriggerStay(Collider* _origin, Collider* _destination)
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

void newBossAttackScript::SetAttackStart(Vector3 _pos)
{
	isAttack = TRUE;
	mAnimator->Play();
	mRazerElapsedTime = 0.0f;
	gameObject->SetActive(true);
	mSpawnPosition = _pos;
	mSpawnPosition.y -= mOriginScale.y * 0.5f;
}

void newBossAttackScript::SetAttackEnd()
{
	isAttack = FALSE;
	gameObject->SetActive(false);
}
