#pragma once
#include "Interface/ICycleHandler.h"
#include "Interface/ICycleEventHandler.h"

class Application;
class PhysicsManager;
class ViewportManager;
class GraphicsManager;
class ResourceManager;
class ComponentManager;
class ViewportScene;
class World;

class GameManager
	: public Engine::ICycleHandler
{
public:
	explicit GameManager(Application* _pApplication);
	virtual ~GameManager();
	GameManager(GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;
	GameManager(GameManager&&) noexcept = default;
	GameManager& operator=(GameManager&&) noexcept = default;
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
	virtual void Render() override;
	virtual void PostRender() override;
public:
    static ViewportScene*   GetCurrentViewport();
    static World*           GetCurrentWorld();
public:
	static float			GetFixedUpdateTick() { return mFixedUpdateTick; }
	static Application*		GetApplication()     { return mApplication; }
	static ViewportManager* GetViewportManager() { return mViewportManager; }
	static PhysicsManager*	GetPhysicsManager()  { return mPhysicsManager; }
private:
	static Application*		 mApplication;
	static ViewportManager*  mViewportManager;
	static PhysicsManager*	 mPhysicsManager;
	static ComponentManager* mComponentManager;
	static float			 mFixedUpdateTick;
};