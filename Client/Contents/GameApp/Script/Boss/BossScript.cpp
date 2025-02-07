#include "pch.h"
#include "BossScript.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"

#include "Manager/PlayerManager.h"

#define BOSS_ANIM_IDLE L"001"
#define BOSS_ANIM_ATTACK_01 L"003"
#define BOSS_ANIM_ATTACK_02 L"004" // 005�� 004�� �ϳ���
#define BOSS_ANIM_HIT		// �ǰ� �ִϸ��̼� X

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
	// ��λ��� : ���峻�� ������ ���ϸ� ���������ǿ� ���� ������ ���� �� ����. �� ������ ����?

	// �� �÷��̾��� ���� ���� ���� ���Ѵ�.
	PlayerScript* Player1 = PlayerManager::GetPlayerInfo(0);
	PlayerScript* Player2 = PlayerManager::GetPlayerInfo(1);

	Vector3 Player1WorldPos = Player1->gameObject->transform->GetWorldPosition();
	Vector3 Player2WorldPos = Player2->gameObject->transform->GetWorldPosition();

	// �÷��̾� �� �߰� ��ġ ���� ���
	Vector3 BetweenPos = (Player1WorldPos + Player2WorldPos) * 0.5f;
	Vector3 AxisPos = mAxisTransform->GetWorldPosition();

	// �߽� �࿡�� ������ �ٶ� ���� ����
	Vector3 ViewDirection = BetweenPos - AxisPos;
	ViewDirection.Normalize();

	// ������ ��ġ ��� (= �߽� �� + ���� ���� * �߽� �����κ����� �Ÿ�)
	gameObject->transform->position = AxisPos + (mDistanceFromAxis.val * ViewDirection);

	// ������ ȸ�� ���
	float targetAngleY = atan2(ViewDirection.x, ViewDirection.y);
	if (targetAngleY < 0.0f)
		targetAngleY += XM_2PI;  // 360�� ��� 2�� ���
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
	// ���� ��� ī��Ʈ�� ���� ��� �ð����� Ŀ����
	if (mIdleTickCounter >= mCurIdleTime)
	{
		mIdleTickCounter = 0.0f;
		mCurIdleTime = Random::Range(mMinIdleTick.val, mMaxIdleTick.val);
		// ���� �ൿ ������ ����.
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
	// �������� ���� ������ ���� ���� �Ʒ� �ٴں��� ������ �Ʒ� �ٴڱ��� �̾����� ���� ���� ������.

}

void BossScript::UpdateAttack02()
{
	// �Ӹ� �ȿ� ���� ����ְ� �ȿ��� ���� ��ü�� ���� �ϴ� ���� ������. ���� �⺻ ���� N����(���� ����)�� ���������� ������ ��ġ�� �����Ѵ�.

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
