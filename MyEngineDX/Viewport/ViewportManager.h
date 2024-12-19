#pragma once
#include "Viewport.h"

namespace Display
{
	class IWindow;
}
class ViewportScene;
class GameManager;
class GraphicsManager;

class ViewportManager : public Engine::IEngineCycle
{
public:
	explicit ViewportManager();
	virtual ~ViewportManager();
	ViewportManager(ViewportManager&) = delete;
	ViewportManager& operator=(const ViewportManager&) = delete;
	ViewportManager(ViewportManager&&) noexcept = default;
	ViewportManager& operator=(ViewportManager&&) noexcept = default;
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
	ViewportScene* CreateViewport(Display::IWindow* _pWindow);
	ViewportScene* GetActiveViewport() { return  mActiveViewport; }
private:
	ViewportScene*				mActiveViewport;
	std::vector<ViewportScene*> mViewports;
};