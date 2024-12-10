#pragma once

namespace Display
{
	class IWindow;
}

class IGraphicsManager
{
public:
	virtual BOOL SetUpDisplay(Display::IWindow** _ppWindow) = 0;
public:
	virtual Graphics::GraphicsDevice* GetDevice() = 0;
	virtual	Graphics::Renderer* GetRenderer() = 0;
};

class GraphicsManager 
	: public IGraphicsManager
{
public:
	GraphicsManager();
	~GraphicsManager();
public:
	void BeginRender();
	void EndRender();
public:
	virtual BOOL SetUpDisplay(Display::IWindow** _ppWindow) override;
private:
	Graphics::GraphicsDevice* mGraphicsDevice;
	Graphics::Renderer*		  mRenderer;

	std::unordered_map<HWND, Graphics::RenderTarget*> mRenderTargetTable;
public:
	inline virtual Graphics::GraphicsDevice* GetDevice() override { return mGraphicsDevice; }
	inline virtual Graphics::Renderer* GetRenderer() override { return mRenderer; }
};

