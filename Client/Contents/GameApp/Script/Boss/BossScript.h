#pragma once

enum class eBossStateType
{
	IDLE,
	ATTACK,
	HIT,
};

class Boss_Attack01_Script;

class BossScript : public MonoBehaviour
{
	enum eBossAttackType
	{
		NONE,		// ����
		ATTACK_01,	// ����
		ATTACK_02,	// ���� ��ȯ
	};
public:
	_MONOBEHAVIOUR_INIT(BossScript);
public:
	void Start();
	void Update();
private:
	inline void SetState(eBossStateType _stateType) { mBossState = _stateType; }

	void UpdateTransform(); // ���� ��ġ�� �����Ѵ�.
	void UpdateAnimation();	// ���� �ִϸ��̼��� �����Ѵ�.
	void UpdateState();		// ���� ���¿� ���� ������Ʈ
	// ===FSM===
	void UpdateIdle();
	void UpdateAttack();
	void UpdateAttack01();
	void UpdateAttack02();
	void UpdateHit();
private:
	Object* mBodyObject;
	Object* mHeadObject;
	Animator* mBodyAnimator;
	Animator* mHeadAnimator;

	Transform* mAxisTransform;							// ������ ���� ���� �� Ʈ������
	SerializeField(FLOAT, mDistanceFromAxis, 572.0f);	// ���� ����� �� ��ŭ ������������

	SerializeField(FLOAT, mMinIdleTick, 3.0f);			// ���� ������ ���� �ּ� ��� �ð�
	SerializeField(FLOAT, mMaxIdleTick, 5.0f);			// ���� ������ ���� �ִ� ��� �ð�
	FLOAT mCurIdleTime = 0.0f;							// Min~Max ���̿��� �������� ���� ���� ��(���� ���ݱ����� ��� �ð�)
	FLOAT mIdleTickCounter = 0.0f;						// ���� ���� ������ ���� �ð� ī����

	eBossStateType mBossState = eBossStateType::IDLE;	//
	BOOL isAttack = FALSE;								//
	eBossAttackType mCurrAttackType = NONE;				//

	// Attack01 ����
	Object* mRazerObject = nullptr;
	Boss_Attack01_Script* mRazerScript = nullptr;
	SerializeField(FLOAT, mRazerScale, 1.0f);			// Attack01������ ���� ũ��(����Ʈ, �ݶ��̴�) ���
	SerializeField(FLOAT, mRazerTime, 3.0f);			// Attack01������ ���� ũ��(����Ʈ, �ݶ��̴�) ���
	BOOL isRazerSpawn = FALSE;
	FLOAT mRazerElapsedTime = 0.0f;
	// Attack02 ����
	SerializeField(INT, mMonstaerSpawnCount, 3);		// Attack02������ ���� ��ȯ ��
	ResourceHandle mSpawnEffectTextrueHandle;
public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;
};

