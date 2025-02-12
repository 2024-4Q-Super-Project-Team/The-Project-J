#include "pch.h"
#include "HeavyRockSystem.h"
#include "../Burn/BurnObjectScript.h"
#include "../Burn/BurningWickScript.h"

void HeavyRockSystem::Start()
{
	auto& children = gameObject->transform->GetChildren();
	for (auto& child : children)
	{
		if (child->gameObject->GetName() == L"FirstShortWick")
			mFirstShortWick = child->gameObject;
		else if (child->gameObject->GetName() == L"SecondShortWick")
			mSecondShortWick = child->gameObject;
		else if (child->gameObject->GetName() == L"FirstLongWick")
			mFirstLongWick = child->gameObject;
		else if (child->gameObject->GetName() == L"SecondLongWick")
			mSecondLongWick = child->gameObject;
		else if (child->gameObject->GetName() == L"Rock")
			mRock = child->gameObject;
	}
	////////////////////////////////////////////////////////////////

	mFirstShortFire = FindChildObject(mFirstShortWick, L"FireObject");
	mSecondShortFire = FindChildObject(mSecondShortWick, L"FireObject");

	mFirstShortFire->SetActive(false);
	mSecondShortFire->SetActive(false);

	mFirstBurnScript = mFirstShortWick->AddComponent<BurnObjectScript>();
	mSecondBurnScript = mSecondShortWick->AddComponent<BurnObjectScript>();

	mFirstShortWick->AddComponent<Rigidbody>();
	mSecondShortWick->AddComponent<Rigidbody>();

	SphereCollider* firstSp = mFirstShortWick->AddComponent<SphereCollider>();
	SphereCollider* secondSp = mSecondShortWick->AddComponent<SphereCollider>();

	firstSp->SetIsTrigger(true);
	secondSp->SetIsTrigger(true);

	///////////////////////////////////////////////////////////////

	mFirstLongFire = FindChildObject(mFirstLongWick, L"FireObject");
	mSecondLongFire = FindChildObject(mSecondLongWick, L"FireObject");

	mFirstLongFire->SetActive(false);
	mSecondLongFire->SetActive(false);

}

void HeavyRockSystem::Update()
{
	if (mIsAllDone) return;

	//짧은 심지 불타는 것 처리
	if (mFirstBurnScript->IsBurning() && !mFirstShortBurned)
	{
		if (mFirstBurnScript->GetDestObject()->gameObject->
			transform->GetParent()->gameObject->GetName() == L"Player1")
		{
			mFirstShortBurned = true;
			mFirstShortFire->SetActive(true);
			mFirstShortFire->GetComponent<Animator>()->Play();
		}
	}

	if (mSecondBurnScript->IsBurning() && !mSecondShortBurned)
	{
		if (mSecondBurnScript->GetDestObject()->gameObject->
			transform->GetParent()->gameObject->GetName() == L"Player2")
		{
			mSecondShortBurned = true;
			mSecondShortFire->SetActive(true);
			mSecondShortFire->GetComponent<Animator>()->Play();
		}
	}

	//긴 심지 불타는 것 처리
	if (mFirstShortBurned && !mFirstLongBurned)
	{
		mFirstAnimElapsedTime += Time::GetScaledDeltaTime();
		mFirstLongWick->transform->position.y -= 30.f * Time::GetScaledDeltaTime();
		if (mFirstAnimElapsedTime > mAnimTime)
		{
			mFirstLongBurned = true;
			mFirstLongWick->SetActive(false);
		}
	}

	if (mSecondShortBurned && !mSecondLongBurned)
	{
		mSecondAnimElapsedTime += Time::GetScaledDeltaTime();
		mSecondLongWick->transform->position.y -= 30.f * Time::GetScaledDeltaTime();
		if (mSecondAnimElapsedTime > mAnimTime)
		{
			mSecondLongBurned = true;
			mSecondLongWick->SetActive(false);
		}
	}

	if (mFirstLongBurned && mSecondLongBurned)
		mAllBurned = true;

	if (mAllBurned)
	{
		mRock->transform->position.y -= mFallingSpeed * Time::GetScaledDeltaTime();

		if (mRock->transform->position.y < -200.f)
		{
			mRock->SetActive(false);
			mIsAllDone = true;
		}
	}
}
