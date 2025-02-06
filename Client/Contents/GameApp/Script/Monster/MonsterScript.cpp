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

	Object* root = gameObject->transform->GetParent()->gameObject;
	auto& Children = root->transform->GetChildren();
	
	for (Transform* child : Children)
	{
		if (child->gameObject->GetName() == L"Scope_A")
			m_pScope = child->gameObject;
	}

	// Init Setting
	{
		mFSM = eMonsterStateType::IDLE;
	}

	// Add Object
	{
		if (!m_pScope)
			m_pScope = CreateObject(L"Scope_A", L"Scope");

		m_pScope->transform->position = gameObject->transform->position;
		m_pScope->AddComponent<ScopeScript>();
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
		m_pHeadCollider->SetExtents(Vector3{ 15,5,15 });
		m_pHeadCollider->SetIsTrigger(true);
	}
	{	// Body Collider Component
		m_pBodyCollider = gameObject->AddComponent<BoxCollider>();
		m_pBodyCollider->SetPosition(Vector3{ 0,30,0 });
		m_pBodyCollider->SetExtents(Vector3{ 30,45,30 });
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
	case eMonsterStateType::DEAD:
		UpdateDead();
		break;
	default:
		break;
	}

	UpdateMonsterAnim();
}

void MonsterScript::UpdateIdle()
{
	mResetCount += Time::GetUnScaledDeltaTime();

	if (mResetCount > 3.f)
	{
		mResetCount = 0.f;
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
	else // Ÿ���� �������� �ʴ°�?
	{
		Vector3 TargetPos{};

		if (bIsScope)
		{
			mResetCount += Time::GetUnScaledDeltaTime();

			float x = 0.f;
			float z = 0.f;

			if (mResetCount > 5.f)
			{
				x = Random::Range(-240, 240);  // x ����
				z = Random::Range(-240, 240);  // z ����
			}
			// ���� ���� ����
			TargetPos = { x, gameObject->transform->position.y, z }; // y ���� ����

			mResetCount = 0;
		}	// ���� ��Ż�� ���� ��ġ�� �̵�
		else
		{
			TargetPos = m_pScope->transform->position;
		}

		Vector3 dir = TargetPos - gameObject->transform->position;
		Vector3 targetDir = { dir.x, gameObject->transform->position.y, dir.z };
		float distance = targetDir.Length();	// �Ÿ� ���ϱ�
		targetDir.Normalize();  // ���� ���ϱ�

		// ���� �������� �̵�
		if (gameObject->transform->position.x != TargetPos.x || gameObject->transform->position.z != TargetPos.z)
			gameObject->transform->position += targetDir * mMoveSpeed.val * Time::GetUnScaledDeltaTime();
		else
			mFSM = eMonsterStateType::IDLE;
	}
}

void MonsterScript::UpdateFastWalk()
{
	// Ÿ���� �����ϴ°�?
	if (m_pTarget)
	{
		Vector3 dir = m_pTarget->transform->position - gameObject->transform->position;
		Vector3 targetDir = { dir.x, gameObject->transform->position.y, dir.z };
		float distance = targetDir.Length();	// �Ÿ� ���ϱ�
		targetDir.Normalize();  // ���� ���ϱ�

		if (distance > mAttackDistance.val)
		{
			gameObject->transform->position += targetDir * mMoveSpeed.val * 1.5 * Time::GetUnScaledDeltaTime();
		}
		else // Ÿ�ٰ��� �Ÿ��� ���� ����� ����
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
		Vector3 dir = m_pTarget->transform->position - gameObject->transform->position;
		Vector3 targetDir = { dir.x, gameObject->transform->position.y, dir.z };
		float distance = targetDir.Length();	// �Ÿ� ���ϱ�
		targetDir.Normalize();  // ���� ���ϱ�

		if (distance > mAttackDistance.val)
			mFSM = eMonsterStateType::FAST_WALK;

		// Ÿ�� ��ġ�� ������ �̵�
		gameObject->transform->position += targetDir * mMoveSpeed.val * 2.f * Time::GetUnScaledDeltaTime();
	}
	else // Ÿ���� �������� �ʴ°�?
	{
		// MOVE �� ü����
		mFSM = eMonsterStateType::WALK;
	}
}

void MonsterScript::UpdateHit()
{
	// GroggyCount ����
	mGroggyCount += Time::GetUnScaledDeltaTime();
	m_pAnimator->SetLoop(false);

	if (mGroggyCount > mGroggyTick.val)
	{
		// N�� �ȿ� ���� �پ��ٸ�
		if (m_pBurnObjectScript)
		{
			if (m_pBurnObjectScript->IsBurning())
			{

				mGroggyCount = 0.f;
				mFSM = eMonsterStateType::DEAD;
			}
			else
				mGroggyCount = 0.f;
		}
	}
	else // N�ʾȿ� ���� ���� �ʾҴٸ� 
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
		if (player && mFSM == eMonsterStateType::RUN)
		{
			// 3��ŭ ������ ������
			// player->Hit(3.f);
		}

		// ������ ������ �ٽ� ó�� ���·�
		mFSM = eMonsterStateType::IDLE;
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