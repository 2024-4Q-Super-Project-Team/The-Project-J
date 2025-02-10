#pragma once

class Boss_Attack01_Script : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(Boss_Attack01_Script);
public:
	virtual void Start() override;
	virtual void Update() override;
	virtual void _CALLBACK OnTriggerStay(Collider* _origin, Collider* _destination) override;
public:
	void SetAttackStart();
	void SetAttackEnd();
	inline BOOL IsAttack() { return isAttack; }
private:
	Object* mAttachBone;

	Rigidbody*		mRigidBody;
	BoxCollider*	mHitColiider;
	Animator*		mAnimator;
	BoneAttacher*	mRazerBoneAttacher = nullptr;

	BOOL isAttack = FALSE;
	SerializeField(INT, mRazerDamage, 1);		// 패턴의 광선 데미지
	SerializeField(FLOAT, mRazerScale, 1.0f);	// 패턴의 광선 크기(이펙트, 콜라이더) 계수
	SerializeField(FLOAT, mRazerTime, 3.0f);	// 패턴의 광선 총 시간
	SerializeField(FLOAT, mRazerRotate, 3.0f);	// 패턴의 광선 회전 반경(라디안? 디그리?)
	FLOAT mRazerElapsedTime = 0.0f;
};

