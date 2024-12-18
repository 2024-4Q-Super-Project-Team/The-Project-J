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
	BOOL Initialize();
public:
	virtual BOOL SetUpDisplay(Display::IWindow** _ppWindow) override;
private:
	Graphics::GraphicsDevice* mGraphicsDevice;
	Graphics::Renderer* mRenderer;

	std::array<Graphics::ConstantBuffer*, static_cast<UINT>(Graphics::eCBufferType::SIZE)> mConstantBuffArr;
	std::unordered_map<HWND, Graphics::RenderTarget*> mRenderTargetTable;
	// 메쉬를 모아서 한번에 렌더링..?

public:
	inline virtual Graphics::GraphicsDevice* GetDevice() override { return mGraphicsDevice; }
	inline virtual Graphics::Renderer* GetRenderer() override { return mRenderer; }
};

