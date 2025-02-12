#pragma once

class BGMSelecter : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(BGMSelecter);
public:
	void Start();
	void Update();
private:
	AudioSource* mStage01BGM;
	AudioSource* mStage02BGM;
	AudioSource* mStage03BGM;
};

