#pragma once

enum class eMonsterStateType
{
	IDLE,		// ��� ��
	WALK,		// �����̴� ��
	FAST_WALK,	// �� �� ���� �����̴� ��
	RUN,		// ���� ��
	HIT,		// ����
	DEAD		// ����
};

class BurnObjectScript;

class MonsterScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(MonsterScript);
public:
	void Start();
	void Update();
private:
	void UpdateIdle();
	void UpdateWalk();
	void UpdateFastWalk();
	void UpdateRun();
	void UpdateHit();
	void UpdateDead();
	void UpdateMonsterAnim();
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
	void SetIsScope(bool _isScope)		{ bIsScope = _isScope; }
	void SetSpeed(float _speed)			{ mMoveSpeed.val = _speed; }
	void SetDistance(float _distance)	{ mAttackDistance.val = _distance; }
	void SetGroggyTick(float _tick)		{ mGroggyTick.val = _tick; }
public:
	Object* GetTarget()		{ return m_pTarget; }
	bool	IsScope()		{ return bIsScope; }
	float	GetSpeed()		{ return mMoveSpeed.val; }
	float	GetDistance()	{ return mAttackDistance.val; }
	float	GetGroggyTick() { return mGroggyTick.val; }
private:
	// Monster ����
	eMonsterStateType mFSM = eMonsterStateType::IDLE;
	// Monster�� ������ ���� ������Ʈ
	Object* m_pScope = nullptr;
	// Monster�� ������ ���� ������Ʈ
	Animator* m_pAnimator = nullptr;
	Rigidbody* m_pRigidBody = nullptr;
	BoxCollider* m_pHeadCollider = nullptr;
	BoxCollider* m_pBodyCollider = nullptr;
	BurnObjectScript* m_pBurnObjectScript = nullptr;
private:
	// Target
	Object* m_pTarget;
	// ���� Ÿ�̸�
	float mResetCount = 0.0f;
	float mGroggyCount = 0.0f;
	// ���� ������ �������� üũ
	bool bIsScope = true;
	SerializeField(FLOAT, mGroggyTick, 10.f);		// ���� tick
	SerializeField(FLOAT, mMoveSpeed, 40.f);		// �̵� �ӵ�
	SerializeField(FLOAT, mAttackDistance, 100.f);	// ���� ����
public:
	//virtual json Serialize() override;
	//virtual void Deserialize(json& j) override;
};

