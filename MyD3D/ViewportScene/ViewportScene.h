#pragma once
#include "Interface/ICycleHandler.h"
#include "Helper/Entity/Entity.h"

namespace Display
{
	class IWindow;
}
class WorldManager;
class GraphicsManager;
class D3DHwndRenderTarget;

// 게임 윈도우 하나에 대한 클래스
// 윈도우, 렌더타겟, 월드매니저의 객체를 포함하고 있다.
class ViewportScene
	: public Engine::Entity
	, public Engine::ICycleHandler
{
public:
	explicit ViewportScene(std::wstring_view _name, Display::IWindow* _pWindow, D3DHwndRenderTarget* _pSwapChain);
	virtual ~ViewportScene();
	ViewportScene(ViewportScene&) = delete;
	ViewportScene& operator=(const ViewportScene&) = delete;
	ViewportScene(ViewportScene&&) noexcept = default;
	ViewportScene& operator=(ViewportScene&&) noexcept = default;
public:
	void Start();
public:
	virtual void Tick()			override;
	virtual void FixedUpdate()	override;
	virtual void PreUpdate()	override;
	virtual void Update()		override;
	virtual void PostUpdate()	override;
	virtual void PreRender()	override;
	virtual void Render()       override;
	virtual void PostRender()	override;

	virtual void EditorUpdate()	override;
	virtual void EditorRender()	override;
protected:
	WorldManager* mWorldManager;
	Display::IWindow* mWindow;
	D3DHwndRenderTarget* mSwapChain;
public:
	inline auto* GetIWindow() { return mWindow; }
	inline auto* GetWorldManager() { return mWorldManager; }
	inline auto* GetSwapChain() { return mSwapChain; }
public:
	// 공유할 리소스(윈도우마다 사이즈별로 하나씩 만들어주고, 뷰포트를 다르게 해서 사용)
	std::weak_ptr<D3DGraphicsViewport>		mSharedViewport;
	std::weak_ptr<D3DBitmapRenderTarget>	mSharedRenderTarget;
	std::weak_ptr<D3DBitmapRenderTarget>	mSharedDeferredRenderTarget;
	std::shared_ptr<D3DGraphicsViewport>	GetMainViewport();
	std::shared_ptr<D3DBitmapRenderTarget>	GetMainRenderTarget();
	std::shared_ptr<D3DBitmapRenderTarget>	GetDeferredRenderTarget();

	void ResizeSharedResourceView();
};

