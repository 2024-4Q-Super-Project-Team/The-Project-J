#include "pch.h"
#include "SavePointManager.h"
#include "Contents/GameApp/Script/GameProgressManager.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"
#include "Contents/GameApp/Script/Object/Button/SavePointScript.h"

SavePointManager& SavePointManager::GetInstance()
{
	static SavePointManager instance;
	return instance;
}

void SavePointManager::Reset()
{
	mSavePointArray.clear();
}

void SavePointManager::AddSavePoint(SavePointScript* _savePoint)
{
	if (_savePoint)
	{
		_savePoint->SetIndex(mCurrentIndex++);
		mSavePointArray.push_back(_savePoint);
	}
}

void SavePointManager::GoBackSavePoint(PlayerScript* deadPlayer)
{
	auto* Player1 = GameProgressManager::GetPlayerInfo(0);
	auto* Player2 = GameProgressManager::GetPlayerInfo(1);

    // 가장 큰 인덱스를 가진 세이브 포인트 찾기
    SavePointScript* lastSavePoint = nullptr;
    for (auto* savePoint : mSavePointArray)
    {
        if (!lastSavePoint || savePoint->GetIndex() > lastSavePoint->GetIndex())
        {
            lastSavePoint = savePoint;
        }
    }

    if (!lastSavePoint) return;

    // 마지막 세이브 포인트의 위치 찾아서
    Vector3 spawnPoint = lastSavePoint->GetSavePointPosition();
    spawnPoint.y += 30.0f;

    // 죽은 플레이어만 위치를 바꿔주고, 상태 Reset
    if (Player1 && Player1 == deadPlayer)
    {
     
        Player1->gameObject->transform->position = spawnPoint;
        Player1->Reset();
    }
    if (Player2 && Player2 == deadPlayer)
    {
        Player2->gameObject->transform->position = spawnPoint;
        Player2->Reset();
    }
}
