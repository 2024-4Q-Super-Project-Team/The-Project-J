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
	void InitSFX();
	void SetSFX(AudioSource* _dstSource, const std::wstring& _filename);
private:
	eBGMType curBGM;
	AudioSource* mCurrAudio;
	AudioSource* AudioSourceArray[(UINT)eBGMType::SIZE];
};

