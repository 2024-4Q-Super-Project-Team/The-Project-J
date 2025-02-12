#pragma once

enum class eBGMType
{
	OPENING,
	TITLE,
	STAGE_1,
	STAGE_2,
	STAGE_BOSS,
	ENDING,
	NONE,
};

class BGMSelecter : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(BGMSelecter);
public:
	void Start();
	void InitSFX();
	static void SetBGM(eBGMType _type);
	static void ChangeBGM(eBGMType _type);
	static void LoadBGM(AudioSource* _dstSource, const std::wstring& _filename);
private:
	static AudioSource* mCurrAudio;
	static AudioSource* AudioSourceArray[(UINT)eBGMType::NONE];
};

