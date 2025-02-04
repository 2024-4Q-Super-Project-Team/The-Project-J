#pragma once
#include "Helper/InputSyncer.h"

// 
enum class ePlayerStateType
{
	IDLE,		// ��� ��
	MOVE,		// �����̴� ��
	JUMP,		// ���� ��
	MOVE_FIRE,	// ���� �ű�� ��
	DEAD,		// �����
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
	void Reset();			// �÷��̾��� �ʱ� ���¸� �ǵ���
	void SetHP(INT _val);	// �÷��̾��� HP�� ����
	void Hit(INT _damage);	// �÷��̾�� �ǰ��� ��Ű�� �Լ�

	inline INT GetPlayerHandle() { return mPlayerHandle.val; }
	inline INT GetCurrentHP() { return mPlayerCurHP; }
	//////////////////////////////////////////////////////////////////////

private:
	void UpdateInput();
	void UpdatePlayerHP(); // �÷��̾��� ü�¿� ���� ������Ʈ
private:
	// 1p, 2p �÷��̾� ���п뵵
	SerializeField(INT,		mPlayerHandle, 0);
	// ���ο��� ����� Player�� ���� ������Ʈ ������
	Animator*				mAnimator = nullptr;
	PlayerController*		mPlayerController = nullptr;
	BurnObjectScript*		mBurnObjectScript;

	////////////////////////////////////////////////
	// [02/02 ~] ���� �۾� - �÷��̾� ü�� ����
	// �ʿ� ���� : 
	// 1. �÷��̾��� ü�� (0~100�� ���� ��)
	// 2. ü�� �Ҹ� ƽ (�� �ʸ��� ü���� ���� ���ΰ�? �� ���� ƽ)
	// 3. ü�� �Ҹ� ī���� (ü�� �Ҹ� �Ǵ� ƽ�� ����ϱ� ���� ī����)
	////////////////////////////////////////////////
	INT						mPlayerCurHP = 100;
	SerializeField(INT,		mPlayerMaxHP, 100);			// Player Hp (0~100�� ���� ��)
	SerializeField(FLOAT,	mHpReduceTick, 1.0f);		// ���� �������� �� �� �� ���� HP�� ���� ���ΰ�?
	FLOAT					mHpReduceCount = 0.0f;		
	SerializeField(FLOAT,	mMoveSpeed, 1.0f);			
	SerializeField(FLOAT,	mJumpPower, 1.0f);			
	////////////////////////////////////////////////
	// [02/02 ~] ���� �۾� - �÷��̾� FSM
	// �ʿ� ���� : 
	// �׼� ������ ���� (�׼��߿��� �������� ���ϰ� �ϱ� ����)
	// 
	////////////////////////////////////////////////
	bool					isAction = false;
	ePlayerStateType		mPlayerState = ePlayerStateType::IDLE;
public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;
};
