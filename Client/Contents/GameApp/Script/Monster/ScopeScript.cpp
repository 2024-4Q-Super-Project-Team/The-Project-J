#include "pch.h"
#include "ScopeScript.h"

#include "Contents/GameApp/Script/Monster/MonsterScript.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"

void ScopeScript::Start()
{
	// Add componenet
	
	{	// Collider Component
		m_pCollider = gameObject->GetComponent<SphereCollider>();
		if (m_pCollider == nullptr)
		{
			m_pCollider = gameObject->AddComponent<SphereCollider>();
		}
	}
}

void ScopeScript::Update()
{

}

void ScopeScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
	if (_destination->gameObject->GetTag() == L"Player")
	{	// �÷��̾� ���� �� ���� �� üũ
		bIsPlayer = true;
	}
}

void ScopeScript::OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination)
{
	if (_destination->gameObject->GetTag() == L"Monster")
	{	// �÷��̾� ���� �� ���� ���� ����
		auto* monster = _destination->GetOwner()->GetComponent<MonsterScript>();
		if(monster)
			monster->bIsTarget = bIsPlayer;
	}
}

void ScopeScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
	if (_destination->gameObject->GetTag() == L"Player")
	{	// �÷��̾� ���� �� ���� �� üũ
		bIsPlayer = false;
	}

	if (_destination->gameObject->GetTag() == L"Monster")
	{	// �ݰ� �� ���� ���Ͱ� ���� �� �ݰ� ���ͷ� �̵�
		auto* monster = _destination->GetOwner()->GetComponent<MonsterScript>();
		if (monster)
			monster->mTarget = gameObject->transform->position;
	}
}
