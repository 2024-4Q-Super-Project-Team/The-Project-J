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

	INT GetPlayerHandle() { return mPlayerHandle.val; }
	INT GetMaxHpValue() { return mPlayerMaxHP.val; }
	//////////////////////////////////////////////////////////////////////

private:
	void UpdatePlayerHP();	// �÷��̾��� ü�¿� ���� ������Ʈ
	void UpdateInput();		// �Է¿� ���� �÷��̾��� ������Ʈ
private:
	// 1p, 2p �÷��̾� ���п뵵
	SerializeField(INT,		mPlayerHandle, 0);
	// ���ο��� ����� Player�� ���� ������Ʈ ������
	Animator*				mAnimator = nullptr;
	PlayerController*		mPlayerController = nullptr;
	BurnObjectScript*		mBurnObjectScript;

	////////////////////////////////////////////////
	// [02/02 ~] ���� �۾� - �÷��̾� ���� ����
	////////////////////////////////////////////////
	INT						mPlayerCurHP = 100;			// �÷��̾��� ���� ü��
	SerializeField(INT,		mPlayerMaxHP, 100);			// �÷��̾��� �ִ� ü��
	SerializeField(FLOAT,	mHpReduceTick, 1.0f);		// ���� �������� �� �� �� ���� HP�� ���� ���ΰ�?
	FLOAT					mHpReduceCount = 0.0f;		// �� �ʰ� �����°�? mHpReduceTick���� ������ ü���� ��� ī���͸� Tick��ŭ ����
	SerializeField(FLOAT,	mMoveSpeed, 1.0f);			// �÷��̾��� �̵��ӵ�
	SerializeField(FLOAT,	mJumpPower, 1.0f);			// �÷��̾��� ������
	Vector3					mVelocity = Vector3::Zero;
	////////////////////////////////////////////////
	// [02/02 ~] ���� �۾� - �÷��̾� FSM
	// �ʿ� ���� : 
	// �׼� ������ ���� (�׼��߿��� �������� ���ϰ� �ϱ� ����)
	// 
	////////////////////////////////////////////////
	bool					isAction = false;
	bool					isJump = false;
	ePlayerStateType		mPlayerState = ePlayerStateType::IDLE;
public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;
};
