#pragma once

enum class eBGMType
{
	OPENING,
	TITLE,
	STAGE_1,
	STAGE_2,
	STAGE_BOSS,
	ENDING,
	SIZE,
};

class BGMSelecter : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(BGMSelecter);
public:
	void Start();
	void Update();
	void ChangeBGM(eBGMType _type);
private:
	eBGMType curBGM;
	AudioSource* mCurrAudio;
	std::vector<AudioSource*> AudioSourceArray;
};

