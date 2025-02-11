#pragma once

class newBossRangeScript;
class newBossAttackScript;

enum class eBossStateType
{
	NONE,
	ENTER,
	IDLE,
	ATTACK,
	EXIT,
};

class newBossScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(newBossScript);
public:
	void Start();
	void Update();

	void Reset();

	void UpdatePositionZ();		// �� �÷��̾� �� ����� ��ü�� ���� z�� ����
	void UpdatePositionX();		// �� ����Ʈ�� �����Ͽ� x�� ����
	void UpdatePositionEnter();	// Enter���� ���� ���� ����� ������ ������Ʈ
	void UpdateAnimation();

	void UpdateNone();
	void UpdateEnter();
	void UpdateIdle();
	void UpdateAttack();
	void UpdateExit();
private:
	Object* mBossObject;
	Object* mRazerObject;
	Object* mRangeObject;
	Animator* mBodyAnimator;
	newBossAttackScript* mAttackScript;
	newBossRangeScript* mRangeScript;

	Object* mPoint[2];					// ������ �̵��� ������ �����ֱ� ���� �� ��
	Vector3 mBossDirection;
	Vector3 mBossOriginPosition;
	eBossStateType mBossState = eBossStateType::NONE;

	SerializeField(FLOAT, mDistanceFromPlayer, 500.0f);

	SerializeField(FLOAT, mMinIdleTick, 5.0f);
	SerializeField(FLOAT, mMaxIdleTick, 7.0f);
	FLOAT mCurIdleTime = 0.0f;							// Min~Max ���̿��� �������� ���� ���� ��(���� ���ݱ����� ��� �ð�)
	FLOAT mIdleTickCounter = 0.0f;

	bool isAttack = false;
};

