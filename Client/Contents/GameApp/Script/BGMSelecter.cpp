#include "pch.h"
#include "BGMSelecter.h"

void BGMSelecter::Start()
{
	AudioSourceArray = gameObject->GetComponents<AudioSource>();
}

void BGMSelecter::Update()
{
}

void BGMSelecter::ChangeBGM(eBGMType _type)
{
	if (mCurrAudio)
	{
		if (mCurrAudio == AudioSourceArray[(int)_type])
			return;
		mCurrAudio->FadeOut(1.0f);
	}
	mCurrAudio = AudioSourceArray[(int)_type];
	mCurrAudio->FadeIn(1.0f);
}
