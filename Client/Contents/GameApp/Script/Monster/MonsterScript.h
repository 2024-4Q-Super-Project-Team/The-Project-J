#pragma once

enum class eMonsterStateType
{
	IDLE,		// ���
	WALK,		// �����̴� ��
	FAST_WALK,	// �� �� ���� �����̴� ��
	ROTATE,		// ȸ��
	RUN,		// ����
	HIT,		// �ǰ�
	GROGGY,		// ����
	DEAD		// ���� -> ����Ʈ �ִϸ��̼�
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

	AudioSource* m_pAudioSource = nullptr;
private:
	// Target
	Object* m_pTarget = nullptr;
	Object* m_pWeakness = nullptr;

	float mIdleCount = 0.0f;	// ���̵� ��� Ÿ�̸�
	float mResetCount = 0.0f;	// ���� ���� Ÿ�̸�
	float mGroggyCount = 0.0f;	// �׷α� Ÿ�̸�
	float mRange = 0;			// �̵�����
	float mDistance = 0.00f;	// �Ÿ�
	bool bIsScope = true;		// ���� ������ �������� üũ
	bool bTargetIsScope = true;		// ���� ������ �������� üũ

	Vector3 mTargetPos{};	// �⺻ ���� ������ ��
	Vector3 mTargetDir{};	// ���� ����
public:
	SerializeField(FLOAT, mGroggyTick, 10.f);				// ���� tick
	SerializeField(FLOAT, mMoveSpeed, 40.f);				// �̵� �ӵ�
	SerializeField(FLOAT, mAttackDistance, 100.f);			// ���� ����
	SerializeField(FLOAT, mDamage, 1);						// ���� ������
	SerializeField(INT, mType, 0);	// ���� Ÿ��
};

