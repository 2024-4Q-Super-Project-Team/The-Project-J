#pragma once

// 
enum class ePlayerStateType
{
	IDLE,		// 대기 중
	MOVE,		// 움직이는 중
	JUMP,		// 점프 중
	MOVE_FIRE	// 불을 옮기는 중
};

// 키 바인딩(동기화) 용 (ex. 키보드와 패드 둘 다 MOVE_LEFT를 인식 가능하게?)
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
	// 1p, 2p 플레이어 구분용도
	UINT mPlayerHandle;
	// Player의 상태
	ePlayerStateType mPlayerState = ePlayerStateType::IDLE;
	// 내부에서 사용할 Player가 가진 컴포넌트 포인터
	FiniteStateMachine* mFSM = nullptr;
	Animator*			mAnimator = nullptr;
	PlayerController*	mPlayerController = nullptr;
	BurnObjectScript*	mBurnObjectScript;

	SerializeField(FLOAT, mMoveSpeed, 0.0f);
	SerializeField(FLOAT, mJumpPower, 0.0f);

	////////////////////////////////////////////////
	// [02/02 ~] 주형 작업 - 플레이어 체력 관련
	// 필요 변수 : 
	// 1. 플레이어의 체력 (0~100의 정수 값)
	// 2. 체력 소모 틱 (몇 초마다 체력을 깎을 것인가? 에 대한 틱)
	// 3. 체력 소모 카운터 (체력 소모가 되는 틱을 계산하기 위한 카운터)
	////////////////////////////////////////////////
	SerializeField(INT,		mPlayerHP, 100);			// Player Hp (0~100의 정수 값)
	SerializeField(FLOAT,	mHpReduceTick, 1.0f);		// 불이 켜져있을 때 몇 초 마다 HP가 깎일 것인가?
	SerializeField(FLOAT,	mHpReduceCount, 0.0f);		// 몇 초가 지났는가? mHpReduceTick보다 높으면 체력을 깎고 카운터를 Tick만큼 감소
	
};
