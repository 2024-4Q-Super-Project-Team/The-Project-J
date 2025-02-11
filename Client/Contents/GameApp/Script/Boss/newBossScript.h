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

	void UpdatePositionZ();		// 두 플레이어 중 가까운 객체에 따라 z값 조정
	void UpdatePositionX();		// 두 포인트를 보간하여 x값 조정
	void UpdatePositionEnter();	// Enter시의 보스 등장 연출용 포지션 업데이트
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

	Object* mPoint[2];					// 보스가 이동할 방향을 정해주기 위한 두 점
	Vector3 mBossDirection;
	Vector3 mBossOriginPosition;
	eBossStateType mBossState = eBossStateType::NONE;

	SerializeField(FLOAT, mDistanceFromPlayer, 500.0f);

	SerializeField(FLOAT, mMinIdleTick, 5.0f);
	SerializeField(FLOAT, mMaxIdleTick, 7.0f);
	FLOAT mCurIdleTime = 0.0f;							// Min~Max 사이에서 랜덤으로 나온 랜덤 값(다음 공격까지의 대기 시간)
	FLOAT mIdleTickCounter = 0.0f;

	bool isAttack = false;
};

