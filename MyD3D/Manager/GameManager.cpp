#include "pch.h"
#include "Application/Application.h"
#include "GameManager.h"
#include "ComponentManager.h"
#include "World/WorldManager.h"
#include "Resource/ResourceManager.h"
#include "Graphics/GraphicsManager.h"
#include "ViewportScene/ViewportManager.h"
#include "Physics/PhysicsManager.h"
#include "ViewportScene/ViewportScene.h"

float				GameManager::mFixedUpdateTick = 0.02f;
Application*		GameManager::mApplication = nullptr;
ViewportManager*	GameManager::mViewportManager = nullptr;
PhysicsManager*		GameManager::mPhysicsManager = nullptr;
ComponentManager*	GameManager::mComponentManager = nullptr;

GameManager::GameManager(Application* _pApplication)
{
	mApplication		= _pApplication;
	mViewportManager	= new ViewportManager();
	mPhysicsManager		= new PhysicsManager();
	mComponentManager	= new ComponentManager();
}

GameManager::~GameManager()
{
}

BOOL GameManager::Initialize()
{
	Time::Initialize();
    Input::Initialize();
    GraphicsManager::Initialize();
    ResourceManager::Initialize();
    mViewportManager->Initialize();
	mComponentManager->Initialize();
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
		Tick();
		FixedUpdate();
		PreUpdate();
		Update();
		PostUpdate();
		PreRender();
		Render();
		PostRender();
        Input::Update();
	}
}

void GameManager::Finalization()
{
    GraphicsManager::Finalization();
    ResourceManager::Finalization();
    mViewportManager->Finalization();
	mComponentManager->Finalization();
    SAFE_DELETE(mViewportManager);
    SAFE_DELETE(mPhysicsManager);
}

void GameManager::Tick()
{
	if (mApplication)
		mApplication->OnTick();
	if (mViewportManager)
		mViewportManager->Tick();
}

void GameManager::FixedUpdate()
{
	static float counter = 0.0f;

	counter += Time::GetUnScaledDeltaTime();
	while (counter >= mFixedUpdateTick)
	{
		counter -= mFixedUpdateTick;
		if (mApplication)
			mApplication->OnFixedUpdate();
		if (mViewportManager)
			mViewportManager->FixedUpdate();
	}
}

void GameManager::PreUpdate()
{
	if (mApplication)
		mApplication->OnPreUpdate();
	if (mViewportManager)
		mViewportManager->PreUpdate();
}

void GameManager::Update()
{
	if (mApplication)
		mApplication->OnUpdate();
	if (mViewportManager)
		mViewportManager->Update();
}

void GameManager::PostUpdate()
{
	if (mApplication)
		mApplication->OnPostUpdate();
	if (mViewportManager)
		mViewportManager->PostUpdate();
}

void GameManager::PreRender()
{
	if (mApplication)
		mApplication->OnPreRender();
	if (mViewportManager)
		mViewportManager->PreRender();
}

void GameManager::Render()
{
	if (mApplication)
		mApplication->OnRender();
	if (mViewportManager)
		mViewportManager->Render();
}

void GameManager::PostRender()
{
	if (mApplication)
		mApplication->OnPostRender();
	if (mViewportManager)
		mViewportManager->PostRender();
}

ViewportScene* GameManager::GetCurrentViewport()
{
    return mViewportManager->GetActiveViewport();;
}

World* GameManager::GetCurrentWorld()
{
    return GetCurrentViewport()->GetWorldManager()->GetActiveWorld();
}
