#pragma once

namespace Display
{
	class IWindow;
}

namespace Graphics
{
	class Buffer;
	enum class eCBufferType;
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
	Graphics::Renderer* mRenderer;

	std::array<Graphics::Buffer*, static_cast<UINT>(Graphics::eCBufferType::SIZE)> mConstantBuffArr;
	std::unordered_map<HWND, Graphics::RenderTarget*> mRenderTargetTable;
public:
	inline virtual Graphics::GraphicsDevice* GetDevice() override { return mGraphicsDevice; }
	inline virtual Graphics::Renderer* GetRenderer() override { return mRenderer; }
};

