#include "pch.h"
#include "BossScript.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"

#include "Manager/PlayerManager.h"

#define BOSS_ANIM_IDLE L"001"
#define BOSS_ANIM_ATTACK_01 L"003"
#define BOSS_ANIM_ATTACK_02 L"004" // 005나 004중 하나임
#define BOSS_ANIM_HIT		// 피격 애니메이션 X

void BossScript::Start()
{
}

void BossScript::Update()
{
	UpdateTransform();
    UpdateAnimation();
	UpdateState();
}

void BossScript::UpdateTransform()
{
	// 고민사항 : 월드내에 보스가 속하면 월드포지션에 대한 연산이 꼬일 것 같음. 맵 밖으로 뺄까?

	// 두 플레이어의 사이 값을 먼저 구한다.
	PlayerScript* Player1 = PlayerManager::GetPlayerInfo(0);
	PlayerScript* Player2 = PlayerManager::GetPlayerInfo(1);

	Vector3 Player1WorldPos = Player1->gameObject->transform->GetWorldPosition();
	Vector3 Player2WorldPos = Player2->gameObject->transform->GetWorldPosition();

	// 플레이어 간 중간 위치 벡터 계산
	Vector3 BetweenPos = (Player1WorldPos + Player2WorldPos) * 0.5f;
	Vector3 AxisPos = mAxisTransform->GetWorldPosition();

	// 중심 축에서 보스가 바라볼 방향 벡터
	Vector3 ViewDirection = BetweenPos - AxisPos;
	ViewDirection.Normalize();

	// 보스의 위치 계산 (= 중심 축 + 보는 방향 * 중심 축으로부터의 거리)
	gameObject->transform->position = AxisPos + (mDistanceFromAxis.val * ViewDirection);

	// 보스의 회전 계산
	float targetAngleY = atan2(ViewDirection.x, ViewDirection.y);
	if (targetAngleY < 0.0f)
		targetAngleY += XM_2PI;  // 360도 대신 2π 사용
	Vector3 CurrAngle = gameObject->transform->GetEulerAngles();
	float currAngleY = CurrAngle.y;
	float delta = fmod(targetAngleY - currAngleY + XM_PI, XM_2PI) - XM_PI;
	float newAngleY = currAngleY + delta * 0.7f;
	gameObject->transform->SetEulerAngles(Vector3(0.0f, newAngleY, 0.0f));
}

void BossScript::UpdateAnimation()
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
		mBodyAnimator->SetCurrentAnimation(BOSS_ANIM_IDLE, 0.5f);
		break;
	}
	case eBossStateType::HIT:
	{
		mBodyAnimator->SetLoop(false);
		mBodyAnimator->SetCurrentAnimation(BOSS_ANIM_IDLE, 0.5f);
		break;
	}
    default:
        break;
    }
}

void BossScript::UpdateState()
{
	switch (mBossState)
	{
	case eBossStateType::IDLE:
		UpdateIdle();
		break;
	case eBossStateType::ATTACK:
		UpdateAttack();
		break;
	case eBossStateType::HIT:
		UpdateHit();
		break;
	default:
		break;
	}
}

void BossScript::UpdateIdle()
{
	mIdleTickCounter += Time::GetScaledDeltaTime();
	// 현재 대기 카운트가 현재 대기 시간보다 커지면
	if (mIdleTickCounter >= mCurIdleTime)
	{
		mIdleTickCounter = 0.0f;
		mCurIdleTime = Random::Range(mMinIdleTick.val, mMaxIdleTick.val);
		// 다음 행동 패턴을 고른다.
		mCurrAttackType = (eBossAttackType)Random::Range<INT>(ATTACK_01, ATTACK_02);
		SetState(eBossStateType::ATTACK);
	}
}

void BossScript::UpdateAttack()
{
	switch (mCurrAttackType)
	{
	case BossScript::NONE:
		break;
	case BossScript::ATTACK_01:
		UpdateAttack01();
		break;
	case BossScript::ATTACK_02:
		UpdateAttack02();
		break;
	default:
		break;
	}
}

void BossScript::UpdateAttack01()
{
	// 램프에서 검은 광선이 나와 왼쪽 아래 바닥부터 오른쪽 아래 바닥까지 이어지는 광역 딜을 날린다.

}

void BossScript::UpdateAttack02()
{
	// 머리 안에 손을 집어넣고 안에서 붉은 구체를 꺼내 하늘 위로 던진다. 이후 기본 몬스터 N마리(종류 랜덤)가 스테이지의 랜덤한 위치에 생성한다.

}

void BossScript::UpdateHit()
{

}

json BossScript::Serialize()
{
	json ret = MonoBehaviour::Serialize();

	ret["distance"] = mDistanceFromAxis.val;
	ret["min idle tick"] = mMinIdleTick.val;
	ret["max idle tick"] = mMaxIdleTick.val;

	return ret;
}

void BossScript::Deserialize(json& j)
{
	MonoBehaviour::Deserialize(j);

	if (j.contains("distance"))
	{
		mDistanceFromAxis.val = j["distance"].get<FLOAT>();
	}
	if (j.contains("min idle tick"))
	{
		mMinIdleTick.val = j["min idle tick"].get<FLOAT>();
	}
	if (j.contains("max idle tick"))
	{
		mMaxIdleTick.val = j["max idle tick"].get<FLOAT>();
	}
}
