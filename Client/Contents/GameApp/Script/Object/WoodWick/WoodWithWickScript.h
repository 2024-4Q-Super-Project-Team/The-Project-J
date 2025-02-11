#pragma once
class WoodWithWickScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(WoodWithWickScript);

public:
	virtual void Start() override;
	virtual void Update() override;

	virtual void OnTriggerStay(Collider* range, Collider* player) override;

private:
	Object* mWoodObject = nullptr;
	Object* mPlateObject = nullptr;
	Object* mWickObject = nullptr;
	Animator* mWickAnimator = nullptr;
	class BurnObjectScript* mBurnScript = nullptr;

	Vector3 mInitialPlatePos;

	bool mIsBurned = false;
	bool mRotated = false;

	float mElapsedTime = 0.f;
	float mRotateTime = 2.f;
};

