#include "pch.h"
#include "BGMSelecter.h"

AudioSource* BGMSelecter::mCurrAudio = nullptr;
std::vector<AudioSource*> BGMSelecter::AudioSourceArray = {};

void BGMSelecter::Start()
{
	AudioSourceArray = gameObject->GetComponents<AudioSource>();
}

void BGMSelecter::Update()
{
}

void BGMSelecter::ChangeBGM(INT _num)
{
	if (mCurrAudio)
	{
		if (mCurrAudio == AudioSourceArray[_num])
			return;
		mCurrAudio->FadeOut(1.0f);
	}
	mCurrAudio = AudioSourceArray[_num];
	mCurrAudio->FadeIn(1.0f);
}
