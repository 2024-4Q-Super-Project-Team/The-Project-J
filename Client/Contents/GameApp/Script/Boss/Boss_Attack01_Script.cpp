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
		// ������ start, end�� ������?
		// �׷� �ݶ��̴��� ��� ó����?
		
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
