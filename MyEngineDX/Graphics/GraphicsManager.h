#pragma once

namespace Display
{
	class IWindow;
}

namespace Graphics
{
	class GraphicsDevice;
	class RenderTarget;
	class Renderer;
	class Buffer;
	class ConstantBuffer;
	class VertexBuffer;
	class IndexBuffer;
	enum class eCBufferType;
}

class GraphicsManager
{
public:
	GraphicsManager();
	~GraphicsManager();
public:
	BOOL Initialize();
public:
	BOOL SetUpDisplay(Display::IWindow** _ppWindow);
private:
	Graphics::GraphicsDevice* mGraphicsDevice;
	Graphics::Renderer* mRenderer;

	std::array<Graphics::ConstantBuffer*, static_cast<UINT>(Graphics::eCBufferType::SIZE)> mConstantBuffArr;
	std::unordered_map<HWND, Graphics::RenderTarget*> mRenderTargetTable;
	// 메쉬를 모아서 한번에 렌더링..?

public:
	Graphics::GraphicsDevice* GetDevice() { return mGraphicsDevice; }
	Graphics::Renderer* GetRenderer() { return mRenderer; }
};

