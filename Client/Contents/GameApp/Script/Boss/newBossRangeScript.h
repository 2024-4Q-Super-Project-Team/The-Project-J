#pragma once

class PlayerScript;

class newBossRangeScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(newBossRangeScript);
public:
	void Start();
	void Update();
	virtual void _CALLBACK OnTriggerEnter(Collider* _origin, Collider* _destination);
	virtual void _CALLBACK OnTriggerExit(Collider* _origin, Collider* _destination);
public:
	inline bool IsTriggerOn() { return mPlayerEnterTable.empty() == false; }
private:
	BoxCollider* mRangeCollider;							// ���� ���忡 �ش��ϴ� ����
	std::unordered_set<PlayerScript*> mPlayerEnterTable;	// �����ִ� �÷��̾� ���̺�
};