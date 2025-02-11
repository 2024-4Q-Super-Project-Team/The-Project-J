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
	void SetRootObject(Object* _root);
	void SetRootView(Vector3 _viewDir);
	void SetRootAngleY(FLOAT _yAngle);
	inline BOOL IsAttack() { return isAttack; }
private:
	Object* mRootObject;	// ���� ������Ʈ
	Vector3 mRootViewDir;
	FLOAT	mRootAngleY;

	Rigidbody*		mRigidBody;
	BoxCollider*	mHitColiider;
	Animator*		mAnimator;
	BoneAttacher*	mRazerBoneAttacher = nullptr;

	BOOL isAttack = FALSE;
	SerializeField(INT, mRazerDamage, 1);		// ������ ���� ������
	SerializeField(FLOAT, mRazerDist, 500.0f);	// ������ ���� �Ÿ�
	SerializeField(FLOAT, mRazerScale, 1.0f);	// ������ ���� ũ��(����Ʈ, �ݶ��̴�) ���
	SerializeField(FLOAT, mRazerTime, 3.0f);	// ������ ���� �� �ð�
	SerializeField(FLOAT, mRazerRotate, 3.0f);	// ������ ���� ȸ�� �ݰ�(����? ��׸�?)
	FLOAT mRazerElapsedTime = 0.0f;

	
};

