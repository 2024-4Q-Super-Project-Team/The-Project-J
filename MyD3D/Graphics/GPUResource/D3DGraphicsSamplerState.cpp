#include "pch.h"
#include "D3DGraphicsSamplerState.h"
#include "Graphics/Core/D3DGraphicsDevice.h"
#include "Graphics/Core/D3DGraphicsRenderer.h"

D3DGraphicsSamplerState::D3DGraphicsSamplerState(IN D3D11_SAMPLER_DESC* _pSamplerDesc)
{
    auto pDevice = D3DGraphicsDevice::GetDevice();
    Helper::HRT(pDevice->CreateSamplerState(_pSamplerDesc, &mSamplerState),
        "Hresult Failed to Create IInputLayout::CreateInputLayout.");
}

void D3DGraphicsSamplerState::Release()
{
    SAFE_RELEASE(mSamplerState);
    delete this;
}

HRESULT D3DGraphicsSamplerState::Create()
{
    return S_OK;
}

HRESULT D3DGraphicsSamplerState::Bind()
{
    ID3D11DeviceContext* pDeviceContext = D3DGraphicsRenderer::GetDevicecontext();
    if (pDeviceContext)
    {
        switch (mStage)
        {
        case eShaderStage::PASS:
            break;
        case eShaderStage::VS:
            pDeviceContext->VSSetSamplers(mSlot, 1, &mSamplerState);
            break;
        case eShaderStage::PS:
            pDeviceContext->PSSetSamplers(mSlot, 1, &mSamplerState);
            break;
        case eShaderStage::GS:
            pDeviceContext->GSSetSamplers(mSlot, 1, &mSamplerState);
            break;
        case eShaderStage::HS:
            pDeviceContext->HSSetSamplers(mSlot, 1, &mSamplerState);
            break;
        case eShaderStage::DS:
            pDeviceContext->DSSetSamplers(mSlot, 1, &mSamplerState);
            break;
        case eShaderStage::CS:
            pDeviceContext->CSSetSamplers(mSlot, 1, &mSamplerState);
            break;
        case eShaderStage::ALL:
            pDeviceContext->VSSetSamplers(mSlot, 1, &mSamplerState);
            pDeviceContext->PSSetSamplers(mSlot, 1, &mSamplerState);
            pDeviceContext->GSSetSamplers(mSlot, 1, &mSamplerState);
            pDeviceContext->HSSetSamplers(mSlot, 1, &mSamplerState);
            pDeviceContext->DSSetSamplers(mSlot, 1, &mSamplerState);
            pDeviceContext->CSSetSamplers(mSlot, 1, &mSamplerState);
            break;
        default:
            return E_INVALIDARG;
        }
        return S_OK;
    }
    return E_FAIL;
}

HRESULT D3DGraphicsSamplerState::Reset()
{
    ID3D11SamplerState* nullSampler = nullptr;
    ID3D11DeviceContext* pDeviceContext = D3DGraphicsRenderer::GetDevicecontext();
    if (pDeviceContext)
    {
        switch (mStage)
        {
        case eShaderStage::PASS:
            break;
        case eShaderStage::VS:
            pDeviceContext->VSSetSamplers(mSlot, 1, &nullSampler);
            break;
        case eShaderStage::PS:
            pDeviceContext->PSSetSamplers(mSlot, 1, &nullSampler);
            break;
        case eShaderStage::GS:
            pDeviceContext->GSSetSamplers(mSlot, 1, &nullSampler);
            break;
        case eShaderStage::HS:
            pDeviceContext->HSSetSamplers(mSlot, 1, &nullSampler);
            break;
        case eShaderStage::DS:
            pDeviceContext->DSSetSamplers(mSlot, 1, &nullSampler);
            break;
        case eShaderStage::CS:
            pDeviceContext->CSSetSamplers(mSlot, 1, &nullSampler);
            break;
        case eShaderStage::ALL:
            pDeviceContext->VSSetSamplers(mSlot, 1, &nullSampler);
            pDeviceContext->PSSetSamplers(mSlot, 1, &nullSampler);
            pDeviceContext->GSSetSamplers(mSlot, 1, &nullSampler);
            pDeviceContext->HSSetSamplers(mSlot, 1, &nullSampler);
            pDeviceContext->DSSetSamplers(mSlot, 1, &nullSampler);
            pDeviceContext->CSSetSamplers(mSlot, 1, &nullSampler);
            break;
        default:
            return E_INVALIDARG;
        }
        return S_OK;
    }
    return E_FAIL;
}
