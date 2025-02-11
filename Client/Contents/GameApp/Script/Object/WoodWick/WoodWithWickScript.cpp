#include "pch.h"
#include "WoodWithWickScript.h"
#include "../Burn/BurnObjectScript.h"
#include "../../Player/PlayerScript.h"
#include "../../SpriteAnimScript.h"

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

		mFireObject = FindChildObject(mWickObject, L"FireObject");
		mFireObject->SetActive(false);
	}
	
	mBurnScript = gameObject->AddComponent<BurnObjectScript>();
	mBurnScript->SetBurn(false);

	gameObject->AddComponent<Rigidbody>();
	SphereCollider* sc = gameObject->AddComponent<SphereCollider>();
	sc->SetRadius(100.f);
	sc->SetIsTrigger(true);


	mInitialPlatePos = mPlateObject->transform->position;
}

void WoodWithWickScript::Update()
{
	if (mBurnScript->IsBurning() && !mIsBurned)
	{
		mFireObject->SetActive(true);
		mFireObject->GetComponent<Animator>()->Play();
		mIsBurned = true;
	}	

	if (mIsBurned && !mRotated)
	{
		mAnimElapsedTime += Time::GetScaledDeltaTime();
		mWickObject->transform->position.y -= 15.f * Time::GetScaledDeltaTime();
		if (mAnimElapsedTime > mAnimTime)
		{
			Transform* tr = mPlateObject->transform;
			tr->RotateTo(2.f, Quaternion::CreateFromYawPitchRoll(0, 0, 80));

			mRotated = true;
		}
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
