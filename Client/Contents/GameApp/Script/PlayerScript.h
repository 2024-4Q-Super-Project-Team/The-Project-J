#pragma once

class PlayerScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(PlayerScript);
public:
	void Start();
	void Update();
private:
	SerializeField(FLOAT, mMoveSpeed, 0.0f);
	SerializeField(FLOAT, mJumpPower, 0.0f);
};

