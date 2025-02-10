#include "pch.h"
#include "SpriteAnimScript.h"

void SpriteAnimScript::Start()
{
	mRenderer = gameObject->GetComponent<SpriteRenderer>();
	mFrameCount = mRenderer->GetFrameCount();
}

void SpriteAnimScript::Update()
{
	if (mRenderer == nullptr) return;

	if (mIsPlaying)
	{
		mElapsedTime += Time::GetUnScaledDeltaTime();

		if (mElapsedTime >= (mEffectTime/mFrameCount) )
		{
			mElapsedTime = 0.f;
			mNowFrameIndex++;
			mRenderer->SetIndex(mNowFrameIndex);
		}

		if (mFrameCount == mNowFrameIndex)
		{
			mIsPlaying = false;
			mNowFrameIndex = 0;
			mRenderer->SetIndex(0);
			mElapsedTime = 0.f;
			mRenderer->SetActive(false);
		}
	}
}

void SpriteAnimScript::Play()
{
	mIsPlaying = true;
	mRenderer->SetActive(true);
	mNowFrameIndex = 0;
	mRenderer->SetIndex(0);
}
