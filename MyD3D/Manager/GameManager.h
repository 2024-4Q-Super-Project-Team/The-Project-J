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

enum class eEngineRunType
{
	NONE,
	GAME_MODE,
	EDITOR_MODE,
};



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
	void GameRun();
	void EditorRun();
	void Finalization();
private:
	void UpdateGame();
public:
	static void SetRunType(eEngineRunType _runType);
public:
    static World* GetCurrentWorld();
public:
	static auto	GetRunType() { return mCurrRunType; }
	static auto	GetFixedUpdateTick() { return mFixedUpdateTick; }
	static auto	GetApplication()     { return mApplication; }
	static auto	GetPhysicsManager()  { return mPhysicsManager; }
private:
	volatile static eEngineRunType    mCurrRunType;
	volatile static eEngineRunType    mNextRunType;
	static Application*		 mApplication;
	static PhysicsManager*	 mPhysicsManager;
	static ComponentManager* mComponentManager;
	static float			 mFixedUpdateTick;
};