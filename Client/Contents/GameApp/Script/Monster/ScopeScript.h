#pragma once

class ScopeScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(ScopeScript);
public:
	void Start();
	void Update();
	virtual void OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination) override;
	virtual void OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination)  override;
	virtual void OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)  override;
private:
	Rigidbody* m_pRigidBody = nullptr;
	BoxCollider* m_pCollider = nullptr;
	Object* m_pPlayer = nullptr;
};