#pragma once

class Boss_Attack01_Script : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(Boss_Attack01_Script);
public:
	void Start();
	void Update();
public:
	void InitCollider();
public:
	void SetAttackStart();
	void SetAttackEnd();
private:
	BoxCollider* mHitColiider;
	Animator* mAnimator;
};

