#include "pch.h"
#include "newBossScript.h"
#include "newBossRangeScript.h"
#include "newBossAttackScript.h"
#include "Contents/GameApp/Script/GameProgressManager.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"
#define BOSS_ANIM_IDLE		L"001"		
#define BOSS_ANIM_ATTACK_01 L"003"		

void newBossScript::Start()
{
	mPoint[0] = FindChildObject(gameObject, L"Point1");
	mPoint[1] = FindChildObject(gameObject, L"Point2");
	mPoint[2] = FindChildObject(gameObject, L"Point3");
	mRangeObject = FindChildObject(gameObject, L"Boss_Range");
	mBossObject = FindChildObject(gameObject, L"Mon_Boss_01.fbx_Prefab");
	mRazerObject = FindChildObject(gameObject, L"Effect_boss_beam_fix01.fbx_Prefab");

	// Point2 -> Point1���� ���� ����
	mBossDirection = mPoint[1]->transform->position - mPoint[0]->transform->position;
	mBossDirection.Normalize();
	mBossOriginPosition = mBossObject->transform->position;

	if (mRangeObject)
	{
		mRangeScript = mRangeObject->AddComponent<newBossRangeScript>();
	}
	if (mBossObject)
	{
		mBodyAnimator = mBossObject->GetComponent<Animator>();
	}
	if (mRazerObject)
	{
		mAttackScript = mRazerObject->GetComponent<newBossAttackScript>();
	}
}

void newBossScript::Update()
{
	UpdateAnimation();

	if (mRangeScript->IsTriggerOn())
	{
		switch (mBossState)
		{
		case eBossStateType::NONE:
			UpdateNone();
			break;
		case eBossStateType::ENTER:
			UpdateEnter();
			break;
		case eBossStateType::IDLE:
			UpdateIdle();
			break;
		case eBossStateType::ATTACK:
			UpdateAttack();
			break;
		case eBossStateType::GROGGY:
			UpdateGroggy();
			break;
		case eBossStateType::EXIT:
			UpdateExit();
			break;
		default:
			break;
		}
		if (mBossState == eBossStateType::NONE)
		{
			mBossState = eBossStateType::ENTER;
		}

	}
	else
	{
		if (mBossState != eBossStateType::GROGGY && mBossState != eBossStateType::EXIT)
		{
			mBossObject->transform->position = mPoint[0]->transform->position;
			mBossObject->transform->position.y += 1000;
			mBossObject->SetActive(false);
			mAttackScript->SetAttackEnd();
			mBossState = eBossStateType::NONE;
		}
	}
}

void newBossScript::Reset()
{
	mBossState = eBossStateType::NONE;
}

void newBossScript::UpdatePositionZ()
{
	Transform* Player1 = GameProgressManager::GetPlayerInfo(0)->gameObject->transform;
	Transform* Player2 = GameProgressManager::GetPlayerInfo(1)->gameObject->transform;

	Vector3 BossPosition = mBossObject->transform->GetWorldPosition();

	// �� �÷��̾� �� ������ �� ����� �÷��̾� ã��
	Transform* nearPlayer =
		(Vector3::Distance(BossPosition, Player1->GetWorldPosition()) < Vector3::Distance(BossPosition, Player2->GetWorldPosition()))
		? Player1 : Player2;

	// ������ ����� �÷��̾�� N(mDistanceFromPlayer.val) �Ÿ� ����
	Vector3 targetPosition = nearPlayer->GetWorldPosition() + (mBossDirection * mDistanceFromPlayer.val);
	float finalZ = Clamp(
		targetPosition.z,
		mPoint[0]->transform->GetWorldPosition().z,
		mPoint[2]->transform->GetWorldPosition().z
	);

	mBossObject->transform->position.z = Lerp(mBossObject->transform->position.z, finalZ, Time::GetScaledDeltaTime());
}

void newBossScript::UpdatePositionX()
{
	Vector3 BossPosition = mBossObject->transform->GetWorldPosition();

	// Point1�� Point2�� X���� �����Ͽ� ������ X��ġ ����
	Vector3 point1Pos = mPoint[0]->transform->GetWorldPosition();
	Vector3 point2Pos = mPoint[1]->transform->GetWorldPosition();

	// ������ Point1�� Point2 ���̿��� �����ϴ� ���� ���
	float totalDistance = abs(point1Pos.z - point2Pos.z);
	float bossDistance = abs(point1Pos.z - BossPosition.z);

	float lerpFactor = (totalDistance > 0) ? (bossDistance / totalDistance) : 0.5f; // �Ÿ� 0 ����
	float finalX = Lerp(point1Pos.x, point2Pos.x, lerpFactor);

	// ������ X ��ġ ������Ʈ
	mBossObject->transform->position.x = Lerp(mBossObject->transform->position.x, finalX, Time::GetScaledDeltaTime());
}


#define BOSS_ENTER_TIME  2.0f // ���� ���� �ð�
void newBossScript::SetPositionEnter()
{
	Vector3 FinalPosition;
	FinalPosition.x = mPoint[0]->transform->position.x;
	FinalPosition.y = mBossOriginPosition.y;
	FinalPosition.z = mPoint[0]->transform->position.z + 500;

	mBossObject->transform->MoveTo(FinalPosition, BOSS_ENTER_TIME, Dotween::EasingEffect::OutQuint);
}

void newBossScript::SetPositionGroggy()
{
	Vector3 FinalPosition;
	FinalPosition.x = mPoint[2]->transform->position.x;
	FinalPosition.y = mBossOriginPosition.y;
	FinalPosition.z = mPoint[2]->transform->position.z;

	mBossObject->transform->MoveTo(FinalPosition, BOSS_ENTER_TIME, Dotween::EasingEffect::OutQuint);
}

#define BOSS_EXIT_TIME 5.0f
void newBossScript::SetPositionExit()
{
	if (mBossObject->transform->scale != Vector3::Zero)
	{
		mBossObject->transform->ScaleTo(Vector3::Zero, BOSS_EXIT_TIME, Dotween::EasingEffect::InOutBounce);
		mBossObject->transform->position.y += 30.0f * Time::GetScaledDeltaTime();
	}
}

void newBossScript::UpdateAnimation()
{
	switch (mBossState)
	{
	case eBossStateType::ENTER:
	{
		mBodyAnimator->SetLoop(true);
		mBodyAnimator->SetCurrentAnimation(BOSS_ANIM_IDLE, 0.5f);
		break;
	}
	case eBossStateType::IDLE:
	{
		mBodyAnimator->SetLoop(true);
		mBodyAnimator->SetCurrentAnimation(BOSS_ANIM_IDLE, 0.5f);
		break;
	}
	case eBossStateType::ATTACK:
	{
		mBodyAnimator->SetLoop(false);
		mBodyAnimator->SetCurrentAnimation(BOSS_ANIM_ATTACK_01, 0.5f);
		break;
	}
	case eBossStateType::GROGGY:
	{
		mBodyAnimator->SetLoop(true);
		mBodyAnimator->SetCurrentAnimation(BOSS_ANIM_IDLE, 0.5f);
		break;
	}
	case eBossStateType::EXIT:
	{
		mBodyAnimator->SetLoop(true);
		mBodyAnimator->SetCurrentAnimation(BOSS_ANIM_IDLE, 0.5f);
		break;
	}
	default:
		break;
	}
}

void newBossScript::UpdateNone()
{
	SetPositionEnter();
}

void newBossScript::UpdateEnter()
{
	mBossObject->SetActive(true);

	static FLOAT elapsedTime = 0.0f;
	elapsedTime += Time::GetScaledDeltaTime();
	FLOAT ratio = elapsedTime / BOSS_ENTER_TIME;

	if (elapsedTime >= BOSS_ENTER_TIME)
	{
		elapsedTime = 0.0f;
		mBossState = eBossStateType::IDLE;
	}
}

void newBossScript::UpdateIdle()
{
	Transform* Player1 = GameProgressManager::GetPlayerInfo(0)->gameObject->transform;
	Transform* Player2 = GameProgressManager::GetPlayerInfo(1)->gameObject->transform;

	Vector3 BossPosition = mBossObject->transform->GetWorldPosition();

	Transform* nearPlayer =
		(Vector3::Distance(BossPosition, Player1->GetWorldPosition()) < Vector3::Distance(BossPosition, Player2->GetWorldPosition()))
		? Player1 : Player2;

	Vector3 playerPosition = nearPlayer->GetWorldPosition();

	if (playerPosition.z < mPoint[0]->transform->GetWorldPosition().z)
	{
		UpdatePositionX();
		UpdatePositionZ();
	}
	// �÷��̾� �� �� �Ѹ��̶� point1 ~ point2 �ȿ� ������ ���� ���� ����
	else if (playerPosition.z > mPoint[0]->transform->GetWorldPosition().z && 
		playerPosition.z < mPoint[1]->transform->GetWorldPosition().z)
	{
		mIdleTickCounter += Time::GetScaledDeltaTime();
		// ���� ��� ī��Ʈ�� ���� ��� �ð����� Ŀ����
		if (mIdleTickCounter >= mCurIdleTime)
		{
			mIdleTickCounter = 0.0f;
			mCurIdleTime = Random::Range(mMinIdleTick.val, mMaxIdleTick.val);
			// ���� �ൿ ������ ����.
			mBossState = eBossStateType::ATTACK;
		}

		UpdatePositionX();
		UpdatePositionZ();
	}
	// �÷��̾ point2�� �Ѿ�� �׷α� ���·� �����Ѵ�.
	else if (playerPosition.z > mPoint[1]->transform->GetWorldPosition().z)
	{
		mBossState = eBossStateType::GROGGY;
		SetPositionGroggy();
	}
}

#define BOSS_ATTACK_TRIGGER_FRAME 28	// ������ ������ �����ϴ� ������
void newBossScript::UpdateAttack()
{
	UpdatePositionX();
	UpdatePositionZ();

	if (mBodyAnimator->GetActiveAnimationKey() == BOSS_ANIM_ATTACK_01)
	{
		// �������� ���� ������ ���� ���� �Ʒ� �ٴں��� ������ �Ʒ� �ٴڱ��� �̾����� ���� ���� ������.
		if (mBodyAnimator->GetDuration() >= BOSS_ATTACK_TRIGGER_FRAME)
		{
			if (isAttack == false)
			{
				mAttackScript->SetAttackStart(mBossObject->transform->position);
				isAttack = true;
			}
		}
		if (mBodyAnimator->IsEnd())
		{
			mBossState = eBossStateType::IDLE;
			isAttack = false;
		}
	}

}

void newBossScript::UpdateGroggy()
{
}

void newBossScript::UpdateExit()
{
	//static FLOAT elapsedTime = 0.0f;
	//elapsedTime += Time::GetScaledDeltaTime();
	//FLOAT ratio = elapsedTime / BOSS_EXIT_TIME;
	//
	//
	//if (elapsedTime >= BOSS_EXIT_TIME)
	//{
	//	elapsedTime = 0.0f;
	//	mBossState = eBossStateType::IDLE;
	//}
}

void newBossScript::SetExit()
{
	if (mBossState == eBossStateType::GROGGY)
	{
		SetPositionExit();
		mBossState = eBossStateType::EXIT;
	}
}

json newBossScript::Serialize()
{
	return json();
}

void newBossScript::Deserialize(json& j)
{
}
