#pragma once

enum class eBossStateType
{
	IDLE,
	ATTACK_01,
	ATTACK_02,
	HIT,
};

class BossScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(BossScript);
public:
	void Start();
	void Update();
private:
	void UpdateTransform(); // 보스 위치를 갱신한다.
private:
	Animator* mHeadAnimator;
	Animator* mBodyAnimator;

	Transform* mAxisTransform;						// 보스의 기준 축이 될 트랜스폼
	SerializeField(FLOAT, mDistanceFromAxis, 0);	// 기준 축부터 몇 만큼 떨어져있을지

	SerializeField(FLOAT, mMinIdleTick, 0);	// 다음 패턴을 위한 최소 대기 시간
	SerializeField(FLOAT, mMaxIdleTick, 0);	// 다음 패턴을 위한 최대 대기 시간


public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;
};

