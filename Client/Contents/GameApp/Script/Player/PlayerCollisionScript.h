#pragma once

class BurnObjectScript;
class PlayerScript;

class PlayerCollisionScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(PlayerCollisionScript);
public:
	virtual void Start() override;
	virtual void Update() override;
	virtual void _CALLBACK OnTriggerStay(Collider* _origin, Collider* _destination) override;
	virtual void _CALLBACK OnTriggerExit(Collider* _origin, Collider* _destination) override;
public:
	inline void SetOwnerPlayer(PlayerScript* _owner) { mOwnerPlayer = _owner; }
private:
	Rigidbody* mRigidBody = nullptr;
	SphereCollider* mCollider = nullptr;
	PlayerScript* mOwnerPlayer = nullptr;
};

