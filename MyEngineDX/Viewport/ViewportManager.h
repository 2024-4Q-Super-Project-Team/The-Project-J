#pragma once
#include "Viewport/Viewport.h"

namespace Display
{
	class IWindow;
}

class ViewportManager : public Engine::IEngineCycle
{
public:
	explicit ViewportManager(GameManager* _pGameManager);
	virtual ~ViewportManager();
	ViewportManager(ViewportManager&) = delete;
	ViewportManager& operator=(const ViewportManager&) = delete;
	ViewportManager(ViewportManager&&) noexcept = default;
	ViewportManager& operator=(ViewportManager&&) noexcept = default;
public:
	virtual void FixedUpdate() override;
	virtual void PreUpdate() override;
	virtual void Update() override;
	virtual void PostUpdate() override;
	virtual void PreRender() override;
	virtual void Render(GraphicsManager* _graphicsManager) override;
	virtual void PostRender() override;
public:
	ViewportScene* CreateViewport(Display::IWindow* _pWindow);
private:
	std::vector<ViewportScene*> mViewports;
	GameManager* const mGameManager;
};