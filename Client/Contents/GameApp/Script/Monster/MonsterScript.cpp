#include "pch.h"
#include "MonsterScript.h"

#include "Contents/GameApp/Script/Player/PlayerScript.h"

void MonsterScript::Start()
{
	gameObject->SetTag(L"Monster");

	// Init Setting
	mFSM = eMonsterStateType::IDLE;
	bIsTarget = false;
	bIsBurn = false;

	// Add componenet
	
	{	// Animator Component
		m_pAnimator = gameObject->GetComponent<Animator>();
		if (m_pAnimator == nullptr)
			m_pAnimator = gameObject->AddComponent<Animator>();
	}

	{	// Head Collider Component
		m_pHeadCollider = gameObject->GetComponent<BoxCollider>();
		if (m_pHeadCollider == nullptr)
			m_pHeadCollider = gameObject->AddComponent<BoxCollider>();
	}

	{	// Foot Collider Component
		m_pFootCollider = gameObject->GetComponent<BoxCollider>();
		if (m_pFootCollider == nullptr)
			m_pFootCollider = gameObject->AddComponent<BoxCollider>();
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
		Display::Console::Log("IDLE");
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
		}

		if (bIsTarget)
		{
			// TODO : Ÿ�� ��ġ�� õõ�� �̵��ϱ�
			//gameObject->transform->position
			// Ÿ�ٰ��� �Ÿ��� N distance ����� ����
			mFSM = eMonsterStateType::ATTACK;
		}
		else
		{
			// TODO : ���� �������� �̵�
		}
		break;
	case eMonsterStateType::ATTACK:
		Display::Console::Log("ATTACK");
		if (m_pAnimator)
		{
			// TODO : �߰� �ִϸ��̼� ���
		}

		if (bIsTarget)
		{
			// TODO : Ÿ�� ��ġ�� ������ �̵�
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

		// ���� �ϴ� �� ���� �پ��ٸ�
		if (bIsBurn)
		{
			mFSM = eMonsterStateType::DIE;
		}
		else  // N�ʾȿ� ���� �ʾҴٸ� �ִϸ��̼� ����� ��
		{
			mFSM = eMonsterStateType::IDLE;
		}
		break;
	case eMonsterStateType::DIE:
		Display::Console::Log("DIE");
		if (m_pAnimator)
		{
			// TODO : 
			// �״� �ִϸ��̼� ��� ��
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
		if (player && m_pAnimator)
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
		auto* player = _destination->gameObject->GetComponent<PlayerScript>();
		// TODO : 
		// �÷��̾�� �Һ��̱� ���°� �����ϵ��� ���ֱ�

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
