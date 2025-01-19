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
private:
    static void InitConstantBuffer();
    static void InitShader();
    static void InitSamplerState();
    static void InitBlendState();
    static void InitDebugDraw();
public:
    static inline auto GetConstantBuffer(eCBufferType _type) { return mCBufferArray[(UINT)_type]; }
    static inline auto GetSamplerState(eSamplerStateType _type) { return mSamplerStateArray[(UINT)_type]; }
    static inline auto GetVertexShader(eVertexShaderType _type) { return mVertexShaderArray[(UINT)_type]; }
    static inline auto GetPixelShader(ePixelShaderType _type) { return mPixelShaderArray[(UINT)_type]; }
    static inline auto GetBlendState(eBlendType _type) { return mBlendStateArray[(UINT)_type]; }
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
    static D3DGraphicsSamplerState*     mSamplerStateArray[SAMPLER_STATE_TYPE_COUNT];
    static D3DGraphicsBlendState*       mBlendStateArray[BLEND_TYPE_COUNT];

    //Debug Draw
#ifdef _DEBUG
public:
    static void SetDebugViewProjection(Matrix _view, Matrix _projection);
    static void DebugDrawBegin();
    static void DebugDrawEnd();
    static class PrimitiveBatch<VertexPositionColor>* GetBatch() { return mBatch.get(); }
private:
    static std::unique_ptr<class CommonStates> mStates;
    static std::unique_ptr<class PrimitiveBatch<VertexPositionColor>> mBatch;
    static std::unique_ptr<class BasicEffect> mEffect;
    static ID3D11InputLayout* mLayout;
    static ID3D11DepthStencilState* originalDepthState;
    static UINT stencilRef;
#endif
};