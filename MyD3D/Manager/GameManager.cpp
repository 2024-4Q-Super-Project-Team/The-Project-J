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

float				GameManager::mFixedUpdateTick = 0.02f;
Application* GameManager::mApplication = nullptr;
PhysicsManager* GameManager::mPhysicsManager = nullptr;
ComponentManager* GameManager::mComponentManager = nullptr;

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

		ViewportManager::Run();

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
	mComponentManager->Finalization();
	mPhysicsManager->Finalization();
	SAFE_DELETE(mPhysicsManager);
}

World* GameManager::GetCurrentWorld()
{
	return ViewportManager::GetActiveViewport()->GetWorldManager()->GetActiveWorld();
}
