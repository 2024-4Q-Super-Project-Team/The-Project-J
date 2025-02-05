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

	//���� ���� ����������

	if (mBurnScript->IsBurning())
	{
		//�����̵� ���� �ٲ۴�. 
		mController->SetSlopeMode(PlayerController::SlopeMode::Slide);
	
	}
	else //���� �����ִٸ� 
	{
		//��� ���� �ٲ۴�. 
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
