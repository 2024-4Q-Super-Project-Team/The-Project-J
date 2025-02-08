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

void SavePointManager::GoBackSavePoint()
{
	auto* Player1 = GameProgressManager::GetPlayerInfo(0);
	auto* Player2 = GameProgressManager::GetPlayerInfo(1);

    // ���� ū �ε����� ���� ���̺� ����Ʈ ã��
    SavePointScript* lastSavePoint = nullptr;
    for (auto* savePoint : mSavePointArray)
    {
        if (!lastSavePoint || savePoint->GetIndex() > lastSavePoint->GetIndex())
        {
            lastSavePoint = savePoint;
        }
    }

    if (!lastSavePoint) return;

    // ������ ���̺� ����Ʈ�� ��ġ ã�Ƽ�
    Vector3 spawnPoint = lastSavePoint->GetSavePointPosition();

    // �ű�� Player ��ġ�� �ٲ��ְ�, Player ���� Reset
    if (Player1)
    {
        Player1->gameObject->transform->position = spawnPoint;
        Player1->Reset();
    }
    if (Player2)
    {
        Player2->gameObject->transform->position = spawnPoint;
        Player2->Reset();
    }
}
