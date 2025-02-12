#pragma once

enum class eMonsterStateType
{
	IDLE,		// 대기
	WALK,		// 움직이는 중
	FAST_WALK,	// 좀 더 빨리 움직이는 중
	ROTATE,		// 회전
	RUN,		// 공격
	HIT,		// 피격
	GROGGY,		// 기절
	DEAD		// 죽음 -> 이펙트 애니메이션
};

enum class eMonsterType
{
	A,
	B,
	SIZE
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
	void UpdateA();
	void UpdateB();
private:
	void UpdateIdle();
	void UpdateRotate();
	void UpdateWalk();
	void UpdateFastWalk();
	void UpdateRun();
	void UpdateHit();
	void UpdateGroggy();
	void UpdateDead();
	void UpdateMonsterAnim_A();
	void UpdateMonsterAnim_B();
	void UpdateMonsterAngle();
public:
	virtual void OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination) override;
	virtual void OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination)  override;
	virtual void OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)  override;
public:
	virtual void OnTriggerEnter(Collider* _origin, Collider* _destination)	override;
	virtual void OnTriggerStay(Collider* _origin, Collider* _destination)	override;
	virtual void OnTriggerExit(Collider* _origin, Collider* _destination)	override;
public:
	void SetSFX(const std::wstring& _filename);
	void InitSFX();

	void SetTarget(Object* _target)		{ m_pTarget = _target; }
	void SetIsScope(bool _isScope)		{ bIsScope = _isScope; }
	void SetSpeed(float _speed)			{ mMoveSpeed.val = _speed; }
	void SetDistance(float _distance)	{ mAttackDistance.val = _distance; }
	void SetGroggyTick(float _tick)		{ mGroggyTick.val = _tick; }
	void SetDamage(int _damage)			{ mDamage.val = _damage; }
	void SetRange(int _range)			{ mRange = _range; }
public:
	Object* GetTarget()		{ return m_pTarget; }
	bool	IsScope()		{ return bIsScope; }
	float	GetSpeed()		{ return mMoveSpeed.val; }
	float	GetDistance()	{ return mAttackDistance.val; }
	float	GetGroggyTick() { return mGroggyTick.val; }
	int		GetDamage()		{ return mDamage.val; }
	int		GetRange()		{ return mRange; }
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

	AudioSource* m_pAudioSource = nullptr;
private:
	// Target
	Object* m_pTarget = nullptr;
	Object* m_pWeakness = nullptr;

	float mIdleCount = 0.0f;	// 아이들 대기 타이머
	float mResetCount = 0.0f;	// 랜덤 리셋 타이머
	float mGroggyCount = 0.0f;	// 그로기 타이머
	float mRange = 0;			// 이동범위
	float mDistance = 0.00f;	// 거리
	bool bIsScope = true;		// 범위 밖으로 나갔는지 체크
	bool bTargetIsScope = true;		// 범위 밖으로 나갔는지 체크

	Vector3 mTargetPos{};	// 기본 랜덤 포지션 값
	Vector3 mTargetDir{};	// 랜덤 벡터
public:
	SerializeField(FLOAT, mGroggyTick, 10.f);				// 기절 tick
	SerializeField(FLOAT, mMoveSpeed, 40.f);				// 이동 속도
	SerializeField(FLOAT, mAttackDistance, 100.f);			// 공격 범위
	SerializeField(FLOAT, mDamage, 1);						// 공격 데미지
	SerializeField(INT, mType, 0);	// 몬스터 타입
};

