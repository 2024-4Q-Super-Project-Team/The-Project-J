#include "pch.h"
#include "D3DGraphicsTexture.h"
#include "Graphics/Core/D3DGraphicsDevice.h"
#include "Graphics/Core/D3DGraphicsRenderer.h"

D3DGraphicsTexture2D::D3DGraphicsTexture2D(D3D11_TEXTURE2D_DESC* _Desc)
    : mDesc(_Desc)
    , mTex(nullptr)
{
    Create();
}

D3DGraphicsTexture2D::D3DGraphicsTexture2D(ID3D11Texture2D* _pTex)
    : mDesc(nullptr)
    , mTex(_pTex)
{
}

void D3DGraphicsTexture2D::Release()
{
    SAFE_RELEASE(mTex);
}

HRESULT D3DGraphicsTexture2D::Create()
{
    D3DGraphicsDevice::GetDevice()->CreateTexture2D(mDesc, nullptr, &mTex);
    return S_OK;
}

HRESULT D3DGraphicsTexture2D::Bind()
{
    return E_NOTIMPL;
}

HRESULT D3DGraphicsTexture2D::Reset()
{
    return E_NOTIMPL;
}
////////////////////////////////////////////////
/// D3DGraphicsRTV
////////////////////////////////////////////////

D3DGraphicsRTV::D3DGraphicsRTV(D3DGraphicsTexture2D* _pTex2D, const D3D11_RENDER_TARGET_VIEW_DESC* _pDesc)
{
    Helper::HRT(D3DGraphicsDevice::GetDevice()->CreateRenderTargetView(_pTex2D->mTex, _pDesc, &mRTV), "HRESULT Failed To CreateRenderTargetView()");
}
void D3DGraphicsRTV::Release()
{
    SAFE_RELEASE(mRTV);
    delete this;
}

HRESULT D3DGraphicsRTV::Create()
{
    return S_OK;
}
HRESULT D3DGraphicsRTV::Bind()
{
    // RTV와 DSV의 동시 바인드는 Renderer에서 해줌
    auto pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    if (pDeviceContext)
    {
        pDeviceContext->OMSetRenderTargets(1, &mRTV, NULL);
        return S_OK;
    }
    return E_FAIL;
}
HRESULT D3DGraphicsRTV::Reset()
{
    // RTV와 DSV의 동시 바인드는 Renderer에서 해줌
    auto pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    if (pDeviceContext)
    {
        const FLOAT clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
        pDeviceContext->ClearRenderTargetView(mRTV, clearColor);
        return S_OK;
    }
    return E_FAIL;
}
////////////////////////////////////////////////
/// D3DGraphicsDSV
////////////////////////////////////////////////
D3DGraphicsDSV::D3DGraphicsDSV(D3DGraphicsTexture2D* _pTex2D, const D3D11_DEPTH_STENCIL_VIEW_DESC* _pDesc)
{
    Helper::HRT(D3DGraphicsDevice::GetDevice()->CreateDepthStencilView(_pTex2D->mTex, _pDesc, &mDSV), "HRESULT Failed To CreateDepthStencilView()");
}
void D3DGraphicsDSV::Release()
{
    SAFE_RELEASE(mDSV);
    delete this;
}

HRESULT D3DGraphicsDSV::Create()
{
    return S_OK;
}
HRESULT D3DGraphicsDSV::Bind()
{
    // RTV와 DSV의 동시 바인드는 Renderer에서 해줌
    auto pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    if (pDeviceContext)
    {
        pDeviceContext->OMSetRenderTargets(0, NULL, mDSV);
        return S_OK;
    }
    return E_FAIL;
}
HRESULT D3DGraphicsDSV::Reset()
{
    // RTV와 DSV의 동시 바인드는 Renderer에서 해줌
    auto pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    if (pDeviceContext)
    {
        pDeviceContext->ClearDepthStencilView(mDSV,
            D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }
    return E_FAIL;
}
////////////////////////////////////////////////
/// D3DGraphicsSRV
////////////////////////////////////////////////
D3DGraphicsSRV::D3DGraphicsSRV(D3DGraphicsTexture2D* _pTex2D, const D3D11_SHADER_RESOURCE_VIEW_DESC* _pDesc)
{
    Helper::HRT(D3DGraphicsDevice::GetDevice()->CreateShaderResourceView(_pTex2D->mTex, _pDesc, &mSRV), "HRESULT Failed To CreateShaderResourceView()");
}
void D3DGraphicsSRV::Release()
{
    SAFE_RELEASE(mSRV);
    delete this;
}

HRESULT D3DGraphicsSRV::Create()
{
    return S_OK;
}

HRESULT D3DGraphicsSRV::Bind()
{
    auto pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    if (pDeviceContext)
    {
        switch (mStage)
        {
        case eShaderStage::PASS:
            break;
        case eShaderStage::VS:
            pDeviceContext->VSSetShaderResources(mSlot, 1, &mSRV);
            break;
        case eShaderStage::PS:
            pDeviceContext->PSSetShaderResources(mSlot, 1, &mSRV);
            break;
        case eShaderStage::GS:
            pDeviceContext->GSSetShaderResources(mSlot, 1, &mSRV);
            break;
        case eShaderStage::HS:
            pDeviceContext->HSSetShaderResources(mSlot, 1, &mSRV);
            break;
        case eShaderStage::DS:
            pDeviceContext->DSSetShaderResources(mSlot, 1, &mSRV);
            break;
        case eShaderStage::CS:
            pDeviceContext->CSSetShaderResources(mSlot, 1, &mSRV);
            break;
        case eShaderStage::ALL:
            pDeviceContext->VSSetShaderResources(mSlot, 1, &mSRV);
            pDeviceContext->PSSetShaderResources(mSlot, 1, &mSRV);
            pDeviceContext->GSSetShaderResources(mSlot, 1, &mSRV);
            pDeviceContext->HSSetShaderResources(mSlot, 1, &mSRV);
            pDeviceContext->DSSetShaderResources(mSlot, 1, &mSRV);
            pDeviceContext->CSSetShaderResources(mSlot, 1, &mSRV);
            break;
        default:
            return E_INVALIDARG;
        }
        return S_OK;
    }
    return E_FAIL;
}

HRESULT D3DGraphicsSRV::Reset()
{
    auto pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    ID3D11ShaderResourceView* nullSRV = nullptr;
    if (pDeviceContext)
    {
        switch (mStage)
        {
        case eShaderStage::PASS:
            break;
        case eShaderStage::VS:
            pDeviceContext->VSSetShaderResources(mSlot, 1, &nullSRV);
            break;
        case eShaderStage::PS:
            pDeviceContext->PSSetShaderResources(mSlot, 1, &nullSRV);
            break;
        case eShaderStage::GS:
            pDeviceContext->GSSetShaderResources(mSlot, 1, &nullSRV);
            break;
        case eShaderStage::HS:
            pDeviceContext->HSSetShaderResources(mSlot, 1, &nullSRV);
            break;
        case eShaderStage::DS:
            pDeviceContext->DSSetShaderResources(mSlot, 1, &nullSRV);
            break;
        case eShaderStage::CS:
            pDeviceContext->CSSetShaderResources(mSlot, 1, &nullSRV);
            break;
        case eShaderStage::ALL:
            pDeviceContext->VSSetShaderResources(mSlot, 1, &nullSRV);
            pDeviceContext->PSSetShaderResources(mSlot, 1, &nullSRV);
            pDeviceContext->GSSetShaderResources(mSlot, 1, &nullSRV);
            pDeviceContext->HSSetShaderResources(mSlot, 1, &nullSRV);
            pDeviceContext->DSSetShaderResources(mSlot, 1, &nullSRV);
            pDeviceContext->CSSetShaderResources(mSlot, 1, &nullSRV);
            break;
        default:
            return E_INVALIDARG;
        }
        return S_OK;
    }
    return E_FAIL;
}

D3DGraphicsImg::D3DGraphicsImg(std::wstring_view _path)
    : GraphicsResourceKeyHandler(_path)
{
    Create();
}

void D3DGraphicsImg::Release()
{
    SAFE_RELEASE(mSRV);
    delete this;
}

HRESULT D3DGraphicsImg::Create()
{
    ScratchImage Image;
    std::wstring ext = mName.substr(mName.find_last_of(L".") + 1);
    if (ext == L"dds")
    {
        Helper::HRT(LoadFromDDSFile(mName.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, Image),
            "Hresult Failed to GraphicsTexture2D::LoadFromDDSFile.");
    }
    else if (ext == L"tga")
    {
        TexMetadata metadata;
        Helper::HRT(LoadFromTGAFile(mName.c_str(), &metadata, Image),
            "Hresult Failed to GraphicsTexture2D::LoadFromDDSFile.");
    }
    else
    {
        Helper::HRT(LoadFromWICFile(mName.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, Image),
            "Hresult Failed to GraphicsTexture2D::LoadFromDDSFile.");
    }
    ID3D11Device* pDevice = D3DGraphicsDevice::GetDevice();
    if (pDevice)
    {
        Helper::HRT(CreateShaderResourceView(
            pDevice
            , Image.GetImages()
            , Image.GetImageCount()
            , Image.GetMetadata()
            , &mSRV),
            "Hresult Failed to GraphicsTexture2D::CreateShaderResourceView.");
        return S_OK;
    }
    return E_FAIL;
}

HRESULT D3DGraphicsImg::Bind()
{
    auto pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    if (pDeviceContext)
    {
        switch (mStage)
        {
        case eShaderStage::PASS:
            break;
        case eShaderStage::VS:
            pDeviceContext->VSSetShaderResources(mSlot, 1, &mSRV);
            break;
        case eShaderStage::PS:
            pDeviceContext->PSSetShaderResources(mSlot, 1, &mSRV);
            break;
        case eShaderStage::GS:
            pDeviceContext->GSSetShaderResources(mSlot, 1, &mSRV);
            break;
        case eShaderStage::HS:
            pDeviceContext->HSSetShaderResources(mSlot, 1, &mSRV);
            break;
        case eShaderStage::DS:
            pDeviceContext->DSSetShaderResources(mSlot, 1, &mSRV);
            break;
        case eShaderStage::CS:
            pDeviceContext->CSSetShaderResources(mSlot, 1, &mSRV);
            break;
        case eShaderStage::ALL:
            pDeviceContext->VSSetShaderResources(mSlot, 1, &mSRV);
            pDeviceContext->PSSetShaderResources(mSlot, 1, &mSRV);
            pDeviceContext->GSSetShaderResources(mSlot, 1, &mSRV);
            pDeviceContext->HSSetShaderResources(mSlot, 1, &mSRV);
            pDeviceContext->DSSetShaderResources(mSlot, 1, &mSRV);
            pDeviceContext->CSSetShaderResources(mSlot, 1, &mSRV);
            break;
        default:
            return E_INVALIDARG;
        }
        return S_OK;
    }
    return E_FAIL;
}

HRESULT D3DGraphicsImg::Reset()
{
    auto pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    ID3D11ShaderResourceView* nullSRV = nullptr;
    if (pDeviceContext)
    {
        switch (mStage)
        {
        case eShaderStage::PASS:
            break;
        case eShaderStage::VS:
            pDeviceContext->VSSetShaderResources(mSlot, 1, &nullSRV);
            break;
        case eShaderStage::PS:
            pDeviceContext->PSSetShaderResources(mSlot, 1, &nullSRV);
            break;
        case eShaderStage::GS:
            pDeviceContext->GSSetShaderResources(mSlot, 1, &nullSRV);
            break;
        case eShaderStage::HS:
            pDeviceContext->HSSetShaderResources(mSlot, 1, &nullSRV);
            break;
        case eShaderStage::DS:
            pDeviceContext->DSSetShaderResources(mSlot, 1, &nullSRV);
            break;
        case eShaderStage::CS:
            pDeviceContext->CSSetShaderResources(mSlot, 1, &nullSRV);
            break;
        case eShaderStage::ALL:
            pDeviceContext->VSSetShaderResources(mSlot, 1, &nullSRV);
            pDeviceContext->PSSetShaderResources(mSlot, 1, &nullSRV);
            pDeviceContext->GSSetShaderResources(mSlot, 1, &nullSRV);
            pDeviceContext->HSSetShaderResources(mSlot, 1, &nullSRV);
            pDeviceContext->DSSetShaderResources(mSlot, 1, &nullSRV);
            pDeviceContext->CSSetShaderResources(mSlot, 1, &nullSRV);
            break;
        default:
            return E_INVALIDARG;
        }
        return S_OK;
    }
    return E_FAIL;
}
