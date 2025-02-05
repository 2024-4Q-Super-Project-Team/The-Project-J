#include "pch.h"
#include "ScopeScript.h"

#include "Contents/GameApp/Script/Monster/MonsterScript.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"

void ScopeScript::Start()
{
	gameObject->SetTag(L"Scope");

	// Add componenet
	
	{	// RigidBody Component
		m_pRigidBody = gameObject->GetComponent<Rigidbody>();
		if (!m_pRigidBody)
			m_pRigidBody = gameObject->AddComponent<Rigidbody>();
	}
	{	// Collider Component
		m_pCollider = gameObject->GetComponent<BoxCollider>();
		if (!m_pCollider)
			m_pCollider = gameObject->AddComponent<BoxCollider>();

		m_pCollider->SetIsTrigger(true);
		m_pCollider->SetExtents(Vector3{ 100, 100, 100 });
	}

	// Init Setting
	{
		if (m_pMonster)
		{
			auto* monster = m_pMonster->GetComponent<MonsterScript>();

			if (monster)
				monster->SetTarget(m_pPlayer);
		}
	}
}

void ScopeScript::Update()
{

}

void ScopeScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
	
}

void ScopeScript::OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination)
{
	if (_destination->gameObject->GetTag() == L"Player")
	{	// �÷��̾� ���� �� ���� �� üũ
		m_pPlayer = _destination->gameObject;
	}
}

void ScopeScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
	
}

void ScopeScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{
	if (_destination->gameObject->GetTag() == L"Player")
	{	// �÷��̾� ���� �� ���� �� üũ
		m_pPlayer = _destination->gameObject;
	}
}

void ScopeScript::OnTriggerStay(Collider* _origin, Collider* _destination)
{

}

void ScopeScript::OnTriggerExit(Collider* _origin, Collider* _destination)
{
	if (_destination->gameObject->GetTag() == L"Player")
	{	// �÷��̾� ���� �� ���� �� üũ
		m_pPlayer = nullptr;
	}

	if (_destination->gameObject->GetTag() == L"Monster")
	{	// �ݰ� �� ���� ���Ͱ� ���� �� ���� ��Ż ����
		auto* monster = _destination->GetOwner()->GetComponent<MonsterScript>();
		if (monster)
			monster->SetIsScope(false);
	}
}
