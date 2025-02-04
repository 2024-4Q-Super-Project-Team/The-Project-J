#include "pch.h"
#include "MonsterScript.h"

#include "Contents/GameApp/Script/Player/PlayerScript.h"
#include "Contents/GameApp/Script/Object/Burn/BurnObjectScript.h"

void MonsterScript::Start()
{
	gameObject->SetTag(L"Monster");

	// Init Setting
	mFSM = eMonsterStateType::IDLE;

	// Add componenet
	
	{	// Animator Component
		m_pAnimator = gameObject->AddComponent<Animator>();
	}

	{	// Head Collider Component
		m_pHeadCollider = gameObject->AddComponent<BoxCollider>();
	}

	{	// Body Collider Component
		m_pBodyCollider = gameObject->AddComponent<SphereCollider>();
	}

	{	// BurnObjectScript Component
		m_pBurnObjectScript = gameObject->AddComponent<BurnObjectScript>();
	}
}

void MonsterScript::Update()
{
	// Monster_A �ý��� ����
	// 1. ���� ���� ���� �÷��̾� �����ϴ��� üũ
	//		������ ���� �ʴ´ٸ� ���� ����
	//		������ �Ѵٸ� Ÿ�� ��ġ��
	//			��Ÿ�� ���� �� �÷��̾� ���� üũ (�����Ѵٸ� �÷��̾�� �߰� ����)
	// 
	//  --<�߰� ���� �ִϸ��̼� ��>--
	//		 2. �÷��̾�� �浹�ߴ��� üũ
	//			���Ӹ� �ݶ��̴� �浹 üũ (�浹�̸� groggy)
	//			������ �ݶ��̴� �浹 üũ (�浹�̸� �÷��̾� ü�� ����)
	//			���浹���� �ʾҴٸ� ������ �� �ٽ� ���� üũ
	//		
	//		--<Groggy ���� ��>--
	//			3. N�� �̳��� ���� �پ��°�? (��ȣ�ۿ� ���� Ȯ��)
	//				���پ��ٸ� DIE
	//				���Ⱥپ��ٸ� �ٽ� IDLE
	//
	// 4. ���Ͱ� ���� ������ ������ �ʵ��� ���ǰ� �ʿ���

	switch (mFSM)
	{
	case eMonsterStateType::IDLE:
		//Display::Console::Log("IDLE");
		if (m_pAnimator)
		{
			// TODO : ���̵� �ִϸ��̼� ���
		}
		break;
	case eMonsterStateType::MOVE:
		Display::Console::Log("MOVE");
		if (m_pAnimator)
		{
			// TODO : ���� �ִϸ��̼� ���
			m_pAnimator->Play();
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
		} // Ÿ���� �������� �ʴ°�?
		else
		{
			Vector3 dir = mRandomPos - gameObject->transform->position;
			float distance = dir.Length();	// �Ÿ� ���ϱ�
			dir.Normalize();  // ���� ���ϱ�
			// ���� �������� �̵�
			gameObject->transform->position += dir * mMoveSpeed.val;
		}
		break;
	case eMonsterStateType::ATTACK:
		Display::Console::Log("ATTACK");
		if (m_pAnimator)
		{
			// TODO : �߰� �ִϸ��̼� ���
		}

		if (m_pTarget)
		{
			// ���� ���� ���ϱ�
			Vector3 dir = m_pTarget->transform->position - gameObject->transform->position;
			float distance = dir.Length();	// �Ÿ� ���ϱ�
			dir.Normalize();  // ���� ���ϱ�

			// Ÿ�� ��ġ�� ������ �̵�
			gameObject->transform->position += dir * mMoveSpeed.val * 1.5f;
		}
		break;
	case eMonsterStateType::HIT:
		Display::Console::Log("HIT");
		break;
	case eMonsterStateType::GROGGY:
		Display::Console::Log("GROGGY");

		if (m_pAnimator)
		{
			// TODO : ���� �ִϸ��̼� ���
		}

		mGroggyCount += Time::GetUnScaledDeltaTime();

		if (mGroggyCount < mGroggyTick.val) // && m_pBurnObjectScript->IsBurning()
		{	// N�� �ȿ� ���� �پ��ٸ�
			mGroggyCount = 0.f;
			mFSM = eMonsterStateType::DIE;
		}
		else
		{	// N�ʾȿ� ���� ���� �ʾҴٸ� 
			// TODO : �ִϸ��̼� �����
			mGroggyCount = 0.f;
			mFSM = eMonsterStateType::IDLE;
		}
		break;
	case eMonsterStateType::DIE:
		Display::Console::Log("DIE");
		if (m_pAnimator)
		{
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
	// ����
	if (_destination->gameObject->GetTag() == L"Player")
	{
		auto* player = _destination->GetOwner()->GetComponent<PlayerScript>();
		if (player)
		{
			// TODO :
			// ���Ͱ� ���� �ִϸ��̼� ���� ����
			// �÷��̾� ü�� ���
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
	// �÷��̾ �Ӹ��� ����� ��
	if (_destination->gameObject->GetTag() == L"Player")
	{
		// ���� �������·� �ٲ��ֱ�
		mFSM = eMonsterStateType::GROGGY;
	}
}

void MonsterScript::OnTriggerStay(Collider* _origin, Collider* _destination)
{
	
}

void MonsterScript::OnTriggerExit(Collider* _origin, Collider* _destination)
{

}
