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
	// 타겟이 존재하는가?
	if (m_pTarget)
	{
		mFSM = eMonsterStateType::FAST_WALK;
	}
	else // 타겟이 존재하지 않는가?
	{
		Vector3 TargetPos{};

		if (bIsScope)
		{
			mResetCount += Time::GetUnScaledDeltaTime();

			float x = 0.f;
			float z = 0.f;

			if (mResetCount > 5.f)
			{
				x = Random::Range(-240, 240);  // x 랜덤
				z = Random::Range(-240, 240);  // z 랜덤
			}
			// 랜덤 방향 설정
			TargetPos = { x, gameObject->transform->position.y, z }; // y 값은 유지

			mResetCount = 0;
		}	// 범위 이탈시 원래 위치로 이동
		else
		{
			TargetPos = m_pScope->transform->position;
		}

		Vector3 dir = TargetPos - gameObject->transform->position;
		Vector3 targetDir = { dir.x, gameObject->transform->position.y, dir.z };
		float distance = targetDir.Length();	// 거리 구하기
		targetDir.Normalize();  // 방향 구하기

		// 랜덤 방향으로 이동
		if (gameObject->transform->position.x != TargetPos.x || gameObject->transform->position.z != TargetPos.z)
			gameObject->transform->position += targetDir * mMoveSpeed.val * Time::GetUnScaledDeltaTime();
		else
			mFSM = eMonsterStateType::IDLE;
	}
}

void MonsterScript::UpdateFastWalk()
{
	// 타겟이 존재하는가?
	if (m_pTarget)
	{
		Vector3 dir = m_pTarget->transform->position - gameObject->transform->position;
		Vector3 targetDir = { dir.x, gameObject->transform->position.y, dir.z };
		float distance = targetDir.Length();	// 거리 구하기
		targetDir.Normalize();  // 방향 구하기

		if (distance > mAttackDistance.val)
		{
			gameObject->transform->position += targetDir * mMoveSpeed.val * 1.5 * Time::GetUnScaledDeltaTime();
		}
		else // 타겟과의 거리가 범위 내라면 공격
		{
			mFSM = eMonsterStateType::RUN;
		}
	}
	else // 타겟이 존재하지 않는가?
	{
		mFSM = eMonsterStateType::WALK;
	}
}

void MonsterScript::UpdateRun()
{
	// 타겟이 존재하는가?
	if (m_pTarget)
	{
		Vector3 dir = m_pTarget->transform->position - gameObject->transform->position;
		Vector3 targetDir = { dir.x, gameObject->transform->position.y, dir.z };
		float distance = targetDir.Length();	// 거리 구하기
		targetDir.Normalize();  // 방향 구하기

		if (distance > mAttackDistance.val)
			mFSM = eMonsterStateType::FAST_WALK;

		// 타겟 위치로 빠르게 이동
		gameObject->transform->position += targetDir * mMoveSpeed.val * 2.f * Time::GetUnScaledDeltaTime();
	}
	else // 타겟이 존재하지 않는가?
	{
		// MOVE 로 체인지
		mFSM = eMonsterStateType::WALK;
	}
}

void MonsterScript::UpdateHit()
{
	// GroggyCount 증가
	mGroggyCount += Time::GetUnScaledDeltaTime();
	m_pAnimator->SetLoop(false);

	if (mGroggyCount > mGroggyTick.val)
	{
		// N초 안에 불이 붙었다면
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
	else // N초안에 불이 붙지 않았다면 
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
			// 애니메이션이 끝나면 몇 초뒤 옵젝 비활성화
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
	// 몸통이랑 플레이어가 부딪혔을 때
	if (_destination->gameObject->GetTag() == L"Player")
	{
		auto* player = _destination->GetOwner()->GetComponent<PlayerScript>();
		if (player && mFSM == eMonsterStateType::RUN)
		{
			// 3만큼 데미지 입히기
			// player->Hit(3.f);
		}

		// 공격이 끝나면 다시 처음 상태로
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