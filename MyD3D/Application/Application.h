#pragma once
#include "Interface/ICycleEventHandler.h"
#include "Interface/IWindowMessageHandler.h"

class GameManager;
class ViewportScene;

class Application 
	: public Engine::ICycleEventHandler
	, public Engine::IWindowMessageHandler
{
public:
	explicit Application(HINSTANCE _hInstance);
	virtual ~Application();
	Application(Application&) = delete;
	Application& operator=(const Application&) = delete;
	Application(Application&&) noexcept = default;
	Application& operator=(Application&&) noexcept = default;
public:
	BOOL Initialize();
	void Run();
	void Finalization();
protected:
	// 상속해서 오버라이드로 사용
	// Application의 Initialize 작업 전에 호출
	virtual BOOL _CALLBACK	OnPreInitialize() = 0;
	// Application의 Initialize 작업 완료 후에 호출
	virtual BOOL _CALLBACK	OnPostInitialize() = 0;
	// Application의 Finalization 작업 전에 호출
	virtual void _CALLBACK	OnPreFinalization() = 0;
	// Application의 Finalization 작업 완료 후에 호출
	virtual void _CALLBACK	OnPostFinalization() = 0;
public:
	// 상속해서 오버라이드로 사용 (지금은 미구현)
	virtual void _CALLBACK	OnTick() override {};
	virtual void _CALLBACK	OnFixedUpdate() override {};
	virtual void _CALLBACK	OnPreUpdate() override {};
	virtual void _CALLBACK	OnUpdate() override {};
	virtual void _CALLBACK	OnPostUpdate() override {};
	virtual void _CALLBACK	OnPreRender() override {};
	virtual void _CALLBACK	OnRender() override {};
	virtual void _CALLBACK	OnPostRender() override {};
public:
	virtual void _CALLBACK	OnWindowMessage(ViewportScene* _pViewport, UINT _msg, WPARAM _wParam, LPARAM _lParam) override {};
public:
	// 어플리케이션에 종료 알림을 보낸다.
	inline void				ShutDown() { IsRun = false; }
	inline HINSTANCE		GetHInstance() { return mHInstance; }
private:
	HINSTANCE				mHInstance;
	bool					IsRun;
protected:
	GameManager* const		mGameManager;
public:
};