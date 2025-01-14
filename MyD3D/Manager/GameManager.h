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
    static World*           GetCurrentWorld();
public:
	static float			GetFixedUpdateTick() { return mFixedUpdateTick; }
	static Application*		GetApplication()     { return mApplication; }
	static PhysicsManager*	GetPhysicsManager()  { return mPhysicsManager; }
private:
	static Application*		 mApplication;
	static PhysicsManager*	 mPhysicsManager;
	static ComponentManager* mComponentManager;
	static float			 mFixedUpdateTick;
};