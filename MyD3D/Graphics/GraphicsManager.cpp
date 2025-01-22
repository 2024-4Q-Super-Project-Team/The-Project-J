#include "pch.h"
#include "GraphicsManager.h"
#include "Manager/GameManager.h"
#include "Resource/ResourceManager.h"

#include "Graphics/Extra/DebugDraw.h"
#include "Directxtk/VertexTypes.h"
#include "Directxtk/Effects.h"
#include "Directxtk/CommonStates.h"

D3DGraphicsConstantBuffer* GraphicsManager::mCBufferArray[CBUFFER_TYPE_COUNT];
D3DGraphicsSamplerState* GraphicsManager::mSamplerStateArray[SAMPLER_STATE_TYPE_COUNT];
D3DGraphicsVertexShader* GraphicsManager::mVertexShaderArray[VS_TYPE_COUNT];
D3DGraphicsGeometryShader* GraphicsManager::mGeometryShaderArray[VS_TYPE_COUNT];
D3DGraphicsPixelShader* GraphicsManager::mPixelShaderArray[PS_TYPE_COUNT];
D3DGraphicsBlendState* GraphicsManager::mBlendStateArray[BLEND_STATE_TYPE_COUNT];

std::unique_ptr<CommonStates> GraphicsManager::mStates;
std::unique_ptr<PrimitiveBatch<VertexPositionColor>> GraphicsManager::mBatch;
std::unique_ptr<BasicEffect> GraphicsManager::mEffect;
ID3D11InputLayout* GraphicsManager::mLayout;
ID3D11DepthStencilState* GraphicsManager::originalDepthState;
UINT GraphicsManager::stencilRef;

using namespace DirectX::DX11;

BOOL GraphicsManager::Initialize()
{
    D3DGraphicsDevice::Initialize();
    D3DGraphicsRenderer::Initialize();

    //////////////////////////////////////////
    // Constant Buffer
    //////////////////////////////////////////
    InitConstantBuffer();

    //////////////////////////////////////////
    // Vertex, Pixel Shader
    //////////////////////////////////////////
    InitShader();
    
    //////////////////////////////////////////
    // Sampler State
    //////////////////////////////////////////
    InitSamplerState();

    //////////////////////////////////////////
    // Blend State
    //////////////////////////////////////////
    InitBlendState();

    //////////////////////////////////////////
    // Debug Draw
    //////////////////////////////////////////
    InitDebugDraw();

    mBlendStateArray[(UINT)eBlendStateType::DEFAULT]->Bind();

    return TRUE;
}

void GraphicsManager::Finalization()
{
    SAFE_RELEASE_ARRAY(mCBufferArray);
    SAFE_RELEASE_ARRAY(mSamplerStateArray);
    SAFE_RELEASE_ARRAY(mVertexShaderArray);
    SAFE_RELEASE_ARRAY(mGeometryShaderArray);
    SAFE_RELEASE_ARRAY(mPixelShaderArray);
    SAFE_RELEASE_ARRAY(mBlendStateArray);
    D3DGraphicsDevice::Finalization();
    D3DGraphicsRenderer::Finalization();
}

void GraphicsManager::InitConstantBuffer()
{
    D3D11_BUFFER_DESC bufDesc = {};
    D3D11_SUBRESOURCE_DATA SubData = {};
    bufDesc.Usage = D3D11_USAGE_DEFAULT;
    bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufDesc.CPUAccessFlags = 0;
    // Transform
    {
        UINT slot = static_cast<UINT>(eCBufferType::Transform);
        bufDesc.ByteWidth = sizeof(TransformCBuffer);
        mCBufferArray[slot] = new D3DGraphicsConstantBuffer(&bufDesc, nullptr);
        mCBufferArray[slot]->SetBindSlot(slot);
        mCBufferArray[slot]->SetBindStage(eShaderStage::VS);
        mCBufferArray[slot]->Bind();
    }
    // Material
    {
        UINT slot = static_cast<UINT>(eCBufferType::Material);
        bufDesc.ByteWidth = sizeof(MaterialCBuffer);
        mCBufferArray[slot] = new D3DGraphicsConstantBuffer(&bufDesc, nullptr);
        mCBufferArray[slot]->SetBindSlot(slot);
        mCBufferArray[slot]->SetBindStage(eShaderStage::PS);
        mCBufferArray[slot]->Bind();
    }
    // Camera
    {
        UINT slot = static_cast<UINT>(eCBufferType::Camera);
        bufDesc.ByteWidth = sizeof(CameraCBuffer);
        mCBufferArray[slot] = new D3DGraphicsConstantBuffer(&bufDesc, nullptr);
        mCBufferArray[slot]->SetBindSlot(slot);
        mCBufferArray[slot]->SetBindStage(eShaderStage::PS);
        mCBufferArray[slot]->Bind();
    }
    // BoneMatrix
    {
        UINT slot = static_cast<UINT>(eCBufferType::BoneMatrix);
        bufDesc.ByteWidth = sizeof(BoneMatrixCBuffer);
        mCBufferArray[slot] = new D3DGraphicsConstantBuffer(&bufDesc, nullptr);
        mCBufferArray[slot]->SetBindSlot(slot);
        mCBufferArray[slot]->SetBindStage(eShaderStage::VS);
        mCBufferArray[slot]->Bind();
    }
    // Light
    {
        UINT slot = static_cast<UINT>(eCBufferType::Light);
        bufDesc.ByteWidth = sizeof(LightCBuffer);
        mCBufferArray[slot] = new D3DGraphicsConstantBuffer(&bufDesc, nullptr);
        mCBufferArray[slot]->SetBindSlot(slot);
        mCBufferArray[slot]->SetBindStage(eShaderStage::PS);
        mCBufferArray[slot]->Bind();
    }
    {
        UINT slot = static_cast<UINT>(eCBufferType::ParticleSize);
        bufDesc.ByteWidth = sizeof(ParticleSizeCBuffer);
        mCBufferArray[slot] = new D3DGraphicsConstantBuffer(&bufDesc, nullptr);
        mCBufferArray[slot]->SetBindSlot(slot);
        mCBufferArray[slot]->SetBindStage(eShaderStage::GS);
        mCBufferArray[slot]->Bind();
    }
}

void GraphicsManager::InitShader()
{
    {
        mVertexShaderArray[(UINT)eVertexShaderType::STANDARD] = new D3DGraphicsVertexShader(L"resource/shader/Standard_VS.cso");
        mVertexShaderArray[(UINT)eVertexShaderType::SKYBOX] = new D3DGraphicsVertexShader(L"resource/shader/SkyBox_VS.cso");
        mVertexShaderArray[(UINT)eVertexShaderType::SPRITE] = new D3DGraphicsVertexShader(L"resource/shader/Sprite_VS.cso");
        mVertexShaderArray[(UINT)eVertexShaderType::SHADOW] = new D3DGraphicsVertexShader(L"resource/shader/Shadow_VS.cso");
        mVertexShaderArray[(UINT)eVertexShaderType::PARTICLE] = new D3DGraphicsVertexShader(L"resource/shader/Particle_VS.cso");

    }
    {
        mGeometryShaderArray[(UINT)eGeometryShaderType::PARTICLE] = new D3DGraphicsGeometryShader(L"resource/shader/Particle_GS.cso");
    }
    {
        mPixelShaderArray[(UINT)ePixelShaderType::BLINN_PHONG] = new D3DGraphicsPixelShader(L"resource/shader/BlinnPhong_PS.cso");
        mPixelShaderArray[(UINT)ePixelShaderType::FOWARD_PBR] = new D3DGraphicsPixelShader(L"resource/shader/FowardPBR_PS.cso");
        mPixelShaderArray[(UINT)ePixelShaderType::DEFERRED_PBR] = new D3DGraphicsPixelShader(L"resource/shader/DeferredPBR_PS.cso");
        mPixelShaderArray[(UINT)ePixelShaderType::SKYBOX] = new D3DGraphicsPixelShader(L"resource/shader/SkyBox_PS.cso");
        mPixelShaderArray[(UINT)ePixelShaderType::SPRITE] = new D3DGraphicsPixelShader(L"resource/shader/Sprite_PS.cso");
        mPixelShaderArray[(UINT)ePixelShaderType::G_BUFFER] = new D3DGraphicsPixelShader(L"resource/shader/GBuffer_PS.cso");
        mPixelShaderArray[(UINT)ePixelShaderType::PARTICLE] = new D3DGraphicsPixelShader(L"resource/shader/Particle_PS.cso");

    }
}

void GraphicsManager::InitSamplerState()
{
    
    D3DGraphicsSamplerState* pSamplerState;
    {
        D3D11_SAMPLER_DESC sampDesc = {};
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sampDesc.MinLOD = 0;
        sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

        UINT slot = static_cast<UINT>(eSamplerStateType::LINEAR_WRAP);
        pSamplerState = new D3DGraphicsSamplerState(&sampDesc);
        pSamplerState->SetBindSlot(slot);
        pSamplerState->SetBindStage(eShaderStage::PS);
        mSamplerStateArray[slot] = pSamplerState;
        pSamplerState->Bind();
    }
    {
        D3D11_SAMPLER_DESC sampDesc = {};
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampDesc.MaxAnisotropy = 1;
        sampDesc.MinLOD = 0;
        sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

        UINT slot = static_cast<UINT>(eSamplerStateType::LINEAR_CLAMP);
        pSamplerState = new D3DGraphicsSamplerState(&sampDesc);
        pSamplerState->SetBindSlot(slot);
        pSamplerState->SetBindStage(eShaderStage::PS);
        mSamplerStateArray[slot] = pSamplerState;
        pSamplerState->Bind();
    }
    {
        D3D11_SAMPLER_DESC sampDesc = {};
        sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
        sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;


        UINT slot = static_cast<UINT>(eSamplerStateType::LINEAR_COMPARISON);
        pSamplerState = new D3DGraphicsSamplerState(&sampDesc);
        pSamplerState->SetBindSlot(slot);
        pSamplerState->SetBindStage(eShaderStage::PS);
        mSamplerStateArray[slot] = pSamplerState;
        pSamplerState->Bind();
    }
}

void GraphicsManager::InitBlendState()
{
    // (불투명처리)
    {
        UINT slot = static_cast<UINT>(eBlendStateType::DEFAULT);
        D3D11_BLEND_DESC blendDesc = {};
        blendDesc.AlphaToCoverageEnable = FALSE;  // 알파 투명도 사용 안 함
        blendDesc.IndependentBlendEnable = FALSE; // 모든 렌더 타겟에 동일한 설정 적용
        for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
        {
            blendDesc.RenderTarget[i].BlendEnable = FALSE;                                  // 블렌딩 비활성화
            blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA ;                    // 소스 그대로
            blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_ZERO;                         // 대상에 영향 없음
            blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;                         // 기본 합산 연산 (영향 없음)
            blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;                      // 알파 채널에 영향 없음
            blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;                    // 알파 채널에 영향 없음
            blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;                    // 알파 합산 (영향 없음)
            blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // RGBA 모두 기록
        }
        mBlendStateArray[slot] = new D3DGraphicsBlendState(&blendDesc);
    }
    // Alpha Blending (투명처리)
    {
        UINT slot = static_cast<UINT>(eBlendStateType::ALPHA);
        D3D11_BLEND_DESC blendDesc = {};
        blendDesc.AlphaToCoverageEnable = TRUE;  // 알파 투명도 사용 함
        blendDesc.IndependentBlendEnable = FALSE; // 모든 렌더 타겟에 동일한 설정 적용
        for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
        {
            blendDesc.RenderTarget[i].BlendEnable = TRUE;                                   // 블렌딩 활성화
            blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;                     // 소스 알파 기반
            blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;                // 대상 (1 - 알파) 기반
            blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;                         // 소스 + 대상
            blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;                      // 알파: 소스를 그대로 사용
            blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;                    // 알파: 대상 무시
            blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;                    // 알파 합산
            blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // RGBA 모두 기록
        }
        mBlendStateArray[slot] = new D3DGraphicsBlendState(&blendDesc);
    }
}

void GraphicsManager::InitDebugDraw()
{
    mStates = std::make_unique<CommonStates>(D3DGraphicsDevice::GetDevice());
    mBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(D3DGraphicsRenderer::GetDevicecontext());

    mEffect = std::make_unique<BasicEffect>(D3DGraphicsDevice::GetDevice());
    mEffect->SetVertexColorEnabled(true);

    void const* shaderByteCode;
    size_t byteCodeLength;

    mEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

    D3DGraphicsDevice::GetDevice()->CreateInputLayout(
        VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
        shaderByteCode, byteCodeLength,
        &mLayout);
}

std::pair<D3DGraphicsRTV*, D3DGraphicsSRV*> GraphicsManager::CreateDefaultRenderTargetView(UINT _width, UINT _height)
{
    D3D11_TEXTURE2D_DESC TexDesc = {};
    D3D11_RENDER_TARGET_VIEW_DESC RTVDesc = {};
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};

    ZeroMemory(&TexDesc, sizeof(TexDesc));
    ZeroMemory(&RTVDesc, sizeof(RTVDesc));
    ZeroMemory(&SRVDesc, sizeof(SRVDesc));

    TexDesc.Width = _width;
    TexDesc.Height = _height;
    TexDesc.MipLevels = 1;
    TexDesc.ArraySize = 1;
    TexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    TexDesc.SampleDesc.Count = 1;
    TexDesc.SampleDesc.Quality = 0;
    TexDesc.Usage = D3D11_USAGE_DEFAULT;
    TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    TexDesc.CPUAccessFlags = 0;
    TexDesc.MiscFlags = 0;

    RTVDesc.Format = TexDesc.Format;
    RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    RTVDesc.Texture2D.MipSlice = 0;

    SRVDesc.Format = TexDesc.Format;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MostDetailedMip = 0;
    SRVDesc.Texture2D.MipLevels = 1;

    D3DGraphicsTexture2D* pTexture = new D3DGraphicsTexture2D(&TexDesc);
    D3DGraphicsRTV* pRTV = new D3DGraphicsRTV(pTexture, &RTVDesc);
    D3DGraphicsSRV* pSRV = new D3DGraphicsSRV(pTexture, &SRVDesc);

    return std::make_pair(pRTV, pSRV);
}

std::pair<D3DGraphicsDSV*, D3DGraphicsSRV*> GraphicsManager::CreateDefaultDepthStencilView(UINT _width, UINT _height)
{
    D3D11_TEXTURE2D_DESC TexDesc = {};
    D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};

    ZeroMemory(&TexDesc, sizeof(TexDesc));
    ZeroMemory(&DSVDesc, sizeof(DSVDesc));
    ZeroMemory(&SRVDesc, sizeof(SRVDesc));

    TexDesc.Width = _width;
    TexDesc.Height = _height;
    TexDesc.MipLevels = 1;
    TexDesc.ArraySize = 1;
    TexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    TexDesc.SampleDesc.Count = 1;
    TexDesc.SampleDesc.Quality = 0;
    TexDesc.Usage = D3D11_USAGE_DEFAULT;
    TexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    TexDesc.CPUAccessFlags = 0;
    TexDesc.MiscFlags = 0;

    DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
    DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    DSVDesc.Texture2D.MipSlice = 0;

    SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MostDetailedMip = 0;
    SRVDesc.Texture2D.MipLevels = 1;

    D3DGraphicsTexture2D* pTexture = new D3DGraphicsTexture2D(&TexDesc);
    D3DGraphicsDSV* pDSV = new D3DGraphicsDSV(pTexture, &DSVDesc);
    D3DGraphicsSRV* pSRV = new D3DGraphicsSRV(pTexture, &SRVDesc);

    return std::make_pair(pDSV, pSRV);
}

std::pair<D3DGraphicsRTV*, D3DGraphicsSRV*> GraphicsManager::CreateAlbedoGBuffer(UINT _width, UINT _height)
{
    D3D11_TEXTURE2D_DESC TexDesc = {};
    D3D11_RENDER_TARGET_VIEW_DESC RTVDesc = {};
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};

    ZeroMemory(&TexDesc, sizeof(TexDesc));
    ZeroMemory(&RTVDesc, sizeof(RTVDesc));
    ZeroMemory(&SRVDesc, sizeof(SRVDesc));

    TexDesc.Width = _width;
    TexDesc.Height = _height;
    TexDesc.MipLevels = 1;
    TexDesc.ArraySize = 1;
    TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    TexDesc.SampleDesc.Count = 1;
    TexDesc.SampleDesc.Quality = 0;
    TexDesc.Usage = D3D11_USAGE_DEFAULT;
    TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    TexDesc.CPUAccessFlags = 0;
    TexDesc.MiscFlags = 0;
    
    RTVDesc.Format = TexDesc.Format;
    RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    RTVDesc.Texture2D.MipSlice = 0;
    
    SRVDesc.Format = TexDesc.Format;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MostDetailedMip = 0;
    SRVDesc.Texture2D.MipLevels = 1;

    D3DGraphicsTexture2D* pTexture = new D3DGraphicsTexture2D(&TexDesc);
    D3DGraphicsRTV* pRTV = new D3DGraphicsRTV(pTexture, &RTVDesc);
    D3DGraphicsSRV* pSRV = new D3DGraphicsSRV(pTexture, &SRVDesc);

    pSRV->SetBindStage(eShaderStage::PS);
    pSRV->SetBindSlot(12);

    return std::make_pair(pRTV, pSRV);
}

std::pair<D3DGraphicsRTV*, D3DGraphicsSRV*> GraphicsManager::CreateNormalGBuffer(UINT _width, UINT _height)
{
    D3D11_TEXTURE2D_DESC TexDesc = {};
    D3D11_RENDER_TARGET_VIEW_DESC RTVDesc = {};
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};

    ZeroMemory(&TexDesc, sizeof(TexDesc));
    ZeroMemory(&RTVDesc, sizeof(RTVDesc));
    ZeroMemory(&SRVDesc, sizeof(SRVDesc));

    TexDesc.Width = _width;
    TexDesc.Height = _height;
    TexDesc.MipLevels = 1;
    TexDesc.ArraySize = 1;
    TexDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
    TexDesc.SampleDesc.Count = 1;
    TexDesc.SampleDesc.Quality = 0;
    TexDesc.Usage = D3D11_USAGE_DEFAULT;
    TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    TexDesc.CPUAccessFlags = 0;
    TexDesc.MiscFlags = 0;

    RTVDesc.Format = TexDesc.Format;
    RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    RTVDesc.Texture2D.MipSlice = 0;

    SRVDesc.Format = TexDesc.Format;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MostDetailedMip = 0;
    SRVDesc.Texture2D.MipLevels = 1;

    D3DGraphicsTexture2D* pTexture = new D3DGraphicsTexture2D(&TexDesc);
    D3DGraphicsRTV* pRTV = new D3DGraphicsRTV(pTexture, &RTVDesc);
    D3DGraphicsSRV* pSRV = new D3DGraphicsSRV(pTexture, &SRVDesc);

    pSRV->SetBindStage(eShaderStage::PS);
    pSRV->SetBindSlot(13);

    return std::make_pair(pRTV, pSRV);
}

std::pair<D3DGraphicsRTV*, D3DGraphicsSRV*> GraphicsManager::CreateMaterialGBuffer(UINT _width, UINT _height)
{
    D3D11_TEXTURE2D_DESC TexDesc = {};
    D3D11_RENDER_TARGET_VIEW_DESC RTVDesc = {};
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};

    ZeroMemory(&TexDesc, sizeof(TexDesc));
    ZeroMemory(&RTVDesc, sizeof(RTVDesc));
    ZeroMemory(&SRVDesc, sizeof(SRVDesc));

    TexDesc.Width = _width;
    TexDesc.Height = _height;
    TexDesc.MipLevels = 1;
    TexDesc.ArraySize = 1;
    TexDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
    TexDesc.SampleDesc.Count = 1;
    TexDesc.SampleDesc.Quality = 0;
    TexDesc.Usage = D3D11_USAGE_DEFAULT;
    TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    TexDesc.CPUAccessFlags = 0;
    TexDesc.MiscFlags = 0;

    RTVDesc.Format = TexDesc.Format;
    RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    RTVDesc.Texture2D.MipSlice = 0;

    SRVDesc.Format = TexDesc.Format;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MostDetailedMip = 0;
    SRVDesc.Texture2D.MipLevels = 1;

    D3DGraphicsTexture2D* pTexture = new D3DGraphicsTexture2D(&TexDesc);
    D3DGraphicsRTV* pRTV = new D3DGraphicsRTV(pTexture, &RTVDesc);
    D3DGraphicsSRV* pSRV = new D3DGraphicsSRV(pTexture, &SRVDesc);

    pSRV->SetBindStage(eShaderStage::PS);
    pSRV->SetBindSlot(14);

    return std::make_pair(pRTV, pSRV);
}

std::pair<D3DGraphicsRTV*, D3DGraphicsSRV*> GraphicsManager::CreateEmessiveGBuffer(UINT _width, UINT _height)
{
    D3D11_TEXTURE2D_DESC TexDesc = {};
    D3D11_RENDER_TARGET_VIEW_DESC RTVDesc = {};
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};

    ZeroMemory(&TexDesc, sizeof(TexDesc));
    ZeroMemory(&RTVDesc, sizeof(RTVDesc));
    ZeroMemory(&SRVDesc, sizeof(SRVDesc));

    TexDesc.Width = _width;
    TexDesc.Height = _height;
    TexDesc.MipLevels = 1;
    TexDesc.ArraySize = 1;
    TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    TexDesc.SampleDesc.Count = 1;
    TexDesc.SampleDesc.Quality = 0;
    TexDesc.Usage = D3D11_USAGE_DEFAULT;
    TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    TexDesc.CPUAccessFlags = 0;
    TexDesc.MiscFlags = 0;

    RTVDesc.Format = TexDesc.Format;
    RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    RTVDesc.Texture2D.MipSlice = 0;

    SRVDesc.Format = TexDesc.Format;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MostDetailedMip = 0;
    SRVDesc.Texture2D.MipLevels = 1;

    D3DGraphicsTexture2D* pTexture = new D3DGraphicsTexture2D(&TexDesc);
    D3DGraphicsRTV* pRTV = new D3DGraphicsRTV(pTexture, &RTVDesc);
    D3DGraphicsSRV* pSRV = new D3DGraphicsSRV(pTexture, &SRVDesc);

    pSRV->SetBindStage(eShaderStage::PS);
    pSRV->SetBindSlot(15);

    return std::make_pair(pRTV, pSRV);
}

std::pair<D3DGraphicsRTV*, D3DGraphicsSRV*> GraphicsManager::CreateWorldPosGBuffer(UINT _width, UINT _height)
{
    D3D11_TEXTURE2D_DESC TexDesc = {};
    D3D11_RENDER_TARGET_VIEW_DESC RTVDesc = {};
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};

    ZeroMemory(&TexDesc, sizeof(TexDesc));
    ZeroMemory(&RTVDesc, sizeof(RTVDesc));
    ZeroMemory(&SRVDesc, sizeof(SRVDesc));

    TexDesc.Width = _width;
    TexDesc.Height = _height;
    TexDesc.MipLevels = 1;
    TexDesc.ArraySize = 1;
    TexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    TexDesc.SampleDesc.Count = 1;
    TexDesc.SampleDesc.Quality = 0;
    TexDesc.Usage = D3D11_USAGE_DEFAULT;
    TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    TexDesc.CPUAccessFlags = 0;
    TexDesc.MiscFlags = 0;

    RTVDesc.Format = TexDesc.Format;
    RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    RTVDesc.Texture2D.MipSlice = 0;

    SRVDesc.Format = TexDesc.Format;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MostDetailedMip = 0;
    SRVDesc.Texture2D.MipLevels = 1;

    D3DGraphicsTexture2D* pTexture = new D3DGraphicsTexture2D(&TexDesc);
    D3DGraphicsRTV* pRTV = new D3DGraphicsRTV(pTexture, &RTVDesc);
    D3DGraphicsSRV* pSRV = new D3DGraphicsSRV(pTexture, &SRVDesc);

    pSRV->SetBindStage(eShaderStage::PS);
    pSRV->SetBindSlot(16);

    return std::make_pair(pRTV, pSRV);
}

void GraphicsManager::SetDebugViewProjection(Matrix _view, Matrix _projection)
{
    mEffect->SetView(_view);
    mEffect->SetProjection(_projection);
}

void GraphicsManager::DebugDrawBegin()
{
    // 기존 깊이 상태를 저장
    D3DGraphicsRenderer::GetDevicecontext()->OMGetDepthStencilState(&originalDepthState, &stencilRef);
    //debug draw
    D3DGraphicsRenderer::GetDevicecontext()->OMSetBlendState(mStates->Opaque(), nullptr, 0xFFFFFFFF);
    D3DGraphicsRenderer::GetDevicecontext()->OMSetDepthStencilState(mStates->DepthNone(), 0);
    D3DGraphicsRenderer::GetDevicecontext()->RSSetState(mStates->CullNone());

    mEffect->Apply(D3DGraphicsRenderer::GetDevicecontext());
    D3DGraphicsRenderer::GetDevicecontext()->IASetInputLayout(mLayout);
    mBatch->Begin();
}

void GraphicsManager::DebugDrawEnd()
{
    D3DGraphicsRenderer::GetDevicecontext()->OMSetDepthStencilState(originalDepthState, stencilRef);
    mBatch->End();
    D3DGraphicsRenderer::GetDevicecontext()->OMSetDepthStencilState(originalDepthState, stencilRef);
}
