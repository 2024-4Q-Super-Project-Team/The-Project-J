#include "pch.h"
#include "GraphicsManager.h"
#include "Manager/GameManager.h"
#include "Resource/ResourceManager.h"

#include "Graphics/Core/D3DGraphicsDevice.h"
#include "Graphics/Core/D3DGraphicsRenderer.h"
#include "Graphics/Core/D3DGraphicsRenderTarget.h"
#include "Graphics/Core/D3DGraphicsSwapChain.h"

#include "Graphics/GPUResource/D3DGraphicsBuffer.h"
#include "Graphics/GPUResource/D3DGraphicsSamplerState.h"
#include "Graphics/GPUResource/D3DGraphicsShader.h"
#include "Graphics/GPUResource/D3DGraphicsTexture.h"

D3DGraphicsConstantBuffer*  GraphicsManager::mCBufferArray[CBUFFER_TYPE_COUNT];
D3DGraphicsSamplerState*    GraphicsManager::mSamplerStates[SAMPLER_STATE_TYPE_COUNT];
D3DGraphicsVertexShader*    GraphicsManager::mVertexShaders[VS_TYPE_COUNT];
D3DGraphicsPixelShader*     GraphicsManager::mPixelShaders[PS_TYPE_COUNT];

BOOL GraphicsManager::Initialize()
{
    D3DGraphicsDevice::Initialize();
    D3DGraphicsRenderer::Initialize();

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
    //////////////////////////////////////////
    // Vertex Shader
    //////////////////////////////////////////
    mVertexShaders[(UINT)eVertexShaderType::STANDARD] = new D3DGraphicsVertexShader(L"resource/shader/Standard_VS.cso");
    mVertexShaders[(UINT)eVertexShaderType::SKYBOX] = new D3DGraphicsVertexShader(L"resource/shader/SkyBox_VS.cso");
    //////////////////////////////////////////
    // Pixel Shader
    //////////////////////////////////////////
    mPixelShaders[(UINT)ePixelShaderType::BLINN_PHONG] = new D3DGraphicsPixelShader(L"resource/shader/BlinnPhong_PS.cso");
    mPixelShaders[(UINT)ePixelShaderType::PBR] = new D3DGraphicsPixelShader(L"resource/shader/PBR_PS.cso");
    mPixelShaders[(UINT)ePixelShaderType::SKYBOX] = new D3DGraphicsPixelShader(L"resource/shader/SkyBox_PS.cso");
    //////////////////////////////////////////
    // Sampler State
    //////////////////////////////////////////
    D3DGraphicsSamplerState* pSamplerState;
    {
        D3D11_SAMPLER_DESC spDesc = {};
        spDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        spDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        spDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        spDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        spDesc.MinLOD = 0;
        spDesc.MaxLOD = D3D11_FLOAT32_MAX;

        UINT slot = static_cast<UINT>(eSamplerStateType::LINEAR_WRAP);
        pSamplerState = new D3DGraphicsSamplerState(&spDesc);
        pSamplerState->SetBindSlot(slot);
        pSamplerState->SetBindStage(eShaderStage::PS);
        mSamplerStates[slot] = pSamplerState;
    }
    {
        D3D11_SAMPLER_DESC spDesc = {};
        spDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        spDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        spDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        spDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        spDesc.MaxAnisotropy = 1;
        spDesc.MinLOD = 0;
        spDesc.MaxLOD = D3D11_FLOAT32_MAX;

        UINT slot = static_cast<UINT>(eSamplerStateType::LINEAR_CLAMP);
        pSamplerState = new D3DGraphicsSamplerState(&spDesc);
        pSamplerState->SetBindSlot(slot);
        pSamplerState->SetBindStage(eShaderStage::PS);
        mSamplerStates[slot] = pSamplerState;
    }
   
    return TRUE;
}

void GraphicsManager::Finalization()
{
    SAFE_RELEASE_ARRAY(mCBufferArray);
    SAFE_RELEASE_ARRAY(mSamplerStates);
    SAFE_RELEASE_ARRAY(mVertexShaders);
    SAFE_RELEASE_ARRAY(mPixelShaders);
    D3DGraphicsDevice::Finalization();
    D3DGraphicsRenderer::Finalization();
}
