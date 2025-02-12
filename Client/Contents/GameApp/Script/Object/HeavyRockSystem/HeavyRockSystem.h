#pragma once

class BurnObjectScript;

class HeavyRockSystem : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(HeavyRockSystem);
public:

	virtual void Start() override;
	virtual void Update() override;

private:
	Object* mFirstShortWick;
	Object* mSecondShortWick;

	Object* mFirstShortFire;
	Object* mSecondShortFire;

	BurnObjectScript* mFirstBurnScript;
	BurnObjectScript* mSecondBurnScript;

	bool mFirstShortBurned = false;
	bool mSecondShortBurned = false;

	///////////////////////////////////

	Object* mFirstLongWick;
	Object* mSecondLongWick;

	Object* mFirstLongFire;
	Object* mSecondLongFire;

	bool mFirstLongBurned = false;
	bool mSecondLongBurned = false;

	float mFirstAnimElapsedTime = 0.f;
	float mSecondAnimElapsedTime = 0.f;

	float mAnimTime = 4.f;

	bool mAllBurned = false;

	///////////////////////////////////
	Object* mRock;
	float mFallingSpeed = 10.f;

	bool mIsAllDone = false;
};

