#include "pch.h"
#include "GraphicsSamplerState.h"
#include "GraphicsDevice.h"
#include "GraphicsRenderer.h"

GraphicsSamplerState::GraphicsSamplerState(const WCHAR* _name, SamplerDesc* _pDesc, GraphicsDevice* _pDevice)
    : mName(_name), mSaplerState(nullptr)
{
    ID3D11Device* pDevice = _pDevice->mDevice;
    if (pDevice)
    {
        D3D11_SAMPLER_DESC samplerDesc = {};
        memcpy(&samplerDesc, _pDesc, sizeof(D3D11_SAMPLER_DESC));
        HRESULT hr = 
            pDevice->CreateSamplerState(&samplerDesc, &mSaplerState);
        if (FAILED(hr))
        {
            if(mSaplerState)
                mSaplerState->Release();
            mSaplerState = nullptr;
            throw std::runtime_error("Hresult Failed to Create IInputLayout::CreateInputLayout.");
        }
    }
}

GraphicsSamplerState::~GraphicsSamplerState()
{
}

UTID GraphicsSamplerState::GetTypeID()
{
    return _UTID_GRAPHICS_SAMPLERSTATE;
}

HRESULT GraphicsSamplerState::QueryInterface(const UTID _UTID, void** _ppObject)
{
    if (_ppObject == nullptr)
    {
        return E_INVALIDARG;
    }
    *_ppObject = nullptr;
    switch (_UTID)
    {
    case _UTID_GRAPHICS_SAMPLERSTATE:
        *_ppObject = static_cast<GraphicsSamplerState*>(this);
        break;
    case _UTID_GRAPHICS_UNKNOWN:
        *_ppObject = static_cast<IGraphicsUnknown*>(this);
        break;
    default:
        *_ppObject = nullptr;
        return E_NOINTERFACE;
        break;
    }
    return (*_ppObject) ? S_OK : E_NOINTERFACE;
}

void GraphicsSamplerState::Release()
{
    if (mSaplerState)
        mSaplerState->Release();
    mSaplerState = nullptr;
    delete this;
}

const WCHAR* GraphicsSamplerState::GetName()
{
    return mName;
}

HRESULT GraphicsSamplerState::Bind(IGraphicsRenderer* _renderer)
{
    if (_renderer == nullptr)
    {
        return E_INVALIDARG;
    }
    GraphicsRenderer* pRenderer;
    if (S_OK == _renderer->QueryInterface(_UTID_GRAPHICS_RENDERER, (void**)&pRenderer))
    {
        ID3D11DeviceContext* pDeviceContext = pRenderer->mDeviceContext;
        if (pDeviceContext == nullptr) return E_FAIL;
        switch (mShaderStage)
        {
        case eShaderStage::PASS:
            break;
        case eShaderStage::VS:
            pDeviceContext->VSSetSamplers(mSlot, 1, &mSaplerState);
            break;
        case eShaderStage::PS:
            pDeviceContext->PSSetSamplers(mSlot, 1, &mSaplerState);
            break;
        case eShaderStage::GS:
            pDeviceContext->GSSetSamplers(mSlot, 1, &mSaplerState);
            break;
        case eShaderStage::HS:
            pDeviceContext->HSSetSamplers(mSlot, 1, &mSaplerState);
            break;
        case eShaderStage::DS:
            pDeviceContext->DSSetSamplers(mSlot, 1, &mSaplerState);
            break;
        case eShaderStage::CS:
            pDeviceContext->CSSetSamplers(mSlot, 1, &mSaplerState);
            break;
        case eShaderStage::ALL:
            pDeviceContext->VSSetSamplers(mSlot, 1, &mSaplerState);
            pDeviceContext->PSSetSamplers(mSlot, 1, &mSaplerState);
            pDeviceContext->GSSetSamplers(mSlot, 1, &mSaplerState);
            pDeviceContext->HSSetSamplers(mSlot, 1, &mSaplerState);
            pDeviceContext->DSSetSamplers(mSlot, 1, &mSaplerState);
            pDeviceContext->CSSetSamplers(mSlot, 1, &mSaplerState);
            break;
        default:
            return E_INVALIDARG;
        }
        return S_OK;
    }
    return E_NOINTERFACE;
}

HRESULT GraphicsSamplerState::Reset(IGraphicsRenderer* _renderer)
{
    if (_renderer == nullptr)
    {
        return E_INVALIDARG;
    }
    GraphicsRenderer* pRenderer;
    if (S_OK == _renderer->QueryInterface(_UTID_GRAPHICS_RENDERER, (void**)&pRenderer))
    {
        ID3D11DeviceContext* pDeviceContext = pRenderer->mDeviceContext;
        if (pDeviceContext == nullptr) return E_FAIL;
        ID3D11SamplerState* nullSampler = nullptr;
        switch (mShaderStage)
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
    return E_NOINTERFACE;
}

ISamplerState& GraphicsSamplerState::SetSlot(UINT _startSlot)
{
    mSlot = _startSlot;
    return *this;
}

ISamplerState& GraphicsSamplerState::SetShaderStage(eShaderStage _stage)
{
    mShaderStage = _stage;
    return *this;
}
