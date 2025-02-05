#pragma once
#include "Helper/InputSyncer.h"

// 
enum class ePlayerStateType
{
	IDLE,		// 대기 중
	MOVE,		// 움직이는 중
	JUMP,		// 점프 중
	MOVE_FIRE,	// 불을 옮기는 중
	DEAD,		// 사ㅋ망ㅋ
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
	/// 외부 사용을 위해 제공하는 메서드
	//////////////////////////////////////////////////////////////////////
	void Reset();					// 플레이어의 초기 상태를 되돌림
	void SetHP(INT _val);			// 플레이어의 HP를 변경
	void Hit(INT _damage);			// 플레이어에게 피격을 시키는 함수

	inline INT GetPlayerHandle() { return mPlayerHandle.val; }
	inline INT GetCurrentHP() { return mPlayerCurHP; }
	inline INT GetMaxHpValue() { return mPlayerMaxHP.val; }
	//////////////////////////////////////////////////////////////////////
private:
	void InitFireLight();
	void UpdatePlayerMove();		// 플레이어의 인풋에 관련된 업데이트
	void UpdatePlayerHP();			// 플레이어의 체력에 대한 업데이트

	void UpdateIdle();
	void UpdateMove();
	void UpdateJump();
	void UpdateAction();
	void UpdateDead();

	inline void SetState(ePlayerStateType _stateType) { mPlayerState = _stateType; }
private:
	// 몸, 촛대, 불 자식 오브젝트
	Object* mBodyObject = nullptr;
	Object* mCandleObject = nullptr;
	Object* mFireObject = nullptr;
	Transform* mCandleTopBone = nullptr;
	// 1p, 2p 플레이어 구분용도
	SerializeField(INT,		mPlayerHandle, 0);
	// 내부에서 사용할 Player가 가진 컴포넌트 포인터
	Animator*				mBodyAnimator = nullptr;
	Animator*				mCandleAnimator = nullptr;
	PlayerController*		mPlayerController = nullptr;
	BurnObjectScript*		mBurnObjectScript;

	////////////////////////////////////////////////
	// [02/02 ~] 주형 작업 - 플레이어 체력 관련
	// 필요 변수 : 
	// 1. 플레이어의 체력 (0~100의 정수 값)
	// 2. 체력 소모 틱 (몇 초마다 체력을 깎을 것인가? 에 대한 틱)
	// 3. 체력 소모 카운터 (체력 소모가 되는 틱을 계산하기 위한 카운터)
	////////////////////////////////////////////////
	INT						mPlayerCurHP = 100;
	SerializeField(INT,		mPlayerMaxHP, 100);			// Player Hp (0~100의 정수 값)
	SerializeField(FLOAT,	mHpReduceTick, 1.0f);		// 불이 켜져있을 때 몇 초 마다 HP가 깎일 것인가?
	FLOAT					mHpReduceCount = 0.0f;		
	SerializeField(FLOAT,	mMoveSpeed, 200.0f);			
	SerializeField(FLOAT,	mJumpPower, 0.5f);			
	////////////////////////////////////////////////
	// [02/02 ~] 주형 작업 - 플레이어 FSM
	// 필요 변수 : 
	// 액션 중인지 여부 (액션중에는 움직이지 못하게 하기 위해)
	// 
	////////////////////////////////////////////////
	bool					isAction = false;
	ePlayerStateType		mPlayerState = ePlayerStateType::IDLE;
public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;
};
