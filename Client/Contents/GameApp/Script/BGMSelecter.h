#pragma once

class BGMSelecter : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(BGMSelecter);
public:
	void Start();
	void Update();
	static void ChangeBGM(INT _num);
private:
	int curIndex;
	static AudioSource* mCurrAudio;
	static std::vector<AudioSource*> AudioSourceArray;
};

