#include "pch.h"
#include "GameProgressManager.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"

PlayerScript* GameProgressManager::mPlayer[2] = { nullptr, nullptr };
Object* GameProgressManager::mStagearray[STAGE_COUNT] = { nullptr ,nullptr ,nullptr ,nullptr };
INT	GameProgressManager::mCurrentStageNum = 0;

void GameProgressManager::Start()
{
	{
		// 스테이지 등을 초기화
		mStagearray[STAGE_01] = FindObjectWithName(L"MAP1");
		mStagearray[STAGE_02] = FindObjectWithName(L"MAP2");
		mStagearray[STAGE_03] = FindObjectWithName(L"MAP3");
		mStagearray[STAGE_04] = FindObjectWithName(L"MAP4");
	}
}

void GameProgressManager::Update()
{
	// 게임 진행상황을 갱신한다. (게임 오버, 클리어 등)

	//UpdateMap();	// 맵을 활성화, 비활성화한다
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
