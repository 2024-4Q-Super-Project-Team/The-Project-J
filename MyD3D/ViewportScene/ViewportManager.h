#pragma once
#include "Interface/ICycleHandler.h"

namespace Display
{
	class IDisplayDevice;
	class IWindow;
    struct WindowDesc;
}
class ViewportScene;
class GraphicsManager;

class ViewportManager
	: public Engine::ICycleHandler
{
public:
	explicit ViewportManager();
	virtual ~ViewportManager();
	ViewportManager(ViewportManager&) = delete;
	ViewportManager& operator=(const ViewportManager&) = delete;
	ViewportManager(ViewportManager&&) noexcept = default;
	ViewportManager& operator=(ViewportManager&&) noexcept = default;
public:
    BOOL Initialize();
    void Finalization();
public:
	virtual void Tick()			override;
	virtual void FixedUpdate()	override;
	virtual void PreUpdate()	override;
	virtual void Update()		override;
	virtual void PostUpdate()	override;
	virtual void PreRender()	override;
	virtual void Render() override;
	virtual void PostRender()	override;
public:
	ViewportScene* CreateViewportScene(Display::WindowDesc* _pWinDesc);
	ViewportScene* GetViewportSceneFromWindowName(std::wstring _winName);
	ViewportScene* GetViewportSceneFromHwnd(HWND _hWnd);
	ViewportScene* GetActiveViewport() { return  mActiveViewport; }
private:
    Display::IDisplayDevice*    mDisplayDevice;
	ViewportScene*              mActiveViewport;
	std::vector<ViewportScene*> mViewportScenes;
};