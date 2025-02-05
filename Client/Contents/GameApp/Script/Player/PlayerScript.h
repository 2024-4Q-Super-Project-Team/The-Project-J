#pragma once
#include "Helper/InputSyncer.h"

// 
enum class ePlayerStateType
{
	IDLE,		// ��� ��
	MOVE,		// �����̴� ��
	HIT,		// �´� ��
	MOVE_FIRE,	// ���� �ű�� ��
	DEAD,		// �礻����
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
	//////////////////////////////////////////////////////////////////////
	/// �ܺ� ����� ���� �����ϴ� �޼���
	//////////////////////////////////////////////////////////////////////
	void Reset();					// �÷��̾��� �ʱ� ���¸� �ǵ���
	void SetHP(INT _val);			// �÷��̾��� HP�� ����
	void Hit(INT _damage);			// �÷��̾�� �ǰ��� ��Ű�� �Լ�

	inline INT	GetPlayerHandle() { return mPlayerHandle.val; }
	inline INT	GetCurrentHP() { return mPlayerCurHP; }
	inline INT	GetMaxHpValue() { return mPlayerMaxHP.val; }
	inline bool IsJump() { return mPlayerController->IsGround() == false; }
	//////////////////////////////////////////////////////////////////////
private:
	void InitFireLight();
	void UpdatePlayerHP();			// �÷��̾��� ü�¿� ���� ������Ʈ
	void UpdatePlayerAnim();		// �÷��̾��� �ִϸ��̼ǿ� ���� ������Ʈ

	void UpdateIdle();
	void UpdateMove();
	void UpdateHit();
	void UpdateAction();
	void UpdateDead();

	bool ProcessMove();
	void ProcessJump();

	inline void SetState(ePlayerStateType _stateType) { mPlayerState = _stateType; }
private:
	// ��, �д�, �� �ڽ� ������Ʈ
	Object* mBodyObject = nullptr;
	Object* mCandleObject = nullptr;
	Object* mFireObject = nullptr;
	Transform* mCandleTopBone = nullptr;
	// 1p, 2p �÷��̾� ���п뵵
	SerializeField(INT,		mPlayerHandle, 0);
	// ���ο��� ����� Player�� ���� ������Ʈ ������
	Animator*				mBodyAnimator = nullptr;
	Animator*				mCandleAnimator = nullptr;
	PlayerController*		mPlayerController = nullptr;
	BurnObjectScript*		mBurnObjectScript = nullptr;

	////////////////////////////////////////////////
	// [02/02 ~] ���� �۾� - �÷��̾� ���� ����
	////////////////////////////////////////////////
	INT						mPlayerCurHP = 100;
	SerializeField(INT,		mPlayerMaxHP, 100);			// Player Hp (0~100�� ���� ��)
	SerializeField(FLOAT,	mHpReduceTick, 1.0f);		// ���� �������� �� �� �� ���� HP�� ���� ���ΰ�?
	FLOAT					mHpReduceCount = 0.0f;		
	SerializeField(FLOAT,	mMoveSpeed, 200.0f);			
	SerializeField(FLOAT,	mJumpPower, 0.3f);			
	SerializeField(FLOAT,	mMaxJumpTimeTick, 0.4f);	
	FLOAT					mJumpTimeCount = 0.0f;
	bool					mJumpTrigger = false;
	////////////////////////////////////////////////
	// [02/02 ~] ���� �۾� - �÷��̾� FSM
	// �ʿ� ���� : 
	// �׼� ������ ���� (�׼��߿��� �������� ���ϰ� �ϱ� ����)
	// 
	////////////////////////////////////////////////
	bool					isAction = false;
	bool					isJump = true;
	ePlayerStateType		mPlayerState = ePlayerStateType::IDLE;
public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;
};
