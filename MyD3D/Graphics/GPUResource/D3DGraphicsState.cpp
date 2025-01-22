#include "pch.h"
#include "D3DGraphicsState.h"
#include "Graphics/Core/D3DGraphicsDevice.h"
#include "Graphics/Core/D3DGraphicsRenderer.h"

D3DGraphicsSamplerState::D3DGraphicsSamplerState(IN D3D11_SAMPLER_DESC* _pSamplerDesc)
{
    Helper::HRT(D3DGraphicsDevice::GetDevice()->CreateSamplerState(_pSamplerDesc, &mSamplerState),
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

D3DGraphicsBlendState::D3DGraphicsBlendState(IN D3D11_BLEND_DESC* _pBlendDesc)
{
    Helper::HRT(D3DGraphicsDevice::GetDevice()->CreateBlendState(_pBlendDesc, &mBlendState),
        "Hresult Failed to Create D3DGraphicsBlendState::CreateBlendState.");
}

void D3DGraphicsBlendState::Release()
{
    SAFE_RELEASE(mBlendState);
}

HRESULT D3DGraphicsBlendState::Create()
{
    return S_OK;
}

HRESULT D3DGraphicsBlendState::Bind()
{
    ID3D11DeviceContext* pDeviceContext = D3DGraphicsRenderer::GetDevicecontext();
    if (pDeviceContext)
    {
        pDeviceContext->OMSetBlendState(mBlendState, NULL, 0xffffffff);
        return S_OK;
    }
    return E_FAIL;
}

HRESULT D3DGraphicsBlendState::Reset()
{
    ID3D11DeviceContext* pDeviceContext = D3DGraphicsRenderer::GetDevicecontext();
    if (pDeviceContext)
    {
        pDeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
        return S_OK;
    }
    return E_FAIL;
}

D3DGraphicsRasterizerState::D3DGraphicsRasterizerState(IN D3D11_RASTERIZER_DESC* _pRRDesc)
{
    Helper::HRT(D3DGraphicsDevice::GetDevice()->CreateRasterizerState(_pRRDesc, &mRRState),
        "Hresult Failed to Create D3DGraphicsRasterizerState::CreateRasterizerState.");
}

void D3DGraphicsRasterizerState::Release()
{
    SAFE_RELEASE(mRRState);
}

HRESULT D3DGraphicsRasterizerState::Create()
{
    return S_OK;
}

HRESULT D3DGraphicsRasterizerState::Bind()
{
    ID3D11DeviceContext* pDeviceContext = D3DGraphicsRenderer::GetDevicecontext();
    if (pDeviceContext)
    {
        pDeviceContext->RSSetState(mRRState);
        return S_OK;
    }
    return E_NOTIMPL;
}

HRESULT D3DGraphicsRasterizerState::Reset()
{
    ID3D11DeviceContext* pDeviceContext = D3DGraphicsRenderer::GetDevicecontext();
    if (pDeviceContext)
    {
        pDeviceContext->RSSetState(nullptr);
        return S_OK;
    }
    return E_NOTIMPL;
}
