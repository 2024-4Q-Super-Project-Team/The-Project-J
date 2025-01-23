#pragma once

class D3DGraphicsDevice;
class D3DGraphicsRenderer;
class D3DGraphicsRenderTarget;

class D3DGraphicsBuffer;
class D3DGraphicsVertexBuffer;
class D3DGraphicsIndexBuffer;
class D3DGraphicsConstantBuffer;
class D3DGraphicsSamplerState;
class D3DGraphicsBlendState;
class D3DGraphicsRasterizerState;
class D3DGraphicsVertexShader;
class D3DGraphicsPixelShader;
class D3DGraphicsGeometryShader;

class GraphicsManager
{
public:
    GraphicsManager() = default;
    ~GraphicsManager() = default;
public:
    static BOOL Initialize();
    static void Finalization();
private:
    static void InitConstantBuffer();
    static void InitShader();
    static void InitSamplerState();
    static void InitBlendState();
    static void InitRasterizerState();
    static void InitDepthStencilState();
public:
    static inline auto GetConstantBuffer(eCBufferType _type) { return mCBufferArray[(UINT)_type]; }
    static inline auto GetSamplerState(eSamplerStateType _type) { return mSamplerStateArray[(UINT)_type]; }
    static inline auto GetVertexShader(eVertexShaderType _type) { return mVertexShaderArray[(UINT)_type]; }
    static inline auto GetGeometryShader(eGeometryShaderType _type) { return mGeometryShaderArray[(UINT)_type]; }
    static inline auto GetPixelShader(ePixelShaderType _type) { return mPixelShaderArray[(UINT)_type]; }
    static inline auto GetBlendState(eBlendStateType _type) { return mBlendStateArray[(UINT)_type]; }
    static inline auto GetRasterizerState(eRasterizerStateType _type) { return mRasterizerStateArray[(UINT)_type]; }
    static inline auto GetDepthStencilState(eDepthStencilStateType _type) { return mDepthStecilStateArray[(UINT)_type]; }
public:
    static std::pair<D3DGraphicsRTV*, D3DGraphicsSRV*> CreateDefaultRenderTargetView(UINT _width, UINT _height);
    static std::pair<D3DGraphicsDSV*, D3DGraphicsSRV*> CreateDefaultDepthStencilView(UINT _width, UINT _height);
    static std::pair<D3DGraphicsRTV*, D3DGraphicsSRV*> CreateAlbedoGBuffer(UINT _width, UINT _height);
    static std::pair<D3DGraphicsRTV*, D3DGraphicsSRV*> CreateNormalGBuffer(UINT _width, UINT _height);
    static std::pair<D3DGraphicsRTV*, D3DGraphicsSRV*> CreateMaterialGBuffer(UINT _width, UINT _height);
    static std::pair<D3DGraphicsRTV*, D3DGraphicsSRV*> CreateEmessiveGBuffer(UINT _width, UINT _height);
    static std::pair<D3DGraphicsRTV*, D3DGraphicsSRV*> CreateWorldPosGBuffer(UINT _width, UINT _height);
private:
    static D3DGraphicsConstantBuffer*   mCBufferArray[CBUFFER_TYPE_COUNT];
    static D3DGraphicsVertexShader*     mVertexShaderArray[VS_TYPE_COUNT];
    static D3DGraphicsPixelShader*      mPixelShaderArray[PS_TYPE_COUNT];
    static D3DGraphicsGeometryShader*   mGeometryShaderArray[GS_TYPE_COUNT];
    static D3DGraphicsSamplerState*     mSamplerStateArray[SAMPLER_STATE_TYPE_COUNT];
    static D3DGraphicsBlendState*       mBlendStateArray[BLEND_STATE_TYPE_COUNT];
    static D3DGraphicsRasterizerState*  mRasterizerStateArray[RASTERIZER_STATE_TYPE_COUNT];
    static D3DGraphicsDepthStencilState* mDepthStecilStateArray[DEPTHSTENCIL_STATE_TYPE_COUNT];
};

namespace DirectX
{
    namespace DX11
    {
        class CommonStates;
        class BasicEffect;
    }
}

using DebugDrawState = CommonStates;
using DebugDrawBatch = PrimitiveBatch<VertexPositionColor>;
using DebugDrawEffect = BasicEffect;
using DebugDrawInputLayout = ID3D11InputLayout;

class DebugRenderer
{
public:
    static BOOL Initialize();
    static void Finalization();
public:
    inline static auto GetState() { return mStates; }
    inline static auto GetBatch() { return mBatch; }
    inline static auto GetEffect() { return mEffect; }
public:
    static void SetViewProjection(const Matrix& _view, const Matrix& _projection);
public:
    static void BeginDraw();
    static void EndDraw();
public:
    static DebugDrawState*         mStates;
    static DebugDrawBatch*         mBatch;
    static DebugDrawEffect*        mEffect;
    static DebugDrawInputLayout*   mInputLayout;
};