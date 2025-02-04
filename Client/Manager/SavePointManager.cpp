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

	// ���̺�����Ʈ ��ġ�� ã�´�.
	//Vector3 spawnPoint = mSavePointArray.back()->GetSavePoint;

	// Player��ġ�� �ٲ��ش�. �ٵ� ���⼭ CharactorController������ ��ġ�� �� �ٲ� ���ɼ��� �ִ�. �ȹٲ�� ����غ���
	//Player1->gameObject->transform->position = 
}
