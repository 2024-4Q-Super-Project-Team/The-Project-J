#include "pch.h"
#include "CheckIceSlope.h"
#include "../Object/Burn/BurnObjectScript.h"

void CheckIceSlope::Start()
{
	mBurnScript = gameObject->GetComponent<BurnObjectScript>();
	mController = gameObject->GetComponent<PlayerController>();
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
	
	}
	else //불이 꺼져있다면 
	{
		//등반 모드로 바꾼다. 
		mController->SetSlopeMode(PlayerController::SlopeMode::Ride);
	}

	return void _CALLBACK();
}

void _CALLBACK CheckIceSlope::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
	if (_destination->gameObject->GetTag() != L"IceSlope")
		return;

	if (!mController) return;

	mController->SetSlopeMode(PlayerController::SlopeMode::Ride);

	return void _CALLBACK();
}
