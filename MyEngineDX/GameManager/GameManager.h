#pragma once
#include "Viewport/ViewportManager.h"
#include "Graphics/GraphicsManager.h"

class PhysicsManager;
namespace Engine
{
	class Application;
}

class GameManager
	: public Engine::IEngineCycle
{
public:
	GameManager(Engine::Application* _pApp);
	~GameManager() = default;
public:
	BOOL Initialize();
	void Run();
	void Finalization();
public:
	virtual void FixedUpdate() override;
	virtual void PreUpdate() override;
	virtual void Update() override;
	virtual void PostUpdate() override;
	virtual void PreRender() override;
	virtual void Render(GraphicsManager* _graphicsManager) override;
	virtual void PostRender() override;
public:
public:
	static float GetFixedUpdateTick() { return mFixedUpdateTick; }
	Engine::Application* GetApplication()	  { return mApplication; }
	ViewportManager*	 GetViewportManager() { return mViewportManager; }
	IGraphicsManager*	 GetGraphicsManager() { return mGraphicsManager; }
	static PhysicsManager*	 GetPhysicsManager()	  { return mPhysicsManager; }
private:
	Engine::Application*	 mApplication;
	ViewportManager*		 mViewportManager;
	GraphicsManager*		 mGraphicsManager;
	static PhysicsManager*			 mPhysicsManager;
	static float mFixedUpdateTick;
};