#pragma once

enum class eMonsterStateType
{
	IDLE,		// 대기 중
	MOVE,		// 움직이는 중
	ATTACK,		// 공격 중
	HIT,		// 맞는 중
	GROGGY,		// 기절
	DIE			// 죽음
};

class BoxCollider;
class Animator;

class MonsterScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(MonsterScript);
public:
	void Start();
	void Update();
public:
	virtual void OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination) override;
	virtual void OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination)  override;
	virtual void OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)  override;
public:
	virtual void OnTriggerEnter(Collider* _origin, Collider* _destination)	override;
	virtual void OnTriggerStay(Collider* _origin, Collider* _destination)	override;
	virtual void OnTriggerExit(Collider* _origin, Collider* _destination)	override;

protected:
	// Monster가 가지고 있을 컴포넌트
	eMonsterStateType mFSM = eMonsterStateType::IDLE;
	Animator* m_pAnimator = nullptr;
	BoxCollider* m_pHeadCollider = nullptr;
	BoxCollider* m_pFootCollider = nullptr;

public:
	// 가고자 하는 타겟 위치 (플레이어 없을 시 랜덤 있을 시 플레이어 위치로)
	Vector3 mTarget{};
	// Player 가 범위 내에 존재하는가
	bool bIsTarget = false;
	bool bIsBurn = false;
};

