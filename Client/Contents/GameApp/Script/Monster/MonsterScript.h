#pragma once

enum class eMonsterStateType
{
	IDLE,		// ��� ��
	MOVE,		// �����̴� ��
	ATTACK,		// ���� ��
	HIT,		// �´� ��
	GROGGY,		// ����
	DIE			// ����
};

class BurnObjectScript;

class MonsterScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(MonsterScript);
public:
	void Start();
	void Update();
public:
	virtual void OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination) override;
	virtual void OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination)  override;
	virtual void OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)  override;
public:
	virtual void OnTriggerEnter(Collider* _origin, Collider* _destination)	override;
	virtual void OnTriggerStay(Collider* _origin, Collider* _destination)	override;
	virtual void OnTriggerExit(Collider* _origin, Collider* _destination)	override;
public:
	void SetTarget(Object* _target)		{ m_pTarget = _target; }
	void SetDirectPos(Vector3 _pos)		{ mRandomPos = _pos; }
	void SetSpeed(float _speed)			{ mMoveSpeed.val = _speed; }
	void SetDistance(float _distance)	{ mAttackDistance.val = _distance; }
	void SetGroggyTick(float _tick)		{ mGroggyTick.val = _tick; }
public:
	Object* GetTarget()		{ return m_pTarget; }
	Vector3 GetDirectPos()	{ return mRandomPos; }
	float	GetSpeed()		{ return mMoveSpeed.val; }
	float	GetDistance()	{ return mAttackDistance.val; }
	float	GetGroggyTick() { return mGroggyTick.val; }
private:
	// Monster ����
	eMonsterStateType mFSM = eMonsterStateType::IDLE;
	// Monster�� ������ ���� ������Ʈ
	Animator* m_pAnimator = nullptr;
	BoxCollider* m_pHeadCollider = nullptr;
	SphereCollider* m_pBodyCollider = nullptr;
	BurnObjectScript* m_pBurnObjectScript = nullptr;
private:
	// Target
	Object* m_pTarget;
	// ���� ��ġ��!
	Vector3 mRandomPos;
	// ���� Ÿ�̸�
	float mGroggyCount = 0.0f;

	SerializeField(FLOAT, mGroggyTick, 5.f);		// ���� tick
	SerializeField(FLOAT, mMoveSpeed, 10.f);		// �̵� �ӵ�
	SerializeField(FLOAT, mAttackDistance, 10.f);	// ���� ����
};

