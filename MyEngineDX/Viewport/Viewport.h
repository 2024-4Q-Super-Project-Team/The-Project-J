#pragma once

class WorldManager;
class ViewportScene;

namespace Graphics
{
	class GraphicsDevice;
	class Renderer;
	class RenderTarget;
}

class ViewportScene
	: public Engine::IEngineCycle
{
public:
	explicit ViewportScene(Display::IWindow* _pWindow, Graphics::RenderTarget* _pRenderTarget);
	virtual ~ViewportScene();
public:
	virtual void Tick() override final;
	virtual void FixedUpdate() override final;
	virtual void PreUpdate() override final;
	virtual void Update() override final;
	virtual void PostUpdate() override final;
	virtual void PreRender() override final;
	virtual void Render(GraphicsManager* _graphicsManager) override final;
	virtual void PostRender() override final;
protected:
	WorldManager*				mWorldManager;
	Display::IWindow*			mWindow;
	Graphics::RenderTarget*		mRenderTarget;
public:
	Display::IWindow*	GetIWindow() { return mWindow; }
	WorldManager*		GetWorldManager() { return mWorldManager; }
};

