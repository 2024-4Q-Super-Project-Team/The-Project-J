#include "pch.h"
#include "Application.h"
#include "Manager/GameManager.h"

Application::Application(HINSTANCE _hInstance)
	: mGameManager(new GameManager(this))
	, mHInstance(_hInstance)
	, IsRun(true)
{
}

Application::~Application()
{
	delete mGameManager;
}

BOOL Application::Initialize()
{
	if (FALSE == OnPreInitialize()) return FALSE;

	if (FALSE == mGameManager->Initialize()) return FALSE;

	if (FALSE == OnPostInitialize()) return FALSE;
	return TRUE;
}

void Application::Run()
{
	while (IsRun)
	{
		mGameManager->Run();
	}
}

void Application::Finalization()
{
}
