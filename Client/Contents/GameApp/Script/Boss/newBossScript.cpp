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

	// Point2 -> Point1으로 가는 벡터
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

	// 두 플레이어 중 보스와 더 가까운 플레이어 찾기
	Transform* nearPlayer =
		(Vector3::Distance(BossPosition, Player1->GetWorldPosition()) < Vector3::Distance(BossPosition, Player2->GetWorldPosition()))
		? Player1 : Player2;

	// 보스가 가까운 플레이어와 N(mDistanceFromPlayer.val) 거리 유지
	Vector3 targetPosition = nearPlayer->GetWorldPosition() + (mBossDirection * mDistanceFromPlayer.val);

	// 보스의 Z 위치 업데이트 (Point1과 Point2 사이로 제한)
	mBossObject->transform->position.z = Clamp(
		targetPosition.z,
		mPoint[0]->transform->GetWorldPosition().z,
		mPoint[1]->transform->GetWorldPosition().z
	);
}

void newBossScript::UpdatePositionX()
{
	Vector3 BossPosition = mBossObject->transform->GetWorldPosition();

	// Point1과 Point2의 X값을 보간하여 보스의 X위치 설정
	float point1X = mPoint[0]->transform->GetWorldPosition().x;
	float point2X = mPoint[1]->transform->GetWorldPosition().x;

	// 보스가 Point1과 Point2 사이에서 차지하는 비율 계산
	float totalDistance = Vector3::Distance(mPoint[0]->transform->GetWorldPosition(), mPoint[1]->transform->GetWorldPosition());
	float bossDistance = Vector3::Distance(mPoint[0]->transform->GetWorldPosition(), BossPosition);

	float lerpFactor = (totalDistance > 0) ? (bossDistance / totalDistance) : 0.5f; // 거리 0 방지
	float interpolatedX = point1X + (point2X - point1X) * lerpFactor;

	// 보스의 X 위치 업데이트
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
	// 현재 대기 카운트가 현재 대기 시간보다 커지면
	if (mIdleTickCounter >= mCurIdleTime)
	{
		mIdleTickCounter = 0.0f;
		mCurIdleTime = Random::Range(mMinIdleTick.val, mMaxIdleTick.val);
		// 다음 행동 패턴을 고른다.
		mBossState = eBossStateType::ATTACK;
	}
}

#define BOSS_ATTACK_TRIGGER_FRAME 28	// 광선이 나가기 시작하는 프레임
void newBossScript::UpdateAttack()
{
	UpdatePositionX();
	UpdatePositionZ();

	if (mBodyAnimator->GetActiveAnimationKey() == BOSS_ANIM_ATTACK_01)
	{
		// 램프에서 검은 광선이 나와 왼쪽 아래 바닥부터 오른쪽 아래 바닥까지 이어지는 광역 딜을 날린다.
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
