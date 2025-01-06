#pragma once

class IGraphicsDevice;
class IGraphicsRenderer;
class IGraphicsRenderTarget;

class IBuffer;
class IVertexBuffer;
class IIndexBuffer;
class IConstantBuffer;

class SamplerState;
class VertexShader;
class PixelShader;

class GraphicsManager
{
public:
    GraphicsManager() = default;
	~GraphicsManager() = default;
public:
    static BOOL Initialize();
    static void Finalization();
public: // Bind 및 Update 리소스
    static HRESULT BindViewport(ViewportDesc* _pViewportDesc);
    template <typename CBUFFER_TYPE>
    static HRESULT UpdateConstantBuffer(eCBufferType _cBufferType, const CBUFFER_TYPE* _srcBuffer);
private:
    static IConstantBuffer* mCBufferArray[CBUFFER_SIZE];
    static std::vector<std::shared_ptr<SamplerState>> mSamplerState;
    static std::vector<std::shared_ptr<VertexShader>> mVertexShader;
    static std::vector<std::shared_ptr<PixelShader>>  mPixelShader;
private:
	static IGraphicsDevice*	    mDevice;
	static IGraphicsRenderer*	mRenderer;
public:
	static IGraphicsDevice*     GetDevice() { return mDevice; }
	static IGraphicsRenderer*   GetRenderer() { return mRenderer; }
};

template<typename CBUFFER_TYPE>
inline HRESULT GraphicsManager::UpdateConstantBuffer(eCBufferType _cBufferType, const CBUFFER_TYPE* _srcBuffer)
{
    CBUFFER_TYPE cb;
    size_t bufferSize = sizeof(CBUFFER_TYPE);
    memcpy(&cb, _srcBuffer, bufferSize);
    return mRenderer->UpdateSubResource(
        mCBufferArray[static_cast<UINT>(_cBufferType)],
        0, nullptr,
        &cb,
        0, 0
    );
}
