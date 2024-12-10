#include "pch.h"
#include "GameManager.h"
#include "Application/Application.h"

GameManager::GameManager(Engine::Application* _pApp)
	: mApplication(_pApp)
	, mGraphicsManager(new GraphicsManager())
	, mViewportManager(new ViewportManager(this))
	, mFixedUpdateTick(0.02f)
{
}

BOOL GameManager::Initialize()
{
	Time::Initialize();
	return TRUE;
}

void GameManager::Run()
{
	MSG msg;
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		//Input::InputMessegeProcess(msg);
	}
	else
	{
		Time::Update();
		//Input::Update();
		FixedUpdate();
		PreUpdate();
		Update();
		PostUpdate();
		PreRender();
		Render(mGraphicsManager);
		PostRender();
		//Input::Reset();
	}
}

void GameManager::Finalization()
{
	SAFE_DELETE(mGraphicsManager)
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

void GameManager::Render(GraphicsManager* _graphicsManager)
{
	if (mApplication)
		mApplication->OnRender();
	if (mViewportManager)
		mViewportManager->Render(_graphicsManager);
}

void GameManager::PostRender()
{
	if (mApplication)
		mApplication->OnPostRender();
	if (mViewportManager)
		mViewportManager->PostRender();
}
