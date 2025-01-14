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
	// ����ؼ� �������̵�� ���
	// Application�� Initialize �۾� ���� ȣ��
	virtual BOOL _CALLBACK	OnPreInitialize() = 0;
	// Application�� Initialize �۾� �Ϸ� �Ŀ� ȣ��
	virtual BOOL _CALLBACK	OnPostInitialize() = 0;
	// Application�� Finalization �۾� ���� ȣ��
	virtual void _CALLBACK	OnPreFinalization() = 0;
	// Application�� Finalization �۾� �Ϸ� �Ŀ� ȣ��
	virtual void _CALLBACK	OnPostFinalization() = 0;
public:
	// ����ؼ� �������̵�� ��� (������ �̱���)
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
	// ���ø����̼ǿ� ���� �˸��� ������.
	inline void				ShutDown() { IsRun = false; }
	inline HINSTANCE		GetHInstance() { return mHInstance; }
private:
	HINSTANCE				mHInstance;
	bool					IsRun;
protected:
	GameManager* const		mGameManager;
public:
};