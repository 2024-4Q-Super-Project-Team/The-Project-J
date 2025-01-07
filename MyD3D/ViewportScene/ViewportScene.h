#pragma once
#include "Interface/ICycleHandler.h"
#include "Helper/Entity/Entity.h"

namespace Display
{
	class IWindow;
}
class WorldManager;
class GraphicsManager;
class D3DGraphicsRenderTarget;

// 게임 윈도우 하나에 대한 클래스
// 윈도우, 렌더타겟, 월드매니저의 객체를 포함하고 있다.
class ViewportScene
	: public Engine::Entity
	, public Engine::ICycleHandler
{
public:
	explicit ViewportScene(std::wstring_view _name, Display::IWindow* _pWindow, D3DGraphicsRenderTarget* _pRenderTarget);
	virtual ~ViewportScene();
	ViewportScene(ViewportScene&) = delete;
	ViewportScene& operator=(const ViewportScene&) = delete;
	ViewportScene(ViewportScene&&) noexcept = default;
	ViewportScene& operator=(ViewportScene&&) noexcept = default;
public:
	virtual void Tick()			override;
	virtual void FixedUpdate()	override;
	virtual void PreUpdate()	override;
	virtual void Update()		override;
	virtual void PostUpdate()	override;
	virtual void PreRender()	override;
	virtual void Render()       override;
	virtual void PostRender()	override;
protected:
	WorldManager* mWorldManager;
	Display::IWindow* mWindow;
	D3DGraphicsRenderTarget* mRenderTarget;
public:
	inline auto* GetIWindow() { return mWindow; }
	inline auto* GetWorldManager() { return mWorldManager; }
};

