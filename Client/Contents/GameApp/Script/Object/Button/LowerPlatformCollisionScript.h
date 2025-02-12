#pragma once
class LowerPlatformButtonScript;

class LowerPlatformCollisionScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(LowerPlatformCollisionScript);
public:
	virtual void Start() override;
	virtual void Update() override;
	virtual void _CALLBACK OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination) override;
public:
	inline void SetOwnerScript(LowerPlatformButtonScript* _owner) { mOwnerScript = _owner; }
private:
	Rigidbody* mRigidBody = nullptr;
	SphereCollider* mCollider = nullptr;
	LowerPlatformButtonScript* mOwnerScript = nullptr;
};