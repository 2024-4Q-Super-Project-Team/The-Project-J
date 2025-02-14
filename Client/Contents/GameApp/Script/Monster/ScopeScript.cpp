#include "pch.h"
#include "ScopeScript.h"

#include "Contents/GameApp/Script/Monster/MonsterScript.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"

void ScopeScript::Start()
{
	// Add componenet
	{	// RigidBody Component
		m_pRigidBody = gameObject->GetComponent<Rigidbody>();
		if (!m_pRigidBody)
			m_pRigidBody = gameObject->AddComponent<Rigidbody>();
	}
	{	// Collider Component
		m_pCollider = gameObject->GetComponent<SphereCollider>();
		if (!m_pCollider)
			m_pCollider = gameObject->AddComponent<SphereCollider>();

		m_pCollider->SetRadius(165.f);
		m_pCollider->SetIsTrigger(true);
	}
}

void ScopeScript::Update()
{
	if (m_pMonster)
	{
		auto* monster = m_pMonster->GetComponent<MonsterScript>();

		if (monster)
		{
			monster->SetTarget(m_pPlayer);
			monster->SetRange(m_pCollider->GetRadius() / 2);
		}
	}
}

void ScopeScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
	
}

void ScopeScript::OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination)
{

}

void ScopeScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{

}

void ScopeScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{
	if (_destination->gameObject->GetTag() == L"Player")
	{	// 플레이어 범위 내 있을 시 체크
		if (m_pPlayer)
			return;

		m_pPlayer = _destination->gameObject;
	}
}

void ScopeScript::OnTriggerStay(Collider* _origin, Collider* _destination)
{

}

void ScopeScript::OnTriggerExit(Collider* _origin, Collider* _destination)
{
	if (_destination->gameObject->GetTag() == L"Player")
	{	// 플레이어 범위 내 없을 시 체크
		m_pPlayer = nullptr;
	}
}
