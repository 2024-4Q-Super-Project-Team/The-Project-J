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
	// Monster_A 시스템 설명
	// 1. 감지 범위 내에 플레이어 존재하는지 체크
	//		ㄴ존재 하지 않는다면 방향 랜덤
	//		ㄴ존재 한다면 타겟 위치로
	//			ㄴ타격 범위 내 플레이어 존재 체크 (존재한다면 플레이어에게 추격 공격)
	// 
	//  --<추격 공격 애니메이션 중>--
	//		 2. 플레이어와 충돌했는지 체크
	//			ㄴ머리 콜라이더 충돌 체크 (충돌이면 groggy)
	//			ㄴ몸통 콜라이더 충돌 체크 (충돌이면 플레이어 체력 감소)
	//			ㄴ충돌하지 않았다면 금정거 후 다시 범위 체크
	//		
	//		--<Groggy 상태 중>--
	//			3. N초 이내에 불이 붙었는가? (상호작용 유무 확인)
	//				ㄴ붙었다면 DIE
	//				ㄴ안붙었다면 다시 IDLE
	//
	// 4. 몬스터가 범위 밖으로 나가지 않도록 주의가 필요함

	switch (mFSM)
	{
	case eMonsterStateType::IDLE:
		//Display::Console::Log("IDLE");
		if (m_pAnimator)
		{
			// TODO : 아이들 애니메이션 재생
		}
		break;
	case eMonsterStateType::MOVE:
		Display::Console::Log("MOVE");
		if (m_pAnimator)
		{
			// TODO : 무브 애니메이션 재생
			m_pAnimator->Play();
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
		} // 타겟이 존재하지 않는가?
		else
		{
			Vector3 dir = mRandomPos - gameObject->transform->position;
			float distance = dir.Length();	// 거리 구하기
			dir.Normalize();  // 방향 구하기
			// 랜덤 방향으로 이동
			gameObject->transform->position += dir * mMoveSpeed.val;
		}
		break;
	case eMonsterStateType::ATTACK:
		Display::Console::Log("ATTACK");
		if (m_pAnimator)
		{
			// TODO : 추격 애니메이션 재생
		}

		if (m_pTarget)
		{
			// 방향 벡터 구하기
			Vector3 dir = m_pTarget->transform->position - gameObject->transform->position;
			float distance = dir.Length();	// 거리 구하기
			dir.Normalize();  // 방향 구하기

			// 타겟 위치로 빠르게 이동
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
			// TODO : 기절 애니메이션 재생
		}

		mGroggyCount += Time::GetUnScaledDeltaTime();

		if (mGroggyCount < mGroggyTick.val) // && m_pBurnObjectScript->IsBurning()
		{	// N초 안에 불이 붙었다면
			mGroggyCount = 0.f;
			mFSM = eMonsterStateType::DIE;
		}
		else
		{	// N초안에 불이 붙지 않았다면 
			// TODO : 애니메이션 역재생
			mGroggyCount = 0.f;
			mFSM = eMonsterStateType::IDLE;
		}
		break;
	case eMonsterStateType::DIE:
		Display::Console::Log("DIE");
		if (m_pAnimator)
		{
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
	// 몸통
	if (_destination->gameObject->GetTag() == L"Player")
	{
		auto* player = _destination->GetOwner()->GetComponent<PlayerScript>();
		if (player)
		{
			// TODO :
			// 몬스터가 공격 애니메이션 중일 때에
			// 플레이어 체력 깎기
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
	// 플레이어가 머리에 닿았을 때
	if (_destination->gameObject->GetTag() == L"Player")
	{
		// 몬스터 기절상태로 바꿔주기
		mFSM = eMonsterStateType::GROGGY;
	}
}

void MonsterScript::OnTriggerStay(Collider* _origin, Collider* _destination)
{
	
}

void MonsterScript::OnTriggerExit(Collider* _origin, Collider* _destination)
{

}
