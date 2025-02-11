#pragma once

class newBossScript;

class newBossAttackScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(newBossAttackScript);
public:
	void Start();
	void Update();
	void _CALLBACK OnTriggerStay(Collider* _origin, Collider* _destination);
public:
	void SetAttackStart(Vector3 _pos);
	void SetAttackEnd();
	inline void SetRootObject(newBossScript* _root) { mRootObject = _root; }
	inline bool IsAttack() { return isAttack; }
private:
	newBossScript* mRootObject;	// 보스 오브젝트

	Rigidbody* mRigidBody;
	BoxCollider* mHitColiider;
	Animator* mAnimator;

	SerializeField(INT,   mRazerDamage, 30);		// 패턴의 광선 데미지
	SerializeField(FLOAT, mRazerDist, 800.0f);		// 패턴의 광선 거리
	SerializeField(FLOAT, mRazerRange, 700.0f);		// 패턴의 광선 반경
	SerializeField(FLOAT, mRazerScale, 1.0f);		// 패턴의 광선 크기(이펙트, 콜라이더) 계수
	SerializeField(FLOAT, mRazerTime, 3.0f);		// 패턴의 광선 총 시간

	bool	isAttack;
	Vector3 mOriginScale = Vector3(200.0f, 3000.0f, 200.0f);
	Vector3 mSpawnPosition = Vector3::Zero;
	Vector3 mColliderExternt = Vector3::One;
	FLOAT	mRazerElapsedTime = 0.0f;

public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;
};