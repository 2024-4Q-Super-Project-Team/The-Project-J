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

	// Point2 -> Point1으로 가는 벡터
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

	// 두 플레이어 중 보스와 더 가까운 플레이어 찾기
	Transform* nearPlayer =
		(Vector3::Distance(BossPosition, Player1->GetWorldPosition()) < Vector3::Distance(BossPosition, Player2->GetWorldPosition()))
		? Player1 : Player2;

	// 보스가 가까운 플레이어와 N(mDistanceFromPlayer.val) 거리 유지
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

	// Point1과 Point2의 X값을 보간하여 보스의 X위치 설정
	Vector3 point1Pos = mPoint[0]->transform->GetWorldPosition();
	Vector3 point2Pos = mPoint[1]->transform->GetWorldPosition();

	// 보스가 Point1과 Point2 사이에서 차지하는 비율 계산
	float totalDistance = abs(point1Pos.z - point2Pos.z);
	float bossDistance = abs(point1Pos.z - BossPosition.z);

	float lerpFactor = (totalDistance > 0) ? (bossDistance / totalDistance) : 0.5f; // 거리 0 방지
	float finalX = Lerp(point1Pos.x, point2Pos.x, lerpFactor);

	// 보스의 X 위치 업데이트
	mBossObject->transform->position.x = Lerp(mBossObject->transform->position.x, finalX, Time::GetScaledDeltaTime());
}


#define BOSS_ENTER_TIME  2.0f // 보스 등장 시간
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
	// 플레이어 둘 중 한명이라도 point1 ~ point2 안에 있으면 패턴 실행 가능
	else if (playerPosition.z > mPoint[0]->transform->GetWorldPosition().z && 
		playerPosition.z < mPoint[1]->transform->GetWorldPosition().z)
	{
		mIdleTickCounter += Time::GetScaledDeltaTime();
		// 현재 대기 카운트가 현재 대기 시간보다 커지면
		if (mIdleTickCounter >= mCurIdleTime)
		{
			mIdleTickCounter = 0.0f;
			mCurIdleTime = Random::Range(mMinIdleTick.val, mMaxIdleTick.val);
			// 다음 행동 패턴을 고른다.
			mBossState = eBossStateType::ATTACK;
		}

		UpdatePositionX();
		UpdatePositionZ();
	}
	// 플레이어가 point2를 넘어서면 그로기 상태로 돌입한다.
	else if (playerPosition.z > mPoint[1]->transform->GetWorldPosition().z)
	{
		mBossState = eBossStateType::GROGGY;
		SetPositionGroggy();
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
