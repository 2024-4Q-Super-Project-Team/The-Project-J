#include "pch.h"
#include "MonsterScript.h"

#include "Contents/GameApp/Script/Player/PlayerScript.h"
#include "Contents/GameApp/Script/Object/Burn/BurnObjectScript.h"
#include "Contents/GameApp/Script/Monster/ScopeScript.h"

#define MONSTER_ANIM_IDLE L"m001"
#define MONSTER_ANIM_WALK L"m002"
#define MONSTER_ANIM_FAST_WALK L"m003"
#define MONSTER_ANIM_RUN L"m004" 
#define MONSTER_ANIM_HIT L"m005"
#define MONSTER_ANIM_GROGGY L"m006"

void MonsterScript::Start()
{
	gameObject->SetTag(L"Monster");

	// Init Setting
	{
		Object* root = gameObject->transform->GetParent()->gameObject;
		auto& rootChildren = root->transform->GetChildren();

		// find Scope
		for (Transform* child : rootChildren)
		{
			if (child->gameObject->GetName() == L"Scope_A")
			{
				m_pScope = child->gameObject;
				m_pScope->transform->position = gameObject->transform->position;
			}
		}

		// find Weakness
		auto& children = gameObject->transform->GetChildren();
		for (Transform* child : children)
		{
			if (child->gameObject->GetName() == L"Weakness_A")
			{
				m_pWeakness = child->gameObject;
				m_pWeakness->SetTag(L"Weakness");
				m_pWeakness->transform->scale = Vector3(30, 30, 22);
				m_pWeakness->transform->SetEulerAngles(Vector3(Degree::ToRadian(90.0f), 0.0f, 0.0f));
				m_pWeakness->transform->position.y = 38;
			}
		}

		mFSM = eMonsterStateType::IDLE;
	}

	// Add componenet
	{	// Animator Component
		m_pAnimator = gameObject->GetComponent<Animator>();
		if (!m_pAnimator)
		{
			m_pAnimator = gameObject->AddComponent<Animator>();
		}
	}
	{	// RigidBody Component
		m_pRigidBody = gameObject->AddComponent<Rigidbody>();
	}
	{	// Head Collider Component
		m_pHeadCollider = gameObject->AddComponent<BoxCollider>();
		m_pHeadCollider->SetPosition(Vector3{0,85,0});
		m_pHeadCollider->SetExtents(Vector3{ 25,2,25 });
		m_pHeadCollider->SetIsTrigger(true);
	}
	{	// Body Collider Component
		m_pBodyCollider = gameObject->AddComponent<BoxCollider>();
		m_pBodyCollider->SetPosition(Vector3{ 0,40,0 });
		m_pBodyCollider->SetExtents(Vector3{ 28,40,28 });
	}
	{	// BurnObjectScript Component
		m_pBurnObjectScript = gameObject->AddComponent<BurnObjectScript>();
	}
}

void MonsterScript::Update()
{
	auto* scope = m_pScope->GetComponent<ScopeScript>();

	if (scope)
		scope->SetMonster(gameObject);

	switch (mFSM)
	{
	case eMonsterStateType::IDLE:
		UpdateIdle();
		break;
	case eMonsterStateType::WALK:
		UpdateWalk();
		break;
	case eMonsterStateType::FAST_WALK:
		UpdateFastWalk();
		break;
	case eMonsterStateType::RUN:
		UpdateRun();
		break;
	case eMonsterStateType::HIT:
		UpdateHit();
		break;
	case eMonsterStateType::GROGGY:
		UpdateGroggy();
		break;
	case eMonsterStateType::DEAD:
		UpdateDead();
		break;
	default:
		break;
	}

	UpdateMonsterAnim();
	UpdateMonsterAngle();
}

void MonsterScript::UpdateIdle()
{
	mIdleCount += Time::GetUnScaledDeltaTime();

	if (mIdleCount > 3.0f)
	{
		mIdleCount = 0.0f;
		mFSM = eMonsterStateType::WALK;
	}
}

void MonsterScript::UpdateWalk()
{
	UpdateMonsterAngle();

	// Ÿ���� �����ϴ°�?
	if (m_pTarget)
	{
		mFSM = eMonsterStateType::FAST_WALK;
	}
	
	mResetCount += Time::GetUnScaledDeltaTime();

	if (mResetCount > 3.0f)
	{
		// ������ ���� ���� ���� ���� ����� �������ֱ�
		if (bIsScope)
		{
			mTargetPos.x = Random::Range(mTargetPos.x - mRange, mTargetPos.x + mRange);
			mTargetPos.y = gameObject->transform->position.y;
			mTargetPos.z = Random::Range(mTargetPos.z - mRange, mTargetPos.z + mRange);
		}
		else
		{
			mTargetPos.x = m_pScope->transform->position.x;
			mTargetPos.y = gameObject->transform->position.y;
			mTargetPos.z = m_pScope->transform->position.z;
		}

		// ���� ���� ����
		mTargetDir = mTargetPos - gameObject->transform->position;
		// �Ÿ� ����
		mDistance = mTargetDir.Length();
		// ���� ����
		mTargetDir.Normalize();

		// �Ÿ��� ������ �̻��̸� ������ �ٱ����� ����
		if (mDistance > 100.f)
			bIsScope = false;

		mResetCount = 0.f;
	}
	else
	{
		if (mDistance > 0.1f)
		{
			gameObject->transform->position += mTargetDir * mMoveSpeed.val * Time::GetUnScaledDeltaTime();
		}
		else 
		{
			//m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_IDLE, 0.5f);
		}
	}
}

void MonsterScript::UpdateFastWalk()
{
	UpdateMonsterAngle();

	// Ÿ���� �����ϴ°�?
	if (m_pTarget)
	{
		// Ÿ�� ����
		mTargetPos = {	m_pTarget->transform->position.x,
								gameObject->transform->position.y, 
								m_pTarget->transform->position.z };
		// Ÿ�� ����
		mTargetDir = mTargetPos - gameObject->transform->position;
		// �Ÿ�
		float distance = mTargetDir.Length();
		// ���� 
		mTargetDir.Normalize();

		// �Ÿ��� ������ ���� �ݰ溸�� Ŭ �������� ��� �����ֱ�
		if (distance > mAttackDistance.val)
		{
			gameObject->transform->position += mTargetDir * mMoveSpeed.val * 1.5 * Time::GetUnScaledDeltaTime();
		}
		else // Ÿ�ٰ��� �Ÿ��� ���� ���� ����� ���� ���·� ��ȯ
		{
			mFSM = eMonsterStateType::RUN;
		}
	}
	else // Ÿ���� �������� �ʴ°�?
	{
		mFSM = eMonsterStateType::WALK;
	}
}

void MonsterScript::UpdateRun()
{
	UpdateMonsterAngle();

	// Ÿ���� �����ϴ°�?
	if (m_pTarget)
	{
		// Ÿ�� ����
		mTargetPos = {	m_pTarget->transform->position.x,
								gameObject->transform->position.y, 
								m_pTarget->transform->position.z };
		// Ÿ�� ����
		mTargetDir = mTargetPos - gameObject->transform->position;
		// �Ÿ�
		float distance = mTargetDir.Length();
		// ����
		mTargetDir.Normalize();

		// �Ÿ��� ���� �ݰ溸�� ũ�ٸ� �ٽ� ���� ��ȯ
		if (distance > mAttackDistance.val)
			mFSM = eMonsterStateType::FAST_WALK;

		// �Ÿ��� ���� �ݰ溸�� �۴ٸ� ����
		// Ÿ�� ��ġ�� ������ �̵�
		gameObject->transform->position += mTargetDir * mMoveSpeed.val * 2.f * Time::GetUnScaledDeltaTime();
	}
	else // Ÿ���� �������� �ʴ°�?
	{
		mFSM = eMonsterStateType::WALK;
	}
}

void MonsterScript::UpdateHit()
{

}

void MonsterScript::UpdateGroggy()
{
	m_pHeadCollider->SetPosition(Vector3{ 0,45,0 });
	m_pBodyCollider->SetPosition(Vector3{ 0,23,0 });
	m_pBodyCollider->SetExtents(Vector3{ 28,22,28 });

	if (m_pAnimator->IsEnd())
	{
		Vector3 pos = { m_pWeakness->transform->position.x,
						m_pWeakness->transform->position.y + 0.f,
						m_pWeakness->transform->position.z };
	}

	if (mGroggyCount < mGroggyTick.val)
	{
		// GroggyCount ����
		mGroggyCount += Time::GetUnScaledDeltaTime();

		// N�� �ȿ� ���� �پ��ٸ�
		if (m_pBurnObjectScript)
		{
			if (m_pBurnObjectScript->IsBurning())
			{
				Vector3 pos = { m_pWeakness->transform->position.x,
						m_pWeakness->transform->position.y - 0.f,
						m_pWeakness->transform->position.z };

				mFSM = eMonsterStateType::DEAD;
				mGroggyCount = 0.f;
			}
		}
	}
	else
	{
		m_pHeadCollider->SetPosition(Vector3{ 0,85,0 });
		m_pBodyCollider->SetPosition(Vector3{ 0,40,0 });
		m_pBodyCollider->SetExtents(Vector3{ 28,40,28 });

		mGroggyCount = 0.f;
		mFSM = eMonsterStateType::IDLE;
	}
}

void MonsterScript::UpdateDead()
{
	if (m_pAnimator)
	{
		if (m_pAnimator->IsEnd())
		{
			// �ִϸ��̼��� ������ �� �ʵ� ���� ��Ȱ��ȭ
			mResetCount += Time::GetUnScaledDeltaTime();

			if (mResetCount > 3.f)
			{
				mResetCount = 0.f;
				gameObject->transform->GetParent()->gameObject->SetActive(false);
			}
		}
	}
}

void MonsterScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
	// �����̶� �÷��̾ �ε����� ��
	if (_destination->gameObject->GetTag() == L"Player")
	{
		auto* player = _destination->GetOwner()->GetComponent<PlayerScript>();
		if (player)
		{
			if (mFSM != eMonsterStateType::DEAD && mFSM != eMonsterStateType::GROGGY)
			{
				// 3��ŭ ������ ������
				player->Hit(mDamage.val);
			}
		}

		// ������ ������ �ٽ� ó�� ���·�
		//mFSM = eMonsterStateType::IDLE;
	}
}

void MonsterScript::OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination)
{
	
}

void MonsterScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{

}

void MonsterScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{
	// �Ӹ� ������ ��
	if (_destination->gameObject->GetTag() == L"Player")
	{
		if (m_pTarget)
		{
			auto* player = m_pTarget->GetComponent<PlayerScript>();
			if (player)
			{
				player->Jump(0.5f, false);
			}
		}
		// GROGGY�� �ٲ��ֱ�
		mFSM = eMonsterStateType::GROGGY;
	}
}

void MonsterScript::OnTriggerStay(Collider* _origin, Collider* _destination)
{
	
}

void MonsterScript::OnTriggerExit(Collider* _origin, Collider* _destination)
{

}

//json MonsterScript::Serialize()
//{
//	json ret = MonoBehaviour::Serialize();
//
//	return ret;
//}
//
//void MonsterScript::Deserialize(json& j)
//{
//	MonoBehaviour::Deserialize(j);
//}

void MonsterScript::UpdateMonsterAnim()
{
	if (!m_pAnimator)
		return;

	switch (mFSM)
	{
	case eMonsterStateType::IDLE:
		m_pAnimator->SetLoop(true);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_IDLE, 0.5f);
		break;
	case eMonsterStateType::WALK:
		m_pAnimator->SetLoop(true);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_WALK, 0.5f);
		break;
	case eMonsterStateType::FAST_WALK:
		m_pAnimator->SetLoop(true);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_FAST_WALK, 0.5f);
		break;
	case eMonsterStateType::RUN:
		m_pAnimator->SetLoop(true);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_RUN, 0.5f);
		break;
	case eMonsterStateType::HIT:
		m_pAnimator->SetLoop(false);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_HIT, 0.5f);
		break;
	case eMonsterStateType::GROGGY:
		m_pAnimator->SetLoop(false);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_GROGGY, 0.5f);
		break;
	case eMonsterStateType::DEAD:
		break;
	}
}

void MonsterScript::UpdateMonsterAngle()
{
	Vector3 pos = gameObject->transform->position;
	Vector2 viewDir = { mTargetPos.x - pos.x, mTargetPos.z - pos.x };
	float targetAngleY = atan2(-viewDir.x, -viewDir.y);
	
	if (targetAngleY < 0.0f)
	{
		targetAngleY += XM_2PI;
	}

	Vector3 currAngle = gameObject->transform->GetEulerAngles();
	float currAngleY = currAngle.y;

	float delta = fmod(targetAngleY - currAngleY + XM_PI, XM_2PI) - XM_PI;
	float newAngleY = currAngleY + delta * 0.7f;

	gameObject->transform->SetEulerAngles(Vector3(0.f, newAngleY, 0.f));
}
