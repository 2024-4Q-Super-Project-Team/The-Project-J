#include "pch.h"
#include "Application/Application.h"
#include "GameManager.h"
#include "ComponentManager.h"
#include "World/WorldManager.h"
#include "Resource/ResourceManager.h"
#include "Graphics/GraphicsManager.h"
#include "ViewportScene/ViewportManager.h"
#include "Editor/EditorManager.h"
#include "Physics/PhysicsManager.h"
#include "ViewportScene/ViewportScene.h"

volatile eEngineRunType		GameManager::mCurrRunType = eEngineRunType::NONE;
volatile eEngineRunType		GameManager::mNextRunType = eEngineRunType::NONE;
float				GameManager::mFixedUpdateTick = 0.02f;
Application*		GameManager::mApplication = nullptr;
PhysicsManager*		GameManager::mPhysicsManager = nullptr;
ComponentManager*	GameManager::mComponentManager = nullptr;

GameManager::GameManager(Application* _pApplication)
{
	mApplication = _pApplication;
	mPhysicsManager = new PhysicsManager();
	mComponentManager = new ComponentManager();
}

GameManager::~GameManager()
{
}

BOOL GameManager::Initialize()
{
	Time::Initialize();
	Input::Initialize();
	AudioHub::Initialize();

	GraphicsManager::Initialize();
	ResourceManager::Initialize();
	UIManager::Initialize();
	ViewportManager::Initialize();
	EditorManager::Initialize();
	mComponentManager->Initialize();
	mPhysicsManager->Initialize();
	return TRUE;
}

void GameManager::Run()
{
	MSG msg;
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		Input::InputMessegeProcess(msg);
	}
	else
	{
		Time::Update();
		AudioHub::Update();
		GameManager::UpdateGame();
		UIManager::Update();

		switch (mCurrRunType)
		{
		case eEngineRunType::NONE:
			break;
		case eEngineRunType::GAME_MODE:
			ViewportManager::GameRun();
			break;
		case eEngineRunType::EDITOR_MODE:
			ViewportManager::EditorRun();
			break;
		default:
			break;
		}
		Input::Update();
	}
}

void GameManager::Finalization()
{
	AudioHub::Finalization();
	GraphicsManager::Finalization();
	ResourceManager::Finalization();
	ViewportManager::Finalization();
	EditorManager::Finalization();
	UIManager::Finalization();
	mComponentManager->Finalization();
	mPhysicsManager->Finalization();
	SAFE_DELETE(mPhysicsManager);
}

void GameManager::UpdateGame()
{
	if (mCurrRunType != mNextRunType)
	{
		// 게임모드 -> 에디터모드 = 세이브를 다시 불러온다.
		if ((mCurrRunType == eEngineRunType::GAME_MODE && mNextRunType == eEngineRunType::EDITOR_MODE) ||
			(mCurrRunType == eEngineRunType::NONE && mNextRunType == eEngineRunType::GAME_MODE))
		{
			SaveManager::Load();
		}
		// 에디터모드 -> 게임모드 = 지금까지의 정보를 저장한다.
		if (mCurrRunType == eEngineRunType::EDITOR_MODE && mNextRunType == eEngineRunType::GAME_MODE)
		{
			SaveManager::Save();
		}
		switch (mNextRunType)
		{
		case eEngineRunType::GAME_MODE:
			ViewportManager::Start();
			break;
		case eEngineRunType::EDITOR_MODE:
			ResourceManager::Reload();
			// TODO : 모든 리소스 로드, JSON 리로드 등
			break;
		default:
			break;
		}
		mCurrRunType = mNextRunType;
	}
}

void GameManager::SetRunType(eEngineRunType _runType)
{
	mNextRunType = _runType;
}

World* GameManager::GetCurrentWorld()
{
	if (!ViewportManager::GetActiveViewport()) return nullptr;
	if (!ViewportManager::GetActiveViewport()->GetWorldManager()) return nullptr;
	return ViewportManager::GetActiveViewport()->GetWorldManager()->GetActiveWorld();
}
