#pragma once

class D3DGraphicsDevice;
class D3DGraphicsRenderer;
class D3DGraphicsRenderTarget;

class D3DGraphicsBuffer;
class D3DGraphicsVertexBuffer;
class D3DGraphicsIndexBuffer;
class D3DGraphicsConstantBuffer;

class D3DGraphicsSamplerState;
class D3DGraphicsVertexShader;
class D3DGraphicsPixelShader;

class GraphicsManager
{
public:
    GraphicsManager() = default;
	~GraphicsManager() = default;
public:
    static BOOL Initialize();
    static void Finalization();

    static inline auto GetConstantBuffer(eCBufferType _type) { return mCBufferArray[(UINT)_type]; }
    static inline auto GetSamplerState(eSamplerStateType _type) { return mSamplerStates[(UINT)_type]; }
    static inline auto GetVertexShader(eVertexShaderType _type) { return mVertexShaders[(UINT)_type]; }
    static inline auto GetPixelShader(ePixelShaderType _type) { return mPixelShaders[(UINT)_type]; }
private:
    static D3DGraphicsConstantBuffer*   mCBufferArray[CBUFFER_TYPE_COUNT];
    static D3DGraphicsSamplerState*     mSamplerStates[SAMPLER_STATE_TYPE_COUNT];
    static D3DGraphicsVertexShader*     mVertexShaders[VS_TYPE_COUNT];
    static D3DGraphicsPixelShader*      mPixelShaders[PS_TYPE_COUNT];
};