#include "pch.h"
#include "MonsterScript.h"

#include "Contents/GameApp/Script/Player/PlayerScript.h"
#include "Contents/GameApp/Script/Object/Burn/BurnObjectScript.h"
#include "Contents/GameApp/Script/Monster/ScopeScript.h"

void MonsterScript::Start()
{
	gameObject->SetTag(L"Monster");

	// Init Setting
	{
		mFSM = eMonsterStateType::MOVE;
	}

	// Add Object
	{
		m_pScope = CreateObject(L"Scope_01", L"Scope");
		m_pScope->transform->position = gameObject->transform->position;
		auto* scope = m_pScope->AddComponent<ScopeScript>();

		if (scope)
			scope->SetMonster(gameObject);
	}

	// Add componenet
	{	// Animator Component
		m_pAnimator = gameObject->AddComponent<Animator>();
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
		m_pBodyCollider->SetPosition(Vector3{ 0,12,0 });
		m_pBodyCollider->SetExtents(Vector3{ 30,30,30 });
	}
	{	// BurnObjectScript Component
		m_pBurnObjectScript = gameObject->AddComponent<BurnObjectScript>();
	}
}

void MonsterScript::Update()
{
	switch (mFSM)
	{
	case eMonsterStateType::MOVE:
		if (m_pAnimator)
		{
			Display::Console::Log("MOVE");
			// TODO : ���� �ִϸ��̼� ���
		}

		// Ÿ���� �����ϴ°�?
		if (m_pTarget)
		{
			Vector3 dir = m_pTarget->transform->position - gameObject->transform->position;
			float distance = dir.Length();	// �Ÿ� ���ϱ�
			dir.Normalize();  // ���� ���ϱ�
			if (distance > mAttackDistance.val)
			{	// Ÿ�ٰ��� �Ÿ��� ���� ���̶�� �̵�
				gameObject->transform->position += dir * mMoveSpeed.val;
			}
			else
			{
				// Ÿ�ٰ��� �Ÿ��� ���� ����� ����
				mFSM = eMonsterStateType::ATTACK;
			}
		}
		else // Ÿ���� �������� �ʴ°�?
		{
			// ���� ���� ����
			float x = Random::Range(-100.f, 100.f);  // x ����
			float z = Random::Range(-100.f, 100.f);  // z ����
			Vector3 TargetPos = {x, gameObject->transform->position.y, z}; // y ���� ����

			// ���� ��Ż�� ���� ��ġ�� �̵�
			if (!bIsScope)
				TargetPos = m_pScope->transform->position;

			Vector3 dir = TargetPos - gameObject->transform->position;
			float distance = dir.Length();	// �Ÿ� ���ϱ�
			dir.Normalize();  // ���� ���ϱ�
			// ���� �������� �̵�
			gameObject->transform->position += dir * mMoveSpeed.val;
		}
		break;
	case eMonsterStateType::ATTACK:
		if (m_pAnimator)
		{
			Display::Console::Log("ATTACK");
			// TODO : �߰� �ִϸ��̼� ���
		}

		// Ÿ���� �����ϴ°�?
		if (m_pTarget)
		{
			Vector3 dir = m_pTarget->transform->position - gameObject->transform->position;
			float distance = dir.Length();	// �Ÿ� ���ϱ�
			dir.Normalize();  // ���� ���ϱ�

			// Ÿ�� ��ġ�� ������ �̵�
			gameObject->transform->position += dir * mMoveSpeed.val * 2.f;
		}
		else // Ÿ���� �������� �ʴ°�?
		{
			// MOVE �� ü����
			mFSM = eMonsterStateType::MOVE;
		}
		break;
	case eMonsterStateType::GROGGY:
		if (m_pAnimator)
		{
			Display::Console::Log("GROGGY");
			// TODO : ���� �ִϸ��̼� ���
		}

		// GroggyCount ����
		mGroggyCount += Time::GetUnScaledDeltaTime();

		// N�� �ȿ� ���� �پ��ٸ�
		if (mGroggyCount < mGroggyTick.val && m_pBurnObjectScript)
		{	
			if (m_pBurnObjectScript->IsBurning())
			{
				mGroggyCount = 0.f;
				mFSM = eMonsterStateType::DIE;
			}
			else
				mGroggyCount = 0.f;
		}
		else // N�ʾȿ� ���� ���� �ʾҴٸ� 
		{
			if (m_pAnimator)
			{
				Display::Console::Log("InverseGROGGY");
				// TODO : �ִϸ��̼� �����
			}

			mGroggyCount = 0.f;
			mFSM = eMonsterStateType::MOVE;
		}
		break;
	case eMonsterStateType::DIE:
		if (m_pAnimator)
		{
			Display::Console::Log("DIE");
			// TODO : �״� �ִϸ��̼� ��� ��
			// ������Ʈ ����
		}
		break;
	default:
		break;
	}
}

void MonsterScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
	// �����̶� �÷��̾ �ε����� ��
	if (_destination->gameObject->GetTag() == L"Player")
	{
		auto* player = _destination->GetOwner()->GetComponent<PlayerScript>();
		if (player && mFSM == eMonsterStateType::ATTACK)
		{
			// 3��ŭ ������ ������
			// player->Hit(3.f);
		}

		// ������ ������ �ٽ� ���� ���·�
		mFSM = eMonsterStateType::MOVE;
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
