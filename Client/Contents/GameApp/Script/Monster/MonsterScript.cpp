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
			// TODO : 무브 애니메이션 재생
		}

		// 타겟이 존재하는가?
		if (m_pTarget)
		{
			Vector3 dir = m_pTarget->transform->position - gameObject->transform->position;
			float distance = dir.Length();	// 거리 구하기
			dir.Normalize();  // 방향 구하기
			if (distance > mAttackDistance.val)
			{	// 타겟과의 거리가 범위 밖이라면 이동
				gameObject->transform->position += dir * mMoveSpeed.val;
			}
			else
			{
				// 타겟과의 거리가 범위 내라면 공격
				mFSM = eMonsterStateType::ATTACK;
			}
		}
		else // 타겟이 존재하지 않는가?
		{
			// 랜덤 방향 설정
			float x = Random::Range(-100.f, 100.f);  // x 랜덤
			float z = Random::Range(-100.f, 100.f);  // z 랜덤
			Vector3 TargetPos = {x, gameObject->transform->position.y, z}; // y 값은 유지

			// 범위 이탈시 원래 위치로 이동
			if (!bIsScope)
				TargetPos = m_pScope->transform->position;

			Vector3 dir = TargetPos - gameObject->transform->position;
			float distance = dir.Length();	// 거리 구하기
			dir.Normalize();  // 방향 구하기
			// 랜덤 방향으로 이동
			gameObject->transform->position += dir * mMoveSpeed.val;
		}
		break;
	case eMonsterStateType::ATTACK:
		if (m_pAnimator)
		{
			Display::Console::Log("ATTACK");
			// TODO : 추격 애니메이션 재생
		}

		// 타겟이 존재하는가?
		if (m_pTarget)
		{
			Vector3 dir = m_pTarget->transform->position - gameObject->transform->position;
			float distance = dir.Length();	// 거리 구하기
			dir.Normalize();  // 방향 구하기

			// 타겟 위치로 빠르게 이동
			gameObject->transform->position += dir * mMoveSpeed.val * 2.f;
		}
		else // 타겟이 존재하지 않는가?
		{
			// MOVE 로 체인지
			mFSM = eMonsterStateType::MOVE;
		}
		break;
	case eMonsterStateType::GROGGY:
		if (m_pAnimator)
		{
			Display::Console::Log("GROGGY");
			// TODO : 기절 애니메이션 재생
		}

		// GroggyCount 증가
		mGroggyCount += Time::GetUnScaledDeltaTime();

		// N초 안에 불이 붙었다면
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
		else // N초안에 불이 붙지 않았다면 
		{
			if (m_pAnimator)
			{
				Display::Console::Log("InverseGROGGY");
				// TODO : 애니메이션 역재생
			}

			mGroggyCount = 0.f;
			mFSM = eMonsterStateType::MOVE;
		}
		break;
	case eMonsterStateType::DIE:
		if (m_pAnimator)
		{
			Display::Console::Log("DIE");
			// TODO : 죽는 애니메이션 재생 후
			// 오브젝트 삭제
		}
		break;
	default:
		break;
	}
}

void MonsterScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
	// 몸통이랑 플레이어가 부딪혔을 때
	if (_destination->gameObject->GetTag() == L"Player")
	{
		auto* player = _destination->GetOwner()->GetComponent<PlayerScript>();
		if (player && mFSM == eMonsterStateType::ATTACK)
		{
			// 3만큼 데미지 입히기
			// player->Hit(3.f);
		}

		// 공격이 끝나면 다시 무브 상태로
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
