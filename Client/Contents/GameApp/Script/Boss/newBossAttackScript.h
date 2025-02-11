#pragma once

class newBossScript;

class newBossAttackScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(newBossAttackScript);
public:
	void Start();
	void Update();
	void _CALLBACK OnTriggerStay(Collider* _origin, Collider* _destination);
public:
	void SetAttackStart(Vector3 _pos);
	void SetAttackEnd();
	inline void SetRootObject(newBossScript* _root) { mRootObject = _root; }
	inline bool IsAttack() { return isAttack; }
private:
	newBossScript* mRootObject;	// ���� ������Ʈ

	Rigidbody* mRigidBody;
	BoxCollider* mHitColiider;
	Animator* mAnimator;

	SerializeField(INT,   mRazerDamage, 30);		// ������ ���� ������
	SerializeField(FLOAT, mRazerDist, 800.0f);		// ������ ���� �Ÿ�
	SerializeField(FLOAT, mRazerRange, 700.0f);		// ������ ���� �ݰ�
	SerializeField(FLOAT, mRazerScale, 1.0f);		// ������ ���� ũ��(����Ʈ, �ݶ��̴�) ���
	SerializeField(FLOAT, mRazerTime, 3.0f);		// ������ ���� �� �ð�

	bool	isAttack;
	Vector3 mOriginScale = Vector3(200.0f, 3000.0f, 200.0f);
	Vector3 mSpawnPosition = Vector3::Zero;
	Vector3 mColliderExternt = Vector3::One;
	FLOAT	mRazerElapsedTime = 0.0f;

public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;
};