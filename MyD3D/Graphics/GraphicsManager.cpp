#include "pch.h"
#include "GraphicsManager.h"
#include "Manager/GameManager.h"
#include "Resource/ResourceManager.h"
#include "Graphics/ConstantBuffer.h"

#include "../Graphics/GraphicsShader.h"
#include "../Graphics/GraphicsInputLayout.h"

IConstantBuffer*                            GraphicsManager::mCBufferArray[CBUFFER_SIZE];
std::vector<std::shared_ptr<SamplerState>>  GraphicsManager::mSamplerState;
std::vector<std::shared_ptr<VertexShader>>  GraphicsManager::mVertexShader;
std::vector<std::shared_ptr<PixelShader>>   GraphicsManager::mPixelShader;
IGraphicsDevice*                            GraphicsManager::mDevice = nullptr;
IGraphicsRenderer*                          GraphicsManager::mRenderer = nullptr;

BOOL GraphicsManager::Initialize()
{
    HRESULT hr = CreateGraphicsDeviceAndRenderer(&mDevice, &mRenderer);
    if (FAILED(hr))
    {
        throw std::runtime_error("Hresult Failed to CreateGraphicsDeviceAndRenderer");
    }

    BufferDesc bufDesc = {};
    bufDesc.Usage = D3D11_USAGE_DEFAULT;
    bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufDesc.CPUAccessFlags = 0;
    // Transform
    {
        UINT slot = static_cast<UINT>(eCBufferType::Transform);
        bufDesc.ByteWidth = sizeof(TransformCBuffer);
        mDevice->CreateBuffer(L"TransformCBuffer", &bufDesc, nullptr,
            (void**)&mCBufferArray[slot]);
        mCBufferArray[slot]->SetSlot(slot).
            SetShaderStage(eShaderStage::VS);
        mCBufferArray[slot]->Bind(mRenderer);
    }
    // Material
    {
        UINT slot = static_cast<UINT>(eCBufferType::Material);
        bufDesc.ByteWidth = sizeof(MaterialCBuffer);
        mDevice->CreateBuffer(L"MaterialCBuffer", &bufDesc, nullptr,
            (void**)&mCBufferArray[slot]);
        mCBufferArray[slot]->SetSlot(slot).
            SetShaderStage(eShaderStage::PS);
        mCBufferArray[slot]->Bind(mRenderer);
    }
    // Camera
    {
        UINT slot = static_cast<UINT>(eCBufferType::Camera);
        bufDesc.ByteWidth = sizeof(CameraCBuffer);
        mDevice->CreateBuffer(L"CameraCBuffer", &bufDesc, nullptr,
            (void**)&mCBufferArray[slot]);
        mCBufferArray[slot]->SetSlot(slot).
            SetShaderStage(eShaderStage::PS);
        mCBufferArray[slot]->Bind(mRenderer);
    }
    // BoneMatrix
    {
        UINT slot = static_cast<UINT>(eCBufferType::BoneMatrix);
        bufDesc.ByteWidth = sizeof(BoneMatrixCBuffer);
        mDevice->CreateBuffer(L"BoneMatrixCBuffer", &bufDesc, nullptr,
            (void**)&mCBufferArray[slot]);
        mCBufferArray[slot]->SetSlot(slot).
            SetShaderStage(eShaderStage::VS);
        mCBufferArray[slot]->Bind(mRenderer);
    }
    // BoneMatrix
    {
        UINT slot = static_cast<UINT>(eCBufferType::Light);
        bufDesc.ByteWidth = sizeof(LightCBuffer);
        mDevice->CreateBuffer(L"BoneMatrixCBuffer", &bufDesc, nullptr,
            (void**)&mCBufferArray[slot]);
        mCBufferArray[slot]->SetSlot(slot).
            SetShaderStage(eShaderStage::PS);
        mCBufferArray[slot]->Bind(mRenderer);
    }
    // Shader
    std::shared_ptr<VertexShader> pVertexShader;
    std::shared_ptr<PixelShader> pPixelShader;
    {
        
        pVertexShader = ResourceManager::AddResource<VertexShader>(L"resource/shader/Standard_VS.cso");
        mVertexShader.push_back(pVertexShader);
    }
    {
        pPixelShader = ResourceManager::AddResource<PixelShader>(L"resource/shader/PBR_PS.cso");
        mPixelShader.push_back(pPixelShader);
    }
    {
        pPixelShader = ResourceManager::AddResource<PixelShader>(L"resource/shader/BlinnPhong_PS.cso");
        mPixelShader.push_back(pPixelShader);
    }

    // SamplerState
    std::shared_ptr<SamplerState> pSamplerState;
    {
        SamplerDesc spDesc = {};
        spDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        spDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        spDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        spDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        spDesc.MinLOD = 0;
        spDesc.MaxLOD = D3D11_FLOAT32_MAX;

        UINT slot = static_cast<UINT>(eSamplerStateType::LINEAR_WRAP);
        pSamplerState = ResourceManager::AddResource<SamplerState>(L"Linear_Wrap_Sampler", &spDesc);
        pSamplerState->mSamplerState->SetShaderStage(eShaderStage::PS).SetSlot(slot);
        pSamplerState->mSamplerState->Bind(mRenderer);

        mSamplerState.push_back(pSamplerState);
    }
    {
        SamplerDesc spDesc = {};
        spDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        spDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        spDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        spDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        spDesc.MaxAnisotropy = 1;
        spDesc.MinLOD = 0;
        spDesc.MaxLOD = D3D11_FLOAT32_MAX;

        UINT slot = static_cast<UINT>(eSamplerStateType::LINEAR_CLAMP);
        pSamplerState = ResourceManager::AddResource<SamplerState>(L"Linear_Clamp_Sampler", &spDesc);
        pSamplerState->mSamplerState->SetShaderStage(eShaderStage::PS).SetSlot(slot);
        pSamplerState->mSamplerState->Bind(mRenderer);

        mSamplerState.push_back(pSamplerState);
    }
   
    return TRUE;
}

void GraphicsManager::Finalization()
{
    SAFE_RELEASE_ARRAY(mCBufferArray);
}

HRESULT GraphicsManager::BindViewport(ViewportDesc* _pViewportDesc)
{
    return mRenderer->BindViewport(_pViewportDesc);
}