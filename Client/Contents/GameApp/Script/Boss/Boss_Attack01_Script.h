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
	SerializeField(FLOAT, mRazerScale, 1.0f);	// ������ ���� ũ��(����Ʈ, �ݶ��̴�) ���
	SerializeField(FLOAT, mRazerTime, 3.0f);	// ������ ���� �� �ð�
	SerializeField(FLOAT, mRazerRotate, 3.0f);	// ������ ���� ȸ�� �ݰ�(����? ��׸�?)
	FLOAT mRazerElapsedTime = 0.0f;
};

