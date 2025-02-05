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

	//���� ���� ����������

	if (mBurnScript->IsBurning())
	{
		//�����̵� ���� �ٲ۴�. 
		mController->SetSlopeMode(PlayerController::SlopeMode::Slide);
		//�̲������� �ʾҴٸ� �ð��� �ø���, �ð� üũ : �ð� �ٵǸ� �߷��� Ű��� �̲����� true
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
		//�̲������� �ִٸ� �̲����� �ð��� �ø���, �ð� üũ : �ð� �ٵǸ� �߷��� ������� �ٲٰ� �̲����� false
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
	else //���� �����ִٸ� 
	{
		//��� ���� �ٲ۴�. 
		mController->SetSlopeMode(PlayerController::SlopeMode::Ride);

		//��� �ð��� �ʱ�ȭ�Ѵ�. 
		mNotSlidingElapsedTime = 0.f;
		mSlidingElapsedTime = 0.f;
		//�߷��� �ʱ�ȭ�Ѵ�. 
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
