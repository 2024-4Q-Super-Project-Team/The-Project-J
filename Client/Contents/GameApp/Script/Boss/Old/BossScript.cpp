#include "pch.h"
#include "BossScript.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"

#include "Contents/GameApp/Script/GameProgressManager.h"
#include "Boss_Attack01_Script.h"

#define BOSS_ANIM_IDLE		L"001"		
#define BOSS_ANIM_ATTACK_01 L"003"		
#define BOSS_ANIM_ATTACK_02 L"004"				// 005�� 004�� �ϳ���
#define BOSS_ANIM_HIT							// �ǰ� �ִϸ��̼� X

/*	
*   ���� ���� �� ���� ����
*	��� ������Ʈ �̸� �����ϱ�
*	Boss ������Ʈ �̸� ���߱�
*	Boss_Attack01 = ������ ������Ʈ
*	���� �� �̸� �����ϱ� ( Origin : Bone.005 -> Boss_Lamp_Bone )
*/

void BossScript::Start()
{
	// ���� �� ������Ʈ�� ã�´�.
	Object* axisObject = FindObjectWithName(L"Stage_lighthouse.fbx");
	
	if (axisObject)
	{
		// ���� �� Ʈ�������� �ʱ�ȭ���ش�.
		mAxisTransform = axisObject->transform;
	}
	auto& Children = gameObject->transform->GetChildren();
	for (Transform* child : Children)
	{
		if (child->gameObject->GetName() == L"Boss_Body")
			mBodyObject = child->gameObject;
		if (child->gameObject->GetName() == L"Boss_Head")
			mHeadObject = child->gameObject;
		if (child->gameObject->GetName() == L"Boss_Attack01")
			mRazerObject = child->gameObject;
	}

	// �ִϸ����� �ʱ�ȭ
	mBodyAnimator = mBodyObject->GetComponent<Animator>();
	//mHeadAnimator = mHeadObject->GetComponent<Animator>();

	// ����Ʈ �ڵ�
	mSpawnEffectTextrueHandle.mResourceType = eResourceType::Texture2DResource;
	mSpawnEffectTextrueHandle.mMainKey = L"";

	mRazerObject->SetActive(false);
	mRazerScript = mRazerObject->AddComponent<Boss_Attack01_Script>();
	mRazerScript->SetRootObject(gameObject);
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
	PlayerScript* Player1 = GameProgressManager::GetPlayerInfo(0);
	PlayerScript* Player2 = GameProgressManager::GetPlayerInfo(1);

	Vector3 Player1WorldPos = Player1->gameObject->transform->GetWorldPosition();
	Vector3 Player2WorldPos = Player2->gameObject->transform->GetWorldPosition();

	// �÷��̾� �� �߰� ��ġ ���� ���
	Vector3 BetweenPos = (Player1WorldPos + Player2WorldPos) * 0.5f;
	Vector3 AxisPos = mAxisTransform->GetWorldPosition();

	// �߽� �࿡�� ������ �ٶ� ���� ����
	Vector3 ViewDirection = BetweenPos - AxisPos;
	ViewDirection.Normalize();

	// ������ ��ġ ��� (= �߽� �� + ���� ���� * �߽� �����κ����� �Ÿ�)
	Vector3 NewPosition = AxisPos + (mDistanceFromAxis.val * ViewDirection);
	mBodyObject->transform->position.x = NewPosition.x;
	mBodyObject->transform->position.z = NewPosition.z;

	float targetAngleY = atan2(-ViewDirection.x, -ViewDirection.z); // ���� ����
	if (targetAngleY < 0.0f)
		targetAngleY += XM_2PI;  // 360�� ��� 2�� ���
	mBodyObject->transform->SetEulerAngles(Vector3(0.0f, targetAngleY, 0.0f));

	mRazerScript->SetRootView(ViewDirection);
	mRazerScript->SetRootAngleY(targetAngleY);
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
		mCurrAttackType == ATTACK_01 ?
			mBodyAnimator->SetCurrentAnimation(BOSS_ANIM_ATTACK_01, 0.5f) :
			mBodyAnimator->SetCurrentAnimation(BOSS_ANIM_ATTACK_02, 0.5f);
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
		//mCurrAttackType = (eBossAttackType)Random::Range<INT>(ATTACK_01, ATTACK_02);
		mCurrAttackType = ATTACK_01;
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

#define BOSS_ATTACK_01_TRIGGER_FRAME 1	// ������ ������ �����ϴ� ������
void BossScript::UpdateAttack01()
{
	if (mBodyAnimator->GetActiveAnimationKey() == BOSS_ANIM_ATTACK_01)
	{
		// �������� ���� ������ ���� ���� �Ʒ� �ٴں��� ������ �Ʒ� �ٴڱ��� �̾����� ���� ���� ������.
		if (mBodyAnimator->GetDuration() >= BOSS_ATTACK_01_TRIGGER_FRAME)
		{
			// ������ ��Ȱ��ȭ ���̸� Ȱ��ȭ
			if (mRazerObject->GetState() == EntityState::Passive)
			{
				mRazerObject->SetActive(true);
				isRazerSpawn = TRUE;
				mRazerScript->SetAttackStart();
			}
		}
		if (mBodyAnimator->IsEnd())
		{
			mCurrAttackType = NONE;
			SetState(eBossStateType::IDLE);
			mRazerObject->SetActive(false);
		}
	}
}

#define BOSS_ATTACK_02_TRIGGER_FRAME 1	// ��ü�� ������ ������
void BossScript::UpdateAttack02()
{
	// �Ӹ� �ȿ� ���� ����ְ� �ȿ��� ���� ��ü�� ���� �ϴ� ���� ������. ���� �⺻ ���� N����(���� ����)�� ���������� ������ ��ġ�� �����Ѵ�.
	//if (mBodyAnimator->GetDuration() >= BOSS_ATTACK_02_TRIGGER_FRAME)
	//{
	//
	//}
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
