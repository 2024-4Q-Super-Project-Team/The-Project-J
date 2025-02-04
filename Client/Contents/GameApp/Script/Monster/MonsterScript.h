#pragma once

enum class eMonsterStateType
{
	IDLE,		// 대기 중
	MOVE,		// 움직이는 중
	ATTACK,		// 공격 중
	HIT,		// 맞는 중
	GROGGY,		// 기절
	DIE			// 죽음
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
	// Monster 상태
	eMonsterStateType mFSM = eMonsterStateType::IDLE;
	// Monster가 가지고 있을 컴포넌트
	Animator* m_pAnimator = nullptr;
	BoxCollider* m_pHeadCollider = nullptr;
	SphereCollider* m_pBodyCollider = nullptr;
	BurnObjectScript* m_pBurnObjectScript = nullptr;
private:
	// Target
	Object* m_pTarget;
	// 원래 위치로!
	Vector3 mRandomPos;
	// 기절 타이머
	float mGroggyCount = 0.0f;

	SerializeField(FLOAT, mGroggyTick, 5.f);		// 기절 tick
	SerializeField(FLOAT, mMoveSpeed, 10.f);		// 이동 속도
	SerializeField(FLOAT, mAttackDistance, 10.f);	// 공격 범위
};

