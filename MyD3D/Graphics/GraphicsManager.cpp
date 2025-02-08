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
D3DGraphicsRasterizerState* GraphicsManager::mRasterizerStateArray[RASTERIZER_STATE_TYPE_COUNT];
D3DGraphicsDepthStencilState* GraphicsManager::mDepthStecilStateArray[DEPTHSTENCIL_STATE_TYPE_COUNT];

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
    // DepthStencil State
    //////////////////////////////////////////
    InitDepthStencilState();

    //////////////////////////////////////////
    // Raterizer State
    //////////////////////////////////////////
    InitRasterizerState();

    DebugRenderer::Initialize();

    return TRUE;
}

void GraphicsManager::Finalization()
{
    SAFE_RELEASE_ARRAY(mCBufferArray);
    SAFE_RELEASE_ARRAY(mVertexShaderArray);
    SAFE_RELEASE_ARRAY(mPixelShaderArray);
    SAFE_RELEASE_ARRAY(mGeometryShaderArray);
    SAFE_RELEASE_ARRAY(mSamplerStateArray);
    SAFE_RELEASE_ARRAY(mBlendStateArray);
    SAFE_RELEASE_ARRAY(mRasterizerStateArray);
    DebugRenderer::Finalization();
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
        mCBufferArray[slot]->SetBindStage(eShaderStage::ALL);
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
    // Particle
    {
        UINT slot = static_cast<UINT>(eCBufferType::ParticleSize);
        bufDesc.ByteWidth = sizeof(ParticleSizeCBuffer);
        mCBufferArray[slot] = new D3DGraphicsConstantBuffer(&bufDesc, nullptr);
        mCBufferArray[slot]->SetBindSlot(slot);
        mCBufferArray[slot]->SetBindStage(eShaderStage::GS);
        mCBufferArray[slot]->Bind();
    }
    // Outline
    {
        UINT slot = static_cast<UINT>(eCBufferType::Outline);
        bufDesc.ByteWidth = sizeof(OutlineCBuffer);
        mCBufferArray[slot] = new D3DGraphicsConstantBuffer(&bufDesc, nullptr);
        mCBufferArray[slot]->SetBindSlot(slot);
        mCBufferArray[slot]->SetBindStage(eShaderStage::ALL);
        mCBufferArray[slot]->Bind();
    }
    // Fog
    {
        UINT slot = static_cast<UINT>(eCBufferType::Fog);
        bufDesc.ByteWidth = sizeof(FogCBuffer);
        mCBufferArray[slot] = new D3DGraphicsConstantBuffer(&bufDesc, nullptr);
        mCBufferArray[slot]->SetBindSlot(slot);
        mCBufferArray[slot]->SetBindStage(eShaderStage::ALL);
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
        mVertexShaderArray[(UINT)eVertexShaderType::GRID] = new D3DGraphicsVertexShader(L"resource/shader/Grid_VS.cso");
        mVertexShaderArray[(UINT)eVertexShaderType::OUTLINE] = new D3DGraphicsVertexShader(L"resource/shader/Outline_VS.cso");
    
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
        mPixelShaderArray[(UINT)ePixelShaderType::GRID] = new D3DGraphicsPixelShader(L"resource/shader/Grid_PS.cso");
        mPixelShaderArray[(UINT)ePixelShaderType::OUTLINE] = new D3DGraphicsPixelShader(L"resource/shader/Outline_PS.cso");

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

void GraphicsManager::InitRasterizerState()
{
    // None_Culling
    {
        UINT slot = static_cast<UINT>(eRasterizerStateType::NONE_CULLING);
        D3D11_RASTERIZER_DESC rasterDesc;
        ZeroMemory(&rasterDesc, sizeof(rasterDesc));
        rasterDesc.FillMode = D3D11_FILL_SOLID; // 폴리곤을 채우는 모드
        rasterDesc.CullMode = D3D11_CULL_NONE; // 백페이스 컬링 비활성화
        rasterDesc.FrontCounterClockwise = false; // 시계 방향 폴리곤을 프론트로 간주
        rasterDesc.DepthClipEnable = true;
        mRasterizerStateArray[slot] = new D3DGraphicsRasterizerState(&rasterDesc);
    }
    // BackFace_Culling
    {
        UINT slot = static_cast<UINT>(eRasterizerStateType::BACKFACE_CULLING);
        D3D11_RASTERIZER_DESC rasterDesc;
        ZeroMemory(&rasterDesc, sizeof(rasterDesc));
        rasterDesc.FillMode = D3D11_FILL_SOLID; // 폴리곤을 채우는 모드
        rasterDesc.CullMode = D3D11_CULL_BACK; // 백페이스 컬링 비활성화
        rasterDesc.FrontCounterClockwise = false; // 시계 방향 폴리곤을 프론트로 간주
        rasterDesc.DepthClipEnable = true;
        mRasterizerStateArray[slot] = new D3DGraphicsRasterizerState(&rasterDesc);
    }
    // FrontFace_Culling
    {
        UINT slot = static_cast<UINT>(eRasterizerStateType::FRONTFACE_CULLING);
        D3D11_RASTERIZER_DESC rasterDesc;
        ZeroMemory(&rasterDesc, sizeof(rasterDesc));
        rasterDesc.FillMode = D3D11_FILL_SOLID;     // 폴리곤을 채우는 모드
        rasterDesc.CullMode = D3D11_CULL_FRONT;     // 백페이스 컬링 비활성화
        rasterDesc.FrontCounterClockwise = false;   // 시계 방향 폴리곤을 프론트로 간주
        rasterDesc.DepthClipEnable = true;
        mRasterizerStateArray[slot] = new D3DGraphicsRasterizerState(&rasterDesc);
    }
}

void GraphicsManager::InitDepthStencilState()
{
    // Default State
    {
        UINT slot = static_cast<UINT>(eDepthStencilStateType::DEFAULT);
        D3D11_DEPTH_STENCIL_DESC DssDesc;
        ZeroMemory(&DssDesc, sizeof(DssDesc));

        DssDesc.DepthEnable = true;
        DssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        DssDesc.DepthFunc = D3D11_COMPARISON_LESS;
        DssDesc.StencilEnable = true;
        DssDesc.StencilReadMask = 0xFF;
        DssDesc.StencilWriteMask = 0xFF;
        DssDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        DssDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        DssDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        DssDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        DssDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        DssDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        DssDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        DssDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        mDepthStecilStateArray[slot] = new D3DGraphicsDepthStencilState(&DssDesc);
    }

    // Only write Stencil State
    {
        UINT slot = static_cast<UINT>(eDepthStencilStateType::STENCIL_WRITE);
        D3D11_DEPTH_STENCIL_DESC swDssDesc;
        ZeroMemory(&swDssDesc, sizeof(swDssDesc));

        swDssDesc.DepthEnable = true;
        swDssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        swDssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        swDssDesc.StencilEnable = true;
        swDssDesc.StencilReadMask = 0x00;
        swDssDesc.StencilWriteMask = 0xFF;
        swDssDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        swDssDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        swDssDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
        swDssDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        swDssDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        swDssDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        swDssDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        swDssDesc.BackFace.StencilFunc = swDssDesc.FrontFace.StencilFunc;

        mDepthStecilStateArray[slot] = new D3DGraphicsDepthStencilState(&swDssDesc);
    }

    // Only Read Stencil State
    {
        UINT slot = static_cast<UINT>(eDepthStencilStateType::STENCIL_READ);
        D3D11_DEPTH_STENCIL_DESC srDssDesc;
        ZeroMemory(&srDssDesc, sizeof(srDssDesc));

        srDssDesc.DepthEnable = true;
        srDssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        srDssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        srDssDesc.StencilEnable = true;
        srDssDesc.StencilReadMask = 0xFF;
        srDssDesc.StencilWriteMask = 0x00;
        srDssDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        srDssDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        srDssDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
        srDssDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
        srDssDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        srDssDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        srDssDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        srDssDesc.BackFace.StencilFunc = srDssDesc.FrontFace.StencilFunc;

        mDepthStecilStateArray[slot] = new D3DGraphicsDepthStencilState(&srDssDesc);
    }
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
    TexDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    TexDesc.SampleDesc.Count = 1;
    TexDesc.SampleDesc.Quality = 0;
    TexDesc.Usage = D3D11_USAGE_DEFAULT;
    TexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    TexDesc.CPUAccessFlags = 0;
    TexDesc.MiscFlags = 0;

    DSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    DSVDesc.Texture2D.MipSlice = 0;
    DSVDesc.Flags = 0;

    SRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
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

DebugDrawState*         DebugRenderer::mStates = nullptr;
DebugDrawBatch*         DebugRenderer::mBatch = nullptr;
DebugDrawEffect*        DebugRenderer::mEffect = nullptr;
DebugDrawInputLayout*   DebugRenderer::mInputLayout = nullptr;

BOOL DebugRenderer::Initialize()
{
    mStates = new DebugDrawState(D3DGraphicsDevice::GetDevice());
    mBatch = new DebugDrawBatch(D3DGraphicsRenderer::GetDevicecontext());
    mEffect = new DebugDrawEffect(D3DGraphicsDevice::GetDevice());
    // 버텍스 컬러 True
    mEffect->SetVertexColorEnabled(true);
    // 버텍스셰이더 정보
    void const* shaderByteCode;
    size_t byteCodeLength;
    mEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

    D3DGraphicsDevice::GetDevice()->CreateInputLayout(
        VertexPositionColor::InputElements,
        VertexPositionColor::InputElementCount,
        shaderByteCode,
        byteCodeLength,
        &mInputLayout
    );
    return TRUE;
}

void DebugRenderer::Finalization()
{
    SAFE_DELETE(mStates)
    SAFE_DELETE(mBatch)
    SAFE_DELETE(mEffect)
}

void DebugRenderer::UpdateWorld(const Matrix& _world)
{
    mEffect->SetWorld(_world);
    mEffect->Apply(D3DGraphicsRenderer::GetDevicecontext());
}

void DebugRenderer::UpdateViewProjection(const Matrix& _view, const Matrix& _projection)
{
    mEffect->SetView(_view);
    mEffect->SetProjection(_projection);
    // Apply를 호출하지 않으면 적용되지 않는다....
    mEffect->Apply(D3DGraphicsRenderer::GetDevicecontext());
}

void DebugRenderer::BeginDraw()
{
    D3DGraphicsRenderer::SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    D3DGraphicsRenderer::GetDevicecontext()->RSSetState(mStates->Wireframe());
    D3DGraphicsRenderer::GetDevicecontext()->OMSetBlendState(mStates->Opaque(), nullptr, 0xFFFFFFFF);
    D3DGraphicsRenderer::GetDevicecontext()->OMSetDepthStencilState(mStates->DepthNone(), 0);

    D3DGraphicsRenderer::GetDevicecontext()->IASetInputLayout(mInputLayout);
    mEffect->Apply(D3DGraphicsRenderer::GetDevicecontext());
    mBatch->Begin();
}

void DebugRenderer::EndDraw()
{
    mBatch->End();
    GraphicsManager::GetBlendState(eBlendStateType::DEFAULT)->Bind();
    GraphicsManager::GetRasterizerState(eRasterizerStateType::NONE_CULLING)->Bind();
    GraphicsManager::GetDepthStencilState(eDepthStencilStateType::DEFAULT)->Bind();
    // 상수버퍼도 디버그드로우가 덮어 씌우므로 다시 바인딩
    GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->Bind();
    D3DGraphicsRenderer::SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
