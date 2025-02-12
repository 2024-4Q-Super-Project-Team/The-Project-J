#include "pch.h"
#include "newBossAttackScript.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"

void newBossAttackScript::Start()
{
	mAnimator = gameObject->GetComponent<Animator>();
	mAudioSource = gameObject->GetComponent<AudioSource>();

	mRigidBody = gameObject->GetComponent<Rigidbody>();
	if (mRigidBody == nullptr) mRigidBody = gameObject->AddComponent<Rigidbody>();

	mHitColiider = gameObject->GetComponent<BoxCollider>();
	if (mHitColiider == nullptr) mHitColiider = gameObject->AddComponent<BoxCollider>();
	mHitColiider->SetExtents(mColliderExternt);
	mHitColiider->SetIsTrigger(true);

	gameObject->transform->scale = mOriginScale;
	gameObject->SetActive(false);
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
	mAudioSource->Reset();
	mAnimator->Play();
	mAudioSource->Play();
	mRazerElapsedTime = 0.0f;
	gameObject->SetActive(true);
	mSpawnPosition = _pos;
	mSpawnPosition.y -= mOriginScale.y * 0.5f;
}

void newBossAttackScript::SetAttackEnd()
{
	isAttack = FALSE;
	mAudioSource->Reset();
	gameObject->SetActive(false);
}

json newBossAttackScript::Serialize()
{
	json ret = MonoBehaviour::Serialize();

	ret["razer damage"] = mRazerDamage.val;
	ret["razer distance"] = mRazerDist.val;
	ret["razer range"] = mRazerRange.val;
	ret["razer scale"] = mRazerScale.val;
	ret["razer time"] = mRazerTime.val;

	return ret;
}

void newBossAttackScript::Deserialize(json& j)
{
	MonoBehaviour::Deserialize(j);
	if (j.contains("razer damage"))
	{
		mRazerDamage.val = j["razer damage"].get<INT>();
	}
	if (j.contains("razer distance"))
	{
		mRazerDist.val = j["razer distance"].get<FLOAT>();
	}
	if (j.contains("razer range"))
	{
		mRazerRange.val = j["razer range"].get<FLOAT>();
	}
	if (j.contains("razer scale"))
	{
		mRazerScale.val = j["razer scale"].get<FLOAT>();
	}
	if (j.contains("razer time"))
	{
		mRazerTime.val = j["razer time"].get<FLOAT>();
	}
}
