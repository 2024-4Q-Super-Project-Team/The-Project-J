#include "pch.h"
#include "Boss_Attack01_Script.h"

#define BOSS_ATTACK_ANIM_START L"001"
#define BOSS_ATTACK_ANIM_RUNNING L"003"
#define BOSS_ATTACK_ANIM_END L"004"

void Boss_Attack01_Script::Start()
{
	mAnimator = gameObject->GetComponent<Animator>();
	mHitColiider = gameObject->GetComponent<BoxCollider>();
}

void Boss_Attack01_Script::Update()
{
	if (mAnimator->GetActiveAnimationKey() == BOSS_ATTACK_ANIM_START)
	{
		// 레이저 start, end가 왜있지?
		// 그럼 콜라이더는 어떻게 처리함?
		
	}
}

void Boss_Attack01_Script::InitCollider()
{
	mHitColiider->SetExtents(Vector3::One);
}

void Boss_Attack01_Script::SetAttackStart()
{
	mAnimator->SetCurrentAnimation(BOSS_ATTACK_ANIM_START);
}

void Boss_Attack01_Script::SetAttackEnd()
{
	mAnimator->SetCurrentAnimation(BOSS_ATTACK_ANIM_END);
}
