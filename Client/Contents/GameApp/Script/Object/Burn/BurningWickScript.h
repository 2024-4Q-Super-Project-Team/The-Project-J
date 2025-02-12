#pragma once

class BurningWickScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(BurningWickScript);

public:
	virtual void Start() override;
	virtual void Update() override;

	void SetAnimTime(float time) { mAnimTime = time; }
	void SetSpeed(float speed) { mSpeed = speed; }

private:
	Object* mWickObject = nullptr;
	Object* mFireObject = nullptr;
	class BurnObjectScript* mBurnScript = nullptr;

	bool mIsBurning = false;
	bool mIsBurned = false;

	float mAnimTime = 3.f;
	float mAnimElapsedTime = 0.f;
	float mSpeed = 15.f;
};

