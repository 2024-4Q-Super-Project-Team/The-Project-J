#include "pch.h"
#include "ScopeScript.h"

#include "Contents/GameApp/Script/Monster/MonsterScript.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"

void ScopeScript::Start()
{
	gameObject->SetTag(L"Scope");

	// Add componenet
	
	{	// RigidBody Component
		m_pRigidBody = gameObject->AddComponent<Rigidbody>();
	}

	{	// Collider Component
		m_pCollider = gameObject->AddComponent<BoxCollider>();
		m_pCollider->SetPosition();
	}
}

void ScopeScript::Update()
{

}

void ScopeScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
	if (_destination->gameObject->GetTag() == L"Player")
	{	// 플레이어 범위 내 있을 시 체크
		m_pPlayer = _destination->gameObject;
	}
}

void ScopeScript::OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination)
{
	if (_destination->gameObject->GetTag() == L"Monster")
	{	// 플레이어 범위 내 존재 여부 전달
		auto* monster = _destination->GetOwner()->GetComponent<MonsterScript>();
		if (monster)
		{
			monster->SetTarget(m_pPlayer);
		}
	}
}

void ScopeScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
	if (_destination->gameObject->GetTag() == L"Player")
	{	// 플레이어 범위 내 없을 시 체크
		m_pPlayer = nullptr;
	}

	if (_destination->gameObject->GetTag() == L"Monster")
	{	// 반경 내 에서 몬스터가 나갈 시 반경 센터로 이동
		auto* monster = _destination->GetOwner()->GetComponent<MonsterScript>();
		if (monster)
			monster->SetDirectPos(gameObject->transform->position);
	}
}
