#pragma once

class ScopeScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(ScopeScript);
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
public:
	void SetPlayer(Object* _player) { m_pPlayer = _player; };
	void SetMonster(Object* _monster) { m_pMonster = _monster; };
private:
	Rigidbody* m_pRigidBody = nullptr;
	BoxCollider* m_pCollider = nullptr;
	Object* m_pPlayer = nullptr;
	Object* m_pMonster = nullptr;
};