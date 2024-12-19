#pragma once

namespace Engine
{
	class Application;
}
class ViewportManager;
class GraphicsManager;
class PhysicsManager;
class ResourceManager;
class GraphicsManager;

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
	static float				GetFixedUpdateTick() { return mFixedUpdateTick; }
	static Engine::Application* GetApplication()	 { return mApplication; }
	static ViewportManager*		GetViewportManager() { return mViewportManager; }
	static GraphicsManager*		GetGraphicsManager() { return mGraphicsManager; }
	static PhysicsManager*		GetPhysicsManager()  { return mPhysicsManager; }
	static ResourceManager*		GetResourceManager() { return mResourceManager; }
private:
	static Engine::Application* mApplication;
	static ViewportManager*		mViewportManager;
	static GraphicsManager*		mGraphicsManager;
	static PhysicsManager*		mPhysicsManager;
	static ResourceManager*		mResourceManager;
	// FixedUpdate аж╠Б
	static float				mFixedUpdateTick;
};