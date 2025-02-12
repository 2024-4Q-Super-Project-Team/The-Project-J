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

	void UpdatePositionZ();			// �� �÷��̾� �� ����� ��ü�� ���� z�� ����
	void UpdatePositionX();			// �� ����Ʈ�� �����Ͽ� x�� ����
	void UpdateAnimation();

	void SetPositionEnter();		// Enter���� ���� ���� ����� ������ Set
	void SetPositionGroggy();		// Groggy���� ���� ���� ����� ������ Set
	void SetPositionExit();			// Exit���� ���� ���� ����� ������ Set

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
	FLOAT mCurIdleTime = 0.0f;							// Min~Max ���̿��� �������� ���� ���� ��(���� ���ݱ����� ��� �ð�)
	FLOAT mIdleTickCounter = 0.0f;

	bool isAttack = false;

public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;
};

