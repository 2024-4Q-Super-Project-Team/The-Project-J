#pragma once

enum class eMonsterStateType
{
	IDLE,		// ��� ��
	MOVE,		// �����̴� ��
	ATTACK,		// ���� ��
	HIT,		// �´� ��
	GROGGY,		// ����
	DIE			// ����
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
	// Monster�� ������ ���� ������Ʈ
	eMonsterStateType mFSM = eMonsterStateType::IDLE;
	Animator* m_pAnimator = nullptr;
	BoxCollider* m_pHeadCollider = nullptr;
	BoxCollider* m_pFootCollider = nullptr;

public:
	// ������ �ϴ� Ÿ�� ��ġ (�÷��̾� ���� �� ���� ���� �� �÷��̾� ��ġ��)
	Vector3 mTarget{};
	// Player �� ���� ���� �����ϴ°�
	bool bIsTarget = false;
	bool bIsBurn = false;
};

