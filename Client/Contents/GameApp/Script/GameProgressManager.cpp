#include "pch.h"
#include "GameProgressManager.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"
#include "Manager/SavePointManager.h"
#include "CameraController.h"

eGameProgressStatus	GameProgressManager::mGameStatus = eGameProgressStatus::PLAYING;
PlayerScript* GameProgressManager::mPlayer[2] = { nullptr, nullptr };
newBossScript* GameProgressManager::mBossScript = nullptr;
Object* GameProgressManager::mScenearray[STAGE_COUNT] = { nullptr ,nullptr ,nullptr };
Object* GameProgressManager::mMainCamera = nullptr;
INT	GameProgressManager::mCurrentSceneNum = 0;

void GameProgressManager::Start()
{
	{
		// 스테이지 등을 초기화
		mScenearray[OPENING] = FindObjectWithName(L"OpeningWorld");
		mScenearray[TITLE] = FindObjectWithName(L"TitleWorld");
		mScenearray[GAME] = FindObjectWithName(L"MainWorld");
		mScenearray[ENDING] = FindObjectWithName(L"EndingWorld");
		mMainCamera = FindObjectWithName(L"Main_Camera");

		mGameStatus = eGameProgressStatus::PLAYING;
		ChangeScene(eSceneType::OPENING);
	}
}

void GameProgressManager::Update()
{
	// 게임 진행상황을 갱신한다. (게임 오버, 클리어 등)

	UpdateMap();	// 맵을 활성화, 비활성화한다
	UpdateGameOver();

	// 스킵 구간 점핑
	SavePointManager& SaveManager = SavePointManager::GetInstance();
	for (int i = 0; i < 10; i++)
	{
		INT keyCode = 49 + i;
		if (Input::IsKeyDown(keyCode)) SaveManager.JumpingSavePoint(i);
	}
}

	if (Input::IsKeyDown(VK_F1))
	{
		ChangeScene(eSceneType::GAME);
	}
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
		if (mScenearray[i])
		{
			if (i == mCurrentSceneNum)
			{
				mScenearray[i]->SetActive(true);
			}
			else
			{
				mScenearray[i]->SetActive(false);
			}
		}
	} 
}

void GameProgressManager::SetPlaying()
{
	mGameStatus = eGameProgressStatus::PLAYING;
}

void GameProgressManager::UpdateGameOver()
{
	// 플레이어 둘 중 한명이라도 죽거나
	if (mPlayer[0]->GetCurrentHP() == 0 ||
		mPlayer[1]->GetCurrentHP() == 0)
	{
		mGameStatus = eGameProgressStatus::GAME_OVER;
	}
	// 플레이어 둘의 불이 다 꺼지거나
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

void GameProgressManager::SetBossInfo(newBossScript* _boss)
{
	mBossScript = _boss;
}

newBossScript* GameProgressManager::GetBossInfo()
{
	return mBossScript;
}

void GameProgressManager::ChangeScene(eSceneType _sceneType)
{
	mCurrentSceneNum = _sceneType;

	if (_sceneType == eSceneType::GAME)
	{
		mMainCamera->GetComponent<CameraController>()->SetActive(true);
	}
	else
	{
		mMainCamera->GetComponent<CameraController>()->SetActive(false);
	}
}

eGameProgressStatus GameProgressManager::GetGameStatus()
{
	return mGameStatus;
}
