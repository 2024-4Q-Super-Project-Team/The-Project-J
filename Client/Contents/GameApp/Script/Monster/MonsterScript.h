#pragma once

enum class eMonsterStateType
{
	IDLE,		// 대기 중
	WALK,		// 움직이는 중
	FAST_WALK,	// 좀 더 빨리 움직이는 중
	RUN,		// 공격 중
	HIT,		// 기절
	DEAD		// 죽음
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
	// Monster 상태
	eMonsterStateType mFSM = eMonsterStateType::IDLE;
	// Monster가 가지고 있을 오브젝트
	Object* m_pScope = nullptr;
	// Monster가 가지고 있을 컴포넌트
	Animator* m_pAnimator = nullptr;
	Rigidbody* m_pRigidBody = nullptr;
	BoxCollider* m_pHeadCollider = nullptr;
	BoxCollider* m_pBodyCollider = nullptr;
	BurnObjectScript* m_pBurnObjectScript = nullptr;
private:
	// Target
	Object* m_pTarget;
	// 기절 타이머
	float mResetCount = 0.0f;
	float mGroggyCount = 0.0f;
	// 범위 밖으로 나갔는지 체크
	bool bIsScope = true;
	SerializeField(FLOAT, mGroggyTick, 10.f);		// 기절 tick
	SerializeField(FLOAT, mMoveSpeed, 40.f);		// 이동 속도
	SerializeField(FLOAT, mAttackDistance, 100.f);	// 공격 범위
public:
	//virtual json Serialize() override;
	//virtual void Deserialize(json& j) override;
};

