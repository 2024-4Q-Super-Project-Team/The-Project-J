#include "pch.h"
#include "SavePointManager.h"
#include "PlayerManager.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"

void SavePointManager::Reset()
{
	mSavePointArray.clear();
}

void SavePointManager::AddSavePoint(SavePointScript* _savePoint)
{
	if (_savePoint)
	{
		mSavePointArray.push_back(_savePoint);
	}
}

void SavePointManager::GoBackSavePoint()
{
	auto* Player1 = PlayerManager::GetPlayerInfo(0);
	auto* Player2 = PlayerManager::GetPlayerInfo(1);

	// 세이브포인트 위치를 찾는다.
	//Vector3 spawnPoint = mSavePointArray.back()->GetSavePoint;

	// Player위치를 바꿔준다. 근데 여기서 CharactorController때문에 위치가 안 바뀔 가능성도 있다. 안바뀌면 고려해보자
	//Player1->gameObject->transform->position = 
}
