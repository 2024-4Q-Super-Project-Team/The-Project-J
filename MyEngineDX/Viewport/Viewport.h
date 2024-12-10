#pragma once
#include "World/WorldManager.h"

class ViewportScene;


class ViewportScene
	: public Engine::IEngineCycle
{
public:
	explicit ViewportScene(Display::IWindow* _pWindow, Graphics::RenderTarget* _pRenderTarget);
	virtual ~ViewportScene();
public:
	virtual void FixedUpdate() override final;
	virtual void PreUpdate() override final;
	virtual void Update() override final;
	virtual void PostUpdate() override final;
	virtual void PreRender() override final;
	virtual void Render(GraphicsManager* _graphicsManager) override final;
	virtual void PostRender() override final;
protected:
	WorldManager*			mWorldManager;
	Display::IWindow*		mWindow;
	Graphics::RenderTarget* mRenderTarget;
public:
	inline Display::IWindow* GetIWindow() { return mWindow; }
	inline WorldManager* GetWorldManager() { return mWorldManager; }
};

