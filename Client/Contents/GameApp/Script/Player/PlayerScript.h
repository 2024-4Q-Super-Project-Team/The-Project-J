#pragma once
#include "Helper/InputSyncer.h"

// 
enum class ePlayerStateType
{
	IDLE,		// ��� ��
	MOVE,		// �����̴� ��
	HIT,		// �´� ��
	MOVE_FIRE,	// ���� �ű�� ��
	OFF_FIRE,	// ���� ���� ��
	DEAD,		// �礻����
};

class BurnObjectScript;
class PlayerCollisionScript;
class CameraController;

class PlayerScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(PlayerScript);
public:
	void Start();
	void Update();
	void PostRender();
	virtual void _CALLBACK OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination) override;
	virtual void _CALLBACK OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination) override;
	virtual void _CALLBACK OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination) override;
	virtual void _CALLBACK OnTriggerEnter(Collider* _origin, Collider* _destination) override;
	virtual void _CALLBACK OnTriggerStay(Collider* _origin, Collider* _destination) override;
	virtual void _CALLBACK OnTriggerExit(Collider* _origin, Collider* _destination) override;
	void _CALLBACK OnTriggerStayCallback(Collider* _origin, Collider* _destination);
	void _CALLBACK OnTriggerExitCallback(Collider* _origin, Collider* _destination);
public:
	//////////////////////////////////////////////////////////////////////
	/// �ܺ� ����� ���� �����ϴ� �޼���
	//////////////////////////////////////////////////////////////////////
	void Reset();												// �÷��̾��� �ʱ� ���¸� �ǵ���
	void SetHP(INT _val);										// �÷��̾��� HP�� ����
	void Hit(INT _damage);										// �÷��̾�� �ǰ��� ��Ű�� �Լ�
	void Jump(FLOAT _scale = 1.0f, bool _canHold = true);		// �÷��̾�� ������ ��Ű�� �Լ�
	
	inline INT	  GetPlayerHandle() { return mPlayerHandle.val; }
	inline INT	  GetCurrentHP()    { return mPlayerCurHP; }
	inline INT	  GetMaxHpValue()   { return mPlayerMaxHP.val; }
	bool IsBurning();
	bool IsJump();

	float GetRatio();
	ePlayerStateType GetState() { return mPlayerState; }
	bool CanMoveFire() { return mBurnObjectTriggerTable.empty() == false; };
	//////////////////////////////////////////////////////////////////////
private:
	void InitFireLight();

	void UpdatePlayerHP();			// �÷��̾��� ü�¿� ���� ������Ʈ
	void UpdatePlayerAnim();		// �÷��̾��� �ִϸ��̼ǿ� ���� ������Ʈ

	void UpdateIdle();
	void UpdateMove();
	void UpdateHit();
	void UpdateMoveFire();
	void UpdateOffFire();
	void UpdateDead();

	bool ProcessMove();
	void ProcessJump();
	void ProcessMoveFire(BurnObjectScript* _dst);
	void ProcessOffFire(BurnObjectScript* _dst);
	//void ProcessWalkSFX();

	inline void SetState(ePlayerStateType _stateType) { mPlayerState = _stateType; }
	void ResetController();
private:
	// ��, �д�, �� �ڽ� ������Ʈ
	Object* mBodyObject = nullptr;
	Object* mCandleObject = nullptr;
	Object* mFireObject = nullptr;
	Object* mCollisionObject = nullptr;
	Object* mFireOffEffectObject = nullptr;
	Transform* mCandleTopBone = nullptr;
	// 1p, 2p �÷��̾� ���п뵵
	SerializeField(INT,		mPlayerHandle, 0);
	// ���ο��� ����� Player�� ���� ������Ʈ ������
	Animator*				mBodyAnimator = nullptr;
	Animator*				mCandleAnimator = nullptr;
	AudioSource*			mAudioSource = nullptr;
	PlayerController*		mPlayerController = nullptr;
	CameraController*		mCameraController = nullptr;
	BurnObjectScript*		mBurnObjectScript = nullptr;

	BurnObjectScript*		mBurnProcessTarget = nullptr; // �� ����, Ȥ�� �� �ű���� �۾� ���
	PlayerCollisionScript*	mCollisionScript = nullptr;
	////////////////////////////////////////////////
	// �÷��̾� ���� ���� ����
	////////////////////////////////////////////////
	INT						mPlayerCurHP = 100;
	SerializeField(INT,		mPlayerMaxHP, 100);			// Player Hp (0~100�� ���� ��)
	SerializeField(FLOAT,	mHpReduceTick, 1.0f);		// ���� �������� �� �� �� ���� HP�� ���� ���ΰ�?
	FLOAT					mHpReduceCount = 0.0f;		
	SerializeField(FLOAT,	mMoveSpeed, 200.0f);			
	SerializeField(FLOAT,	mJumpPower, 0.3f);			
	SerializeField(FLOAT,	mMaxJumpTimeTick, 0.4f);	
	SerializeField(FLOAT,	mMaxJumpHoldScale, 1.0f);	
	FLOAT					mJumpTimeCount = 0.0f;
	bool					mJumpTrigger = false;
	////////////////////////////////////////////////
	// �÷��̾� ���� üũ�� ����
	////////////////////////////////////////////////
	bool					isAction = false;
	bool					isJump = true;
	ePlayerStateType		mPlayerState = ePlayerStateType::IDLE;
	std::unordered_set<BurnObjectScript*> mBurnObjectTriggerTable;
	////////////////////////////////////////////////
	// �÷��̾� �׼� ���� ����
	////////////////////////////////////////////////
	SerializeField(FLOAT, mMoveFireTick, 2.0f);
	FLOAT mMoveFireCount = 0.0f;

	// ī�޶��� üũ�� bool����
	bool isInTrigger = false;
	bool isDone6 = false;
	bool isDone7 = false;


public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;
};
