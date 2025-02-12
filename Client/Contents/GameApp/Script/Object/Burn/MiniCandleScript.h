#pragma once

class MiniCandleScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(MiniCandleScript);

public:
	virtual void Start() override;
	virtual void Update() override;

private:
	Object* mCandleObject = nullptr;
	Object* mFireObject = nullptr;
	class BurnObjectScript* mBurnScript = nullptr;

	bool mIsBurning = false;
	bool mIsBurned = false;
};

