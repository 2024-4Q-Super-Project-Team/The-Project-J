#pragma once

class Boss_Attack01_Script : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(Boss_Attack01_Script);
public:
	void Start();
	void Update();
public:
	void SetAttackStart();
	void SetAttackEnd();
	inline BOOL IsAttack() { return isAttack; }
private:
	Object* mAttachBone;

	BoxCollider*	mHitColiider;
	Animator*		mAnimator;
	BoneAttacher*	mRazerBoneAttacher = nullptr;

	BOOL isAttack = FALSE;
	SerializeField(FLOAT, mRazerScale, 1.0f);	// 패턴의 광선 크기(이펙트, 콜라이더) 계수
	SerializeField(FLOAT, mRazerTime, 3.0f);	// 패턴의 광선 총 시간
	SerializeField(FLOAT, mRazerRotate, 3.0f);	// 패턴의 광선 회전 반경(라디안? 디그리?)
	FLOAT mRazerElapsedTime = 0.0f;
};

