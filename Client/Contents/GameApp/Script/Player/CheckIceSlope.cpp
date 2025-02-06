#include "pch.h"
#include "CheckIceSlope.h"
#include "../Object/Burn/BurnObjectScript.h"

void CheckIceSlope::Start()
{
	mBurnScript = gameObject->GetComponent<BurnObjectScript>();
	mController = gameObject->GetComponent<PlayerController>();

	mInitialGravity = mController->GetGravity();
}

void _CALLBACK CheckIceSlope::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
	if (_destination->gameObject->GetTag() != L"IceSlope")
		return;

	if (!mBurnScript || !mController) return;


	return void _CALLBACK();
}

void _CALLBACK CheckIceSlope::OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination)
{
	if (_destination->gameObject->GetTag() != L"IceSlope")
		return;

	if ( !mBurnScript || !mController ) return;

	//현재 불이 켜져있으면

	if (mBurnScript->IsBurning())
	{
		//슬라이드 모드로 바꾼다. 
		mController->SetSlopeMode(PlayerController::SlopeMode::Slide);
		//미끄러지지 않았다면 시간을 늘리고, 시간 체크 : 시간 다되면 중력을 키우고 미끄러짐 true
		if (!mIsSliding)
		{
			mNotSlidingElapsedTime += Time::GetScaledDeltaTime();

			if (mNotSlidingElapsedTime > NotSlidingTime.val)
			{
				mIsSliding = true;
				mNotSlidingElapsedTime = 0.f;
				mController->SetGravity(mSlideGravity);
			}
		}
		//미끄러지고 있다면 미끄러짐 시간을 늘리고, 시간 체크 : 시간 다되면 중력을 원래대로 바꾸고 미끄러짐 false
		else
		{
			mSlidingElapsedTime += Time::GetScaledDeltaTime();
			if (mSlidingElapsedTime > SlidingTime.val)
			{
				mIsSliding = false;
				mSlidingElapsedTime = 0.f;
				mController->SetGravity(mInitialGravity);
			}
		}
	}
	else //불이 꺼져있다면 
	{
		//등반 모드로 바꾼다. 
		mController->SetSlopeMode(PlayerController::SlopeMode::Ride);

		//모든 시간을 초기화한다. 
		mNotSlidingElapsedTime = 0.f;
		mSlidingElapsedTime = 0.f;
		//중력을 초기화한다. 
		mController->SetGravity(mInitialGravity);
	}


	return void _CALLBACK();
}

void _CALLBACK CheckIceSlope::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
	if (_destination->gameObject->GetTag() != L"IceSlope")
		return;

	if (!mController) return;

	mNotSlidingElapsedTime = 0.f;
	mSlidingElapsedTime = 0.f;

	mController->SetSlopeMode(PlayerController::SlopeMode::Ride);
	mController->SetGravity(mInitialGravity);

	return void _CALLBACK();
}
