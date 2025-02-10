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
    spawnPoint.y += 30.0f;

    // ���� �÷��̾ ��ġ�� �ٲ��ְ�, ���� Reset
    if (Player1 && Player1 == deadPlayer)
    {
        // Reset�� ���� ȣ���Ͽ� ���� �ʱ�ȭ
        Player1->Reset();
        Player1->Jump(0.5f);
        Player1->gameObject->transform->position = Vector3(spawnPoint.z, spawnPoint.y, spawnPoint.x);
        // ��ġ ���� �� ��Ʈ�ѷ� ��ġ�� ������Ʈ
        //Player1->mPlayerContro

    }
    if (Player2 && Player2 == deadPlayer)
    {
        Player2->gameObject->transform->position = Vector3(spawnPoint.z, spawnPoint.y, spawnPoint.x);;
        Player2->Reset();
    }
}
