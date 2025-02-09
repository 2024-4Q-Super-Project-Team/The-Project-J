#pragma once

enum class eBossStateType
{
	IDLE,
	ATTACK,
	HIT,
};

class Boss_Attack01_Script;

class BossScript : public MonoBehaviour
{
	enum eBossAttackType
	{
		NONE,		// 없음
		ATTACK_01,	// 광선
		ATTACK_02,	// 몬스터 소환
	};
public:
	_MONOBEHAVIOUR_INIT(BossScript);
public:
	void Start();
	void Update();
private:
	inline void SetState(eBossStateType _stateType) { mBossState = _stateType; }

	void UpdateTransform(); // 보스 위치를 갱신한다.
	void UpdateAnimation();	// 보스 애니메이션을 갱신한다.
	void UpdateState();		// 보스 상태에 따른 업데이트
	// ===FSM===
	void UpdateIdle();
	void UpdateAttack();
	void UpdateAttack01();
	void UpdateAttack02();
	void UpdateHit();
private:
	Object* mBodyObject;
	Object* mHeadObject;
	Animator* mBodyAnimator;
	Animator* mHeadAnimator;

	Transform* mAxisTransform;							// 보스의 기준 축이 될 트랜스폼
	SerializeField(FLOAT, mDistanceFromAxis, 572.0f);	// 기준 축부터 몇 만큼 떨어져있을지

	SerializeField(FLOAT, mMinIdleTick, 3.0f);			// 다음 패턴을 위한 최소 대기 시간
	SerializeField(FLOAT, mMaxIdleTick, 5.0f);			// 다음 패턴을 위한 최대 대기 시간
	FLOAT mCurIdleTime = 0.0f;							// Min~Max 사이에서 랜덤으로 나온 랜덤 값(다음 공격까지의 대기 시간)
	FLOAT mIdleTickCounter = 0.0f;						// 다음 공격 패턴을 위한 시간 카운터

	eBossStateType mBossState = eBossStateType::IDLE;	//
	BOOL isAttack = FALSE;								//
	eBossAttackType mCurrAttackType = NONE;				//

	// Attack01 관련
	Object* mRazerObject = nullptr;
	Boss_Attack01_Script* mRazerScript = nullptr;
	SerializeField(FLOAT, mRazerScale, 1.0f);			// Attack01패턴의 광선 크기(이펙트, 콜라이더) 계수
	SerializeField(FLOAT, mRazerTime, 3.0f);			// Attack01패턴의 광선 크기(이펙트, 콜라이더) 계수
	BOOL isRazerSpawn = FALSE;
	FLOAT mRazerElapsedTime = 0.0f;
	// Attack02 관련
	SerializeField(INT, mMonstaerSpawnCount, 3);		// Attack02패턴의 몬스터 소환 수
	ResourceHandle mSpawnEffectTextrueHandle;
public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;
};

