#pragma once
#include "Helper/InputSyncer.h"

// 
enum class ePlayerStateType
{
	IDLE,		// 대기 중
	MOVE,		// 움직이는 중
	HIT,		// 맞는 중
	MOVE_FIRE,	// 불을 옮기는 중
	OFF_FIRE,	// 불을 끄는 중
	DEAD,		// 사ㅋ망ㅋ
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
	/// 외부 사용을 위해 제공하는 메서드
	//////////////////////////////////////////////////////////////////////
	void Reset();												// 플레이어의 초기 상태를 되돌림
	void SetHP(INT _val);										// 플레이어의 HP를 변경
	void Hit(INT _damage);										// 플레이어에게 피격을 시키는 함수
	void Jump(FLOAT _scale = 1.0f, bool _canHold = true);		// 플레이어에게 점프를 시키는 함수
	
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

	void UpdatePlayerHP();			// 플레이어의 체력에 대한 업데이트
	void UpdatePlayerAnim();		// 플레이어의 애니메이션에 대한 업데이트

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
	// 몸, 촛대, 불 자식 오브젝트
	Object* mBodyObject = nullptr;
	Object* mCandleObject = nullptr;
	Object* mFireObject = nullptr;
	Object* mCollisionObject = nullptr;
	Object* mFireOffEffectObject = nullptr;
	Transform* mCandleTopBone = nullptr;
	// 1p, 2p 플레이어 구분용도
	SerializeField(INT,		mPlayerHandle, 0);
	// 내부에서 사용할 Player가 가진 컴포넌트 포인터
	Animator*				mBodyAnimator = nullptr;
	Animator*				mCandleAnimator = nullptr;
	AudioSource*			mAudioSource = nullptr;
	PlayerController*		mPlayerController = nullptr;
	CameraController*		mCameraController = nullptr;
	BurnObjectScript*		mBurnObjectScript = nullptr;

	BurnObjectScript*		mBurnProcessTarget = nullptr; // 불 끄기, 혹은 불 옮기기의 작업 대상
	PlayerCollisionScript*	mCollisionScript = nullptr;
	////////////////////////////////////////////////
	// 플레이어 스탯 관련 변수
	////////////////////////////////////////////////
	INT						mPlayerCurHP = 100;
	SerializeField(INT,		mPlayerMaxHP, 100);			// Player Hp (0~100의 정수 값)
	SerializeField(FLOAT,	mHpReduceTick, 1.0f);		// 불이 켜져있을 때 몇 초 마다 HP가 깎일 것인가?
	FLOAT					mHpReduceCount = 0.0f;		
	SerializeField(FLOAT,	mMoveSpeed, 200.0f);			
	SerializeField(FLOAT,	mJumpPower, 0.3f);			
	SerializeField(FLOAT,	mMaxJumpTimeTick, 0.4f);	
	SerializeField(FLOAT,	mMaxJumpHoldScale, 1.0f);	
	FLOAT					mJumpTimeCount = 0.0f;
	bool					mJumpTrigger = false;
	////////////////////////////////////////////////
	// 플레이어 상태 체크용 변수
	////////////////////////////////////////////////
	bool					isAction = false;
	bool					isJump = true;
	ePlayerStateType		mPlayerState = ePlayerStateType::IDLE;
	std::unordered_set<BurnObjectScript*> mBurnObjectTriggerTable;
	////////////////////////////////////////////////
	// 플레이어 액션 관련 변수
	////////////////////////////////////////////////
	SerializeField(FLOAT, mMoveFireTick, 2.0f);
	FLOAT mMoveFireCount = 0.0f;

	// 카메라연출 체크용 bool변수
	bool isInTrigger = false;
	bool isDone6 = false;
	bool isDone7 = false;


public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;
};
