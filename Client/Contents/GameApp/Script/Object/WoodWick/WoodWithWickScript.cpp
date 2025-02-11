#include "pch.h"
#include "WoodWithWickScript.h"
#include "../Burn/BurnObjectScript.h"
#include "../../Player/PlayerScript.h"

void WoodWithWickScript::Start()
{
	{
		auto& children = gameObject->transform->GetChildren();
		for (auto& child : children)
		{
			if (child->gameObject->GetName() == L"WoodObject")
				mWoodObject = child->gameObject;
			if (child->gameObject->GetName() == L"PlateObject")
				mPlateObject = child->gameObject;
		}
	}
	{
		auto& children = mWoodObject->transform->GetChildren();
		for (auto& child : children)
		{
			if (child->gameObject->GetTag() == L"WoodWick")
				mWickObject = child->gameObject;
		}
	}
	
	mBurnScript = gameObject->AddComponent<BurnObjectScript>();
	mBurnScript->SetBurn(false);

	gameObject->AddComponent<Rigidbody>();
	SphereCollider* sc = gameObject->AddComponent<SphereCollider>();
	sc->SetRadius(100.f);
	sc->SetIsTrigger(true);

	mWickAnimator = mWickObject->GetComponent<Animator>();
	mInitialPlatePos = mPlateObject->transform->position;
}

void WoodWithWickScript::Update()
{
	if (mBurnScript->IsBurning() && !mIsBurned)
	{
		mWickAnimator->SetCurrentAnimation(L"Weakness05");
		mWickAnimator->Play();
		mIsBurned = true;
	}	

	if (mWickAnimator->IsEnd())
	{
		Transform* tr = mPlateObject->transform;
		tr->RotateTo(2.f, Quaternion::CreateFromYawPitchRoll(0,0,80));

		mWickAnimator->Stop();
	}
}

void WoodWithWickScript::OnTriggerStay(Collider* range, Collider* player)
{
	if (player->gameObject->GetTag() != L"Player") return;

	PlayerScript* playerScript = player->gameObject->GetComponent<PlayerScript>();

	if (playerScript)
	{
		playerScript->OnTriggerStayCallback(player, range);
	}
}
