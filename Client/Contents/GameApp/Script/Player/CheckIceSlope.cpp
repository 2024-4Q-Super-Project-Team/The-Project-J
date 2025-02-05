#include "pch.h"
#include "CheckIceSlope.h"
#include "../Object/Burn/BurnObjectScript.h"

void CheckIceSlope::Start()
{
	
}

void CheckIceSlope::Update()
{
}

void _CALLBACK CheckIceSlope::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
	if (_destination->gameObject->GetTag() != L"IceSlope")
		return;

	if (mBurnScript == nullptr)
		mBurnScript = gameObject->GetComponent<BurnObjectScript>();

	if (mController == nullptr)
		mController = gameObject->GetComponent<PlayerController>();

	return void _CALLBACK();
}

void _CALLBACK CheckIceSlope::OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination)
{
	if (_destination->gameObject->GetTag() != L"IceSlope")
		return;

	if ( !mBurnScript || !mController ) return;

	if (mBurnScript->IsBurning())
	{
		mController->SetSlopeMode(PlayerController::SlopeMode::Slide);
	}
	else
	{
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
