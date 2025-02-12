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
        auto itr = mSavePointArray.find(_savePoint);
        if (FIND_FAILED(itr, mSavePointArray))
        {
            mSavePointArray.insert(_savePoint);
        }
	}
}

void SavePointManager::GoBackLastSavePoint()
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
    if (lastSavePoint)
    {
        // 마지막 세이브 포인트의 위치 찾아서
        Vector3 spawnPoint = lastSavePoint->GetSavePointPosition();
        Vector3 offsetPoint = Vector3(0, 30.0f, 15.0f);
        spawnPoint += offsetPoint;
        // 죽은 플레이어만 위치를 바꿔주고, 상태 Reset
        if (Player1 && Player2)
        {
            Player1->gameObject->transform->position = spawnPoint;
            Player2->gameObject->transform->position = spawnPoint;
            Player1->gameObject->transform->UpdateMatrix();
            Player2->gameObject->transform->UpdateMatrix();
        }
    }
    // 아무 세이브 포인트도 없으면
    else
    {
        GoBackOriginPoint();
    }
    Player1->Reset();
    Player2->Reset();
    GameProgressManager::SetPlaying();
}

void SavePointManager::GoBackSavePoint(SavePointScript* _savePoint)
{
    if (_savePoint)
    {
        // 마지막 세이브 포인트의 위치 찾아서
        Vector3 spawnPoint = _savePoint->GetSavePointPosition();
        Vector3 offsetPoint = Vector3(0, 30.0f, 15.0f);
        spawnPoint += offsetPoint;

        auto* Player1 = GameProgressManager::GetPlayerInfo(0);
        auto* Player2 = GameProgressManager::GetPlayerInfo(1);
        if (Player1 && Player2)
        {
            Player1->gameObject->transform->position = spawnPoint;
            Player2->gameObject->transform->position = spawnPoint;
            Player1->gameObject->transform->UpdateMatrix();
            Player2->gameObject->transform->UpdateMatrix();
        }
        Player1->Reset();
        Player2->Reset();
        GameProgressManager::SetPlaying();
    }
}

void SavePointManager::GoBackOriginPoint()
{
    auto* Player1 = GameProgressManager::GetPlayerInfo(0);
    auto* Player2 = GameProgressManager::GetPlayerInfo(1);

    Player1->gameObject->transform->position = Player1->mOriginPos;
    Player1->gameObject->transform->rotation = Player1->mOriginRot;
    Player2->gameObject->transform->position = Player2->mOriginPos;
    Player2->gameObject->transform->rotation = Player2->mOriginRot;
    Player1->gameObject->transform->UpdateMatrix();
    Player2->gameObject->transform->UpdateMatrix();

    Player1->Reset();
    Player2->Reset();
    GameProgressManager::SetPlaying();
}

void SavePointManager::JumpingSavePoint(INT _index)
{
    SavePointScript* targetPoint = SavePointScript::GetSavePoint(_index);
    if (targetPoint)
    {
        AddSavePoint(targetPoint);
        GoBackSavePoint(targetPoint);
    }
    else
    { 
        GoBackOriginPoint();
    }
}
