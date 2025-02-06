#pragma once

class BurnObjectScript;
class PlayerScript;

class PlayerCollisionScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(PlayerCollisionScript);
public:
	void Start();
	void Update();
	virtual void _CALLBACK OnTriggerStay(Collider* _origin, Collider* _destination) override;
public:
	inline void SetOwnerPlayer(PlayerScript* _owner) { mOwnerPlayer = _owner; }
private:
	Rigidbody* mRigidBody = nullptr;
	BoxCollider* mBoxCollider = nullptr;
	PlayerScript* mOwnerPlayer = nullptr;
};

