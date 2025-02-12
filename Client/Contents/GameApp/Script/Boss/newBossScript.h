#pragma once

class newBossRangeScript;
class newBossAttackScript;
class CameraController;

enum class eBossStateType
{
	NONE,
	ENTER,
	IDLE,
	ATTACK,
	GROGGY,
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

	void UpdatePositionZ();			// 두 플레이어 중 가까운 객체에 따라 z값 조정
	void UpdatePositionX();			// 두 포인트를 보간하여 x값 조정
	void UpdateAnimation();

	void SetPositionEnter();		// Enter시의 보스 등장 연출용 포지션 Set
	void SetPositionGroggy();		// Groggy시의 보스 등장 연출용 포지션 Set
	void SetPositionExit();			// Exit시의 보스 등장 연출용 포지션 Set

	void UpdateNone();
	void UpdateEnter();
	void UpdateIdle();
	void UpdateAttack();
	void UpdateGroggy();
	void UpdateExit();
public:
	void SetExit();
private:
	Object* mBossObject;
	Object* mRazerObject;
	Object* mRangeObject;
	Animator* mBodyAnimator;
	AudioSource* mAudioSoruce;
	newBossAttackScript* mAttackScript;
	newBossRangeScript* mRangeScript;

	Object* mPoint[3];
	Vector3 mBossDirection;
	Vector3 mBossOriginPosition;
	eBossStateType mBossState = eBossStateType::NONE;

	CameraController* mCameraController = nullptr;

	SerializeField(FLOAT, mDistanceFromPlayer, 500.0f);

	SerializeField(FLOAT, mMinIdleTick, 5.0f);
	SerializeField(FLOAT, mMaxIdleTick, 7.0f);
	FLOAT mCurIdleTime = 0.0f;							// Min~Max 사이에서 랜덤으로 나온 랜덤 값(다음 공격까지의 대기 시간)
	FLOAT mIdleTickCounter = 0.0f;

	bool isAttack = false;

public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;
};

