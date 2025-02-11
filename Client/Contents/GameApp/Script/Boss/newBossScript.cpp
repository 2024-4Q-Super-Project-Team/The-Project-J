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
	mRangeObject = FindChildObject(gameObject, L"Boss_Range");
	mBossObject = FindChildObject(gameObject, L"Mon_Boss_01.fbx_Prefab");
	mRazerObject = FindChildObject(gameObject, L"Effect_boss_beam_fix01.fbx_Prefab");

	// Point2 -> Point1���� ���� ����
	mBossDirection = mPoint[1]->transform->position - mPoint[0]->transform->position;
	mBossDirection.Normalize();

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
		case eBossStateType::EXIT:
			UpdateExit();
			break;
		default:
			break;
		}
	}
}

void newBossScript::Reset()
{
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

	// ������ Z ��ġ ������Ʈ (Point1�� Point2 ���̷� ����)
	mBossObject->transform->position.z = Clamp(
		targetPosition.z,
		mPoint[0]->transform->GetWorldPosition().z,
		mPoint[1]->transform->GetWorldPosition().z
	);
}

void newBossScript::UpdatePositionX()
{
	Vector3 BossPosition = mBossObject->transform->GetWorldPosition();

	// Point1�� Point2�� X���� �����Ͽ� ������ X��ġ ����
	float point1X = mPoint[0]->transform->GetWorldPosition().x;
	float point2X = mPoint[1]->transform->GetWorldPosition().x;

	// ������ Point1�� Point2 ���̿��� �����ϴ� ���� ���
	float totalDistance = Vector3::Distance(mPoint[0]->transform->GetWorldPosition(), mPoint[1]->transform->GetWorldPosition());
	float bossDistance = Vector3::Distance(mPoint[0]->transform->GetWorldPosition(), BossPosition);

	float lerpFactor = (totalDistance > 0) ? (bossDistance / totalDistance) : 0.5f; // �Ÿ� 0 ����
	float interpolatedX = point1X + (point2X - point1X) * lerpFactor;

	// ������ X ��ġ ������Ʈ
	mBossObject->transform->position.x = interpolatedX;
}

void newBossScript::UpdateAnimation()
{
	switch (mBossState)
	{
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
	default:
		break;
	}
}

void newBossScript::UpdateNone()
{
	mBossObject->SetActive(false);
	mRazerObject->SetActive(false);
}

void newBossScript::UpdateEnter()
{
	mBossObject->SetActive(true);
	mRazerObject->SetActive(true);
	UpdatePositionZ();
}

void newBossScript::UpdateIdle()
{
	UpdatePositionX();
	UpdatePositionZ();

	mIdleTickCounter += Time::GetScaledDeltaTime();
	// ���� ��� ī��Ʈ�� ���� ��� �ð����� Ŀ����
	if (mIdleTickCounter >= mCurIdleTime)
	{
		mIdleTickCounter = 0.0f;
		mCurIdleTime = Random::Range(mMinIdleTick.val, mMaxIdleTick.val);
		// ���� �ൿ ������ ����.
		mBossState = eBossStateType::ATTACK;
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

void newBossScript::UpdateExit()
{
	
}
