#include "pch.h"
#include "BurningWickScript.h"
#include "../Burn/BurnObjectScript.h"

void BurningWickScript::Start()
{
	mWickObject = gameObject;
	mFireObject = FindChildObject(gameObject, L"FireObject");
	mFireObject->SetActive(false);

	mBurnScript = mWickObject->AddComponent<BurnObjectScript>();
	mBurnScript->SetBurn(false);
}

void BurningWickScript::Update()
{
	if (mBurnScript->IsBurning() && !mIsBurning)
	{
		mFireObject->SetActive(true);
		mFireObject->GetComponent<Animator>()->Play();
		mIsBurning = true;
	}

	if (mIsBurning && !mIsBurned)
	{
		mAnimElapsedTime += Time::GetScaledDeltaTime();
		mWickObject->transform->position.y -= mSpeed * Time::GetScaledDeltaTime();
		if (mAnimElapsedTime > mAnimTime)
		{
			mIsBurned = true;
		}
	}
}
