#pragma once

// 
enum class ePlayerStateType
{
	IDLE,		// ��� ��
	MOVE,		// �����̴� ��
	JUMP,		// ���� ��
	MOVE_FIRE	// ���� �ű�� ��
};

// Ű ���ε�(����ȭ) �� (ex. Ű����� �е� �� �� MOVE_LEFT�� �ν� �����ϰ�?)
enum class ePlayerInputType
{
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_FORWARD,
	MOVE_BACKWARD,
	JUMP,
	ACTION_MOVE_FIRE,
	ACTION_OFF_FIRE,
};

class BurnObjectScript;

class PlayerScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(PlayerScript);
public:
	void Start();
	void Update();
	virtual void OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination) override;
	virtual void OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination) override;
	virtual void OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination) override;
public:
	float GetKeyForce(ePlayerInputType _inputType);
private:
	// 1p, 2p �÷��̾� ���п뵵
	UINT mPlayerHandle;
	// Player�� ����
	ePlayerStateType mPlayerState = ePlayerStateType::IDLE;
	// ���ο��� ����� Player�� ���� ������Ʈ ������
	FiniteStateMachine* mFSM = nullptr;
	Animator*			mAnimator = nullptr;
	PlayerController*	mPlayerController = nullptr;
	BurnObjectScript*	mBurnObjectScript;

	SerializeField(FLOAT, mMoveSpeed, 0.0f);
	SerializeField(FLOAT, mJumpPower, 0.0f);

	////////////////////////////////////////////////
	// [02/02 ~] ���� �۾� - �÷��̾� ü�� ����
	// �ʿ� ���� : 
	// 1. �÷��̾��� ü�� (0~100�� ���� ��)
	// 2. ü�� �Ҹ� ƽ (�� �ʸ��� ü���� ���� ���ΰ�? �� ���� ƽ)
	// 3. ü�� �Ҹ� ī���� (ü�� �Ҹ� �Ǵ� ƽ�� ����ϱ� ���� ī����)
	////////////////////////////////////////////////
	SerializeField(INT,		mPlayerHP, 100);			// Player Hp (0~100�� ���� ��)
	SerializeField(FLOAT,	mHpReduceTick, 1.0f);		// ���� �������� �� �� �� ���� HP�� ���� ���ΰ�?
	SerializeField(FLOAT,	mHpReduceCount, 0.0f);		// �� �ʰ� �����°�? mHpReduceTick���� ������ ü���� ��� ī���͸� Tick��ŭ ����
	
};
