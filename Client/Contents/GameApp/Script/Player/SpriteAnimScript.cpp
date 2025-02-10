#include "pch.h"
#include "SpriteAnimScript.h"

void SpriteAnimScript::Start()
{
	mRenderer = gameObject->GetComponent<SpriteRenderer>();

}

void SpriteAnimScript::Update()
{
	if (mRenderer == nullptr) return;


	if (mIsPlaying)
	{
		mElapsedTime += Time::GetUnScaledDeltaTime();

		if (mEffectTime >= (mEffectTime/mFrameCount) )
		{
			mElapsedTime = 0.f;
			mRenderer->SetIndex(mNowFrameIndex);
		}

		if (mFrameCount == mNowFrameIndex)
		{
			mIsPlaying = false;
			mNowFrameIndex = 0;
			mElapsedTime = 0.f;
		}
			
		mNowFrameIndex++;
	}
}

void SpriteAnimScript::Play()
{
	mIsPlaying = true;
}
