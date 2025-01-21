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
{
public:
	explicit ViewportManager() = default;
	virtual ~ViewportManager() = default;
	ViewportManager(ViewportManager&) = delete;
	ViewportManager& operator=(const ViewportManager&) = delete;
	ViewportManager(ViewportManager&&) noexcept = default;
	ViewportManager& operator=(ViewportManager&&) noexcept = default;
public:
	static BOOL Initialize();
	static void GameRun();
	static void EditorRun();
	static void Finalization();
public:
	static void Start();
public:
	static void Tick();
	static void FixedUpdate();
	static void PreUpdate();
	static void Update();
	static void PostUpdate();
	static void PreRender();
	static void Render();
	static void PostRender();

	static void EditorUpdate();
	static void EditorRender();
public:
	static ViewportScene* CreateViewportScene(Display::WindowDesc* _pWinDesc);
	static ViewportScene* GetViewportSceneFromWindowName(std::wstring _winName);
	static ViewportScene* GetViewportSceneFromHwnd(HWND _hWnd);
	static ViewportScene* GetActiveViewport() { return  mActiveViewport; }
private:
	static ViewportScene* mActiveViewport;
	static Display::IDisplayDevice* mDisplayDevice;
	static std::vector<ViewportScene*> mViewportScenes;
public:
	static LRESULT CALLBACK WinProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
};