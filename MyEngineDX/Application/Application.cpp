#include "pch.h"
#include "Application.h"
#include "../Window/IDisplay.h"

namespace Engine
{
	Application::Application(HINSTANCE _hInstance)
		: mHInstance(_hInstance), IsShutdown(false)
		, mGameManager(new GameManager(this))
	{
	}
	Application::~Application()
	{
		if(mGameManager)
			delete mGameManager;
	}
	BOOL Application::Initialize()
	{
		if (FALSE == OnPreInitialize()) return FALSE;

		if (S_OK != Display::CreateIDisplayDevice(mHInstance, &mDisplayDevice)) return FALSE;

		if (FALSE == mGameManager->Initialize()) return FALSE;

		if (FALSE == OnPostInitialize()) return FALSE;

		return TRUE;
	}
	void Application::Run()
	{
		while (!IsShutdown)
		{
			mGameManager->Run();
		}
	}
	void Application::Finalization()
	{
		OnPreFinalization();

		mGameManager->Finalization();

		OnPostFinalization();
	}
	HRESULT Application::CreateWindowDisplay(Display::WindowDesc* _pWinDesc, Display::IWindow** _ppWindow)
	{
		HRESULT hr = mDisplayDevice->CreateWindowDisplay(_pWinDesc, _ppWindow);
		if (S_OK == hr)
		{

		}
		return hr;
	}
	HRESULT Application::DestroyDisplay(HWND _hWnd)
	{
		HRESULT hr = mDisplayDevice->DestroyDisplay(_hWnd);
		return hr;
	}
	HRESULT Application::DestroyDisplay(Display::IDisplay** _ppDisplay)
	{
		HRESULT hr = mDisplayDevice->DestroyDisplay(_ppDisplay);
		return hr;
	}
}