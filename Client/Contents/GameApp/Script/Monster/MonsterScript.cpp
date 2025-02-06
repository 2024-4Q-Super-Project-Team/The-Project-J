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
#define MONSTER_ANIM_DEAD L"m006"

void MonsterScript::Start()
{
	gameObject->SetTag(L"Monster");

	// Init Setting
	{
		Object* root = gameObject->transform->GetParent()->gameObject;
		auto& Children = root->transform->GetChildren();

		for (Transform* child : Children)
		{
			if (child->gameObject->GetName() == L"Scope_A")
			{
				m_pScope = child->gameObject;
				m_pScope->transform->position = gameObject->transform->position;

				if (!m_pScope)
				{
					m_pScope = CreateObject(L"Scope_A", L"Scope");
				}
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
	UpdateMonsterAnim();

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
	case eMonsterStateType::DEAD:
		UpdateDead();
		break;
	default:
		break;
	}
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
			mRandomPos.x = Random::Range(mRandomPos.x - mRange, mRandomPos.x + mRange);
			mRandomPos.y = gameObject->transform->position.y;
			mRandomPos.z = Random::Range(mRandomPos.z - mRange, mRandomPos.z + mRange);
		}
		else
		{
			mRandomPos.x = m_pScope->transform->position.x;
			mRandomPos.y = gameObject->transform->position.y;
			mRandomPos.z = m_pScope->transform->position.z;
		}

		// ���� ���� ����
		mRandomDir = mRandomPos - gameObject->transform->position;
		// �Ÿ� ����
		mDistance = mRandomDir.Length();
		// ���� ����
		mRandomDir.Normalize();

		// �Ÿ��� ������ �̻��̸� ������ �ٱ����� ����
		if (mDistance > 100.f)
			bIsScope = false;

		mResetCount = 0.f;
	}
	else
	{
		if (mDistance > 0)
		{
			gameObject->transform->position += mRandomDir * mMoveSpeed.val * Time::GetUnScaledDeltaTime();
		}
		else 
		{
			m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_IDLE);
		}
	}
}

void MonsterScript::UpdateFastWalk()
{
	// Ÿ���� �����ϴ°�?
	if (m_pTarget)
	{
		// Ÿ�� ����
		Vector3 targetPos = {	m_pTarget->transform->position.x, 
								gameObject->transform->position.y, 
								m_pTarget->transform->position.z };
		// Ÿ�� ����
		Vector3 targetDir = targetPos - gameObject->transform->position;
		// �Ÿ�
		float distance = targetDir.Length();
		// ���� 
		targetDir.Normalize();

		// �Ÿ��� ������ ���� �ݰ溸�� Ŭ �������� ��� �����ֱ�
		if (distance > mAttackDistance.val)
		{
			gameObject->transform->position += targetDir * mMoveSpeed.val * 1.5 * Time::GetUnScaledDeltaTime();
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
	// Ÿ���� �����ϴ°�?
	if (m_pTarget)
	{
		// Ÿ�� ����
		Vector3 targetPos = {	m_pTarget->transform->position.x, 
								gameObject->transform->position.y, 
								m_pTarget->transform->position.z };
		// Ÿ�� ����
		Vector3 targetDir = targetPos - gameObject->transform->position;
		// �Ÿ�
		float distance = targetDir.Length();
		// ����
		targetDir.Normalize();

		// �Ÿ��� ���� �ݰ溸�� ũ�ٸ� �ٽ� ���� ��ȯ
		if (distance > mAttackDistance.val)
			mFSM = eMonsterStateType::FAST_WALK;

		// �Ÿ��� ���� �ݰ溸�� �۴ٸ� ����
		// Ÿ�� ��ġ�� ������ �̵�
		gameObject->transform->position += targetDir * mMoveSpeed.val * 2.f * Time::GetUnScaledDeltaTime();
	}
	else // Ÿ���� �������� �ʴ°�?
	{
		mFSM = eMonsterStateType::WALK;
	}
}

void MonsterScript::UpdateHit()
{
	if (mGroggyCount < mGroggyTick.val)
	{
		// GroggyCount ����
		mGroggyCount += Time::GetUnScaledDeltaTime();

		// N�� �ȿ� ���� �پ��ٸ�
		if (m_pBurnObjectScript)
		{
			if (m_pBurnObjectScript->IsBurning())
			{
				mFSM = eMonsterStateType::DEAD;
				mGroggyCount = 0.f;
			}
		}
	}
	else
	{
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
				gameObject->SetActive(false);
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
			// 3��ŭ ������ ������
			player->Hit(mDamage.val);
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
		// HIT�� �ٲ��ֱ�
		mFSM = eMonsterStateType::HIT;
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
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_IDLE);
		break;
	case eMonsterStateType::WALK:
		m_pAnimator->SetLoop(true);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_WALK);
		break;
	case eMonsterStateType::FAST_WALK:
		m_pAnimator->SetLoop(true);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_FAST_WALK);
		break;
	case eMonsterStateType::RUN:
		m_pAnimator->SetLoop(true);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_RUN);
		break;
	case eMonsterStateType::HIT:
		m_pAnimator->SetLoop(false);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_HIT);
		break;
	case eMonsterStateType::DEAD:
		m_pAnimator->SetLoop(false);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_DEAD);
		break;
	}
}