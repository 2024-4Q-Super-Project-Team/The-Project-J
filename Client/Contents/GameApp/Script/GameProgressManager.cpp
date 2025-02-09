#include "pch.h"
#include "GameProgressManager.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"

eGameProgressStatus	GameProgressManager::mGameStatus = eGameProgressStatus::PLAYING;
PlayerScript* GameProgressManager::mPlayer[2] = { nullptr, nullptr };
Object* GameProgressManager::mStagearray[STAGE_COUNT] = { nullptr ,nullptr ,nullptr ,nullptr };
INT	GameProgressManager::mCurrentStageNum = 0;

void GameProgressManager::Start()
{
	{
		// �������� ���� �ʱ�ȭ
		mStagearray[STAGE_01] = FindObjectWithName(L"MAP1");
		mStagearray[STAGE_02] = FindObjectWithName(L"MAP2");
		mStagearray[STAGE_03] = FindObjectWithName(L"MAP3");
		mStagearray[STAGE_04] = FindObjectWithName(L"MAP4");
	}
}

void GameProgressManager::Update()
{
	// ���� �����Ȳ�� �����Ѵ�. (���� ����, Ŭ���� ��)

	// UpdateMap();	// ���� Ȱ��ȭ, ��Ȱ��ȭ�Ѵ�
}

void GameProgressManager::ResetGame()
{
	if(mPlayer[0]) mPlayer[0]->Reset();
	if(mPlayer[1]) mPlayer[1]->Reset();
}

void GameProgressManager::UpdateMap()
{
	for (int i = 0; i < STAGE_COUNT; ++i)
	{
		if (mStagearray[i])
		{
			if (i == mCurrentStageNum)
			{
				mStagearray[i]->SetActive(true);
			}
			else
			{
				mStagearray[i]->SetActive(false);
			}
		}
	} 
}

void GameProgressManager::UpdateGameOver()
{
	// �÷��̾� �� �� �Ѹ��̶� �װų�
	if (mPlayer[0]->GetCurrentHP() == 0 ||
		mPlayer[1]->GetCurrentHP() == 0)
	{
		mGameStatus = eGameProgressStatus::GAME_OVER;
	}
	// �÷��̾� ���� ���� �� �����ų�
	if (mPlayer[0]->IsBurning() == false &&
		mPlayer[1]->IsBurning() == false)
	{
		mGameStatus = eGameProgressStatus::GAME_OVER;
	}
}

void GameProgressManager::SetPlayerInfo(PlayerScript* _player)
{
	if (_player)
	{
		mPlayer[_player->GetPlayerHandle()] = _player;
	}
}

PlayerScript* GameProgressManager::GetPlayerInfo(INT _handle)
{
	return mPlayer[_handle];
}

eGameProgressStatus GameProgressManager::GetGameStatus()
{
	return mGameStatus;
}
