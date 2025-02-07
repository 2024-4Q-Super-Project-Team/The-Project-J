#pragma once

enum class eBossStateType
{
	IDLE,
	ATTACK_01,	// ����
	ATTACK_02,	// ���� ��ȯ
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
	void UpdateTransform(); // ���� ��ġ�� �����Ѵ�.
	void UpdateAnimation();
private:
	Animator* mHeadAnimator;
	Animator* mBodyAnimator;

	Transform* mAxisTransform;						// ������ ���� ���� �� Ʈ������
	SerializeField(FLOAT, mDistanceFromAxis, 0);	// ���� ����� �� ��ŭ ������������

	SerializeField(FLOAT, mMinIdleTick, 0);	// ���� ������ ���� �ּ� ��� �ð�
	SerializeField(FLOAT, mMaxIdleTick, 0);	// ���� ������ ���� �ִ� ��� �ð�

	eBossStateType mBossState;
	BOOL isAttack;
public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;
};

