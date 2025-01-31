#pragma once

class PlayerScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(PlayerScript);
public:
	void Start();
	void Update();
	virtual void _CALLBACK OnCollisionEnter(Collider* _origin, Collider* _destination) override;
	virtual void _CALLBACK OnCollisionExit(Collider* _origin, Collider* _destination) override;
private:
	SerializeField(FLOAT, mMoveSpeed, 0.0f);
	SerializeField(FLOAT, mJumpPower, 0.0f);
};
