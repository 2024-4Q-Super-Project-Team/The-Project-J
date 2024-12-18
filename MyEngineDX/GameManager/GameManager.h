#pragma once
#include "Viewport/ViewportManager.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/ResourceManager.h"
#include "Physics/PhysicsManager.h"

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
	virtual void Tick() override;
	virtual void FixedUpdate() override;
	virtual void PreUpdate() override;
	virtual void Update() override;
	virtual void PostUpdate() override;
	virtual void PreRender() override;
	virtual void Render(GraphicsManager* _graphicsManager) override;
	virtual void PostRender() override;
public:
	static float GetFixedUpdateTick() { return mFixedUpdateTick; }
	static Engine::Application* GetApplication() { return mApplication; }
	static ViewportManager* GetViewportManager() { return mViewportManager; }
	static IGraphicsManager* GetGraphicsManager() { return mGraphicsManager; }
	static PhysicsManager* GetPhysicsManager() { return mPhysicsManager; }
	static ResourceManager* GetResourceManager() { return mResourceManager; }
private:
	static Engine::Application* mApplication;
	static ViewportManager* mViewportManager;
	static GraphicsManager* mGraphicsManager;
	static PhysicsManager* mPhysicsManager;
	static ResourceManager* mResourceManager;
	static float mFixedUpdateTick;
};