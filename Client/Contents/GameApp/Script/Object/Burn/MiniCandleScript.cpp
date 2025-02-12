#include "pch.h"
#include "MiniCandleScript.h"
#include "../Burn/BurnObjectScript.h"
#include "../../GameProgressManager.h"
#include "../../Boss/NewBossScript.h"

void MiniCandleScript::Start()
{
	mCandleObject = gameObject;
	mFireObject = FindChildObject(mCandleObject, L"FireObject");

	mBurnScript = mCandleObject->AddComponent<BurnObjectScript>();
	mCandleObject->AddComponent<Rigidbody>();
	SphereCollider* sp = mCandleObject->AddComponent<SphereCollider>();
	sp->SetRadius(100);
	sp->SetIsTrigger(true);
	mBurnScript->SetBurn(false);
	mFireObject->SetActive(false);
}

void MiniCandleScript::Update()
{
	if (mBurnScript->IsBurning() && !mIsBurning)
	{
		mFireObject->SetActive(true);
		mFireObject->GetComponent<Animator>()->Play();
		GameProgressManager::GetBossInfo()->SetExit();
		mIsBurning = true;
	}
}
