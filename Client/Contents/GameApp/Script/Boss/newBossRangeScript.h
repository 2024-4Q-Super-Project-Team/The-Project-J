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
	BoxCollider* mRangeCollider;							// 보스 등장에 해당하는 범위
	std::unordered_set<PlayerScript*> mPlayerEnterTable;	// 들어와있는 플레이어 테이블
};