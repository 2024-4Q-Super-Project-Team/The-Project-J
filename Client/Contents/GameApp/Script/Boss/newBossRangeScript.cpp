#include "pch.h"
#include "newBossRangeScript.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"

void newBossRangeScript::Start()
{
	mRangeCollider = gameObject->GetComponent<BoxCollider>();

}

void newBossRangeScript::Update()
{
	// 플레이어 들어와있으면
	if (mPlayerEnterTable.empty() == false)
	{

	}
}

void _CALLBACK newBossRangeScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{
	if (_destination->gameObject->GetTag() == L"Player")
	{
		PlayerScript* player = _destination->gameObject->GetComponent<PlayerScript>();
		if (player)
		{
			auto itr = mPlayerEnterTable.find(player);
			if (FIND_FAILED(itr, mPlayerEnterTable))
			{
				mPlayerEnterTable.insert(player);
			}
		}
	}
	return void _CALLBACK();
}

void _CALLBACK newBossRangeScript::OnTriggerExit(Collider* _origin, Collider* _destination)
{
	if (_destination->gameObject->GetTag() == L"Player")
	{
		PlayerScript* player = _destination->gameObject->GetComponent<PlayerScript>();
		if (player)
		{
			auto itr = mPlayerEnterTable.find(player);
			if (FIND_SUCCESS(itr, mPlayerEnterTable))
			{
				mPlayerEnterTable.erase(itr);
			}
		}
	}
	return void _CALLBACK();
}
