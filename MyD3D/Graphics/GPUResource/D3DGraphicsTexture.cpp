#include "pch.h"
#include "D3DGraphicsTexture.h"
#include "Graphics/Core/D3DGraphicsDevice.h"
#include "Graphics/Core/D3DGraphicsRenderer.h"

D3DGraphicsTexture2D::D3DGraphicsTexture2D(D3D11_TEXTURE2D_DESC* _Desc)
    : mDesc({})
    , mTex(nullptr)
{
    if (_Desc == nullptr)
    {
        Helper::HRT(E_FAIL, "Nullpointer reference to D3D11_TEXTURE2D_DESC.");
    }
    else
    {
        mDesc = (*_Desc);
    }
    Helper::HRT(Create(),"Create Fail D3DGraphicsTexture2D.");
}

D3DGraphicsTexture2D::D3DGraphicsTexture2D(ID3D11Texture2D* _pTex)
    : mDesc({})
    , mTex(_pTex)
{
    if (_pTex == nullptr)
    {
        Helper::HRT(E_FAIL, "Nullpointer reference to ID3D11Texture2D.");
    }
    _pTex->GetDesc(&mDesc);
}

D3DGraphicsTexture2D::~D3DGraphicsTexture2D()
{
    if (mTex)
    {
        ULONG refCount = 0;
        do {
            refCount = mTex->Release();
        } while (refCount > 0);
        mTex = nullptr;
    }
}

void D3DGraphicsTexture2D::Release()
{
    if (mTex)
    {
        ULONG refCount = 0;
        do {
            refCount = mTex->Release();
        } while (refCount > 0);
        mTex = nullptr;
    }
    delete this;
}

HRESULT D3DGraphicsTexture2D::Create()
{
    return D3DGraphicsDevice::GetDevice()->CreateTexture2D(&mDesc, nullptr, &mTex);
}

HRESULT D3DGraphicsTexture2D::Bind()
{
    return E_NOTIMPL;
}

HRESULT D3DGraphicsTexture2D::Reset()
{
    return E_NOTIMPL;
}

HRESULT D3DGraphicsTexture2D::Resize(UINT _width, UINT _height)
{
    if (mTex)
    {
        mTex->GetDesc(&mDesc);
        SAFE_RELEASE(mTex);
        mDesc.Width = _width;
        mDesc.Height = _height;
        return Create();
    }
    return E_FAIL;
}

D3DResourceView::D3DResourceView(D3DGraphicsTexture2D* _pRefTex)
    : mRefTex(_pRefTex)
{
}

D3DResourceView::~D3DResourceView()
{
    // 여기서 삭제하면 RTV, SRV등 같은 텍스쳐를도 참조하는데 두번 삭제해버리므로, 이중 Delete가 이뤄져 터진다.
}

HRESULT D3DResourceView::Resize(UINT _width, UINT _height)
{
    return mRefTex->Resize(_width, _height);
}
////////////////////////////////////////////////
/// D3DGraphicsRTV
////////////////////////////////////////////////
D3DGraphicsRTV::D3DGraphicsRTV(D3DGraphicsTexture2D* _pTex2D, D3D11_RENDER_TARGET_VIEW_DESC* _pDesc)
    : D3DResourceView(_pTex2D), mDesc(_pDesc)
{
    Helper::HRT(Create(), "HRESULT Failed To CreateRenderTargetView()");
}

D3DGraphicsRTV::~D3DGraphicsRTV()
{
    SAFE_RELEASE(mRTV);
    SAFE_RELEASE(mRefTex);
}

void D3DGraphicsRTV::Release()
{
    SAFE_RELEASE(mRTV);
    SAFE_RELEASE(mRefTex);
    delete this;
}

HRESULT D3DGraphicsRTV::Create()
{
    return D3DGraphicsDevice::GetDevice()->CreateRenderTargetView(mRefTex->mTex, mDesc, &mRTV);
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
D3DGraphicsDSV::D3DGraphicsDSV(D3DGraphicsTexture2D* _pTex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* _pDesc)
    : D3DResourceView(_pTex2D), mDesc(_pDesc)
{
    Helper::HRT(Create(), "HRESULT Failed To CreateDepthStencilView()");
}

D3DGraphicsDSV::~D3DGraphicsDSV()
{
    SAFE_RELEASE(mDSV);
    SAFE_RELEASE(mRefTex);
}

void D3DGraphicsDSV::Release()
{
    SAFE_RELEASE(mDSV);
    SAFE_RELEASE(mRefTex);
    delete this;
}

HRESULT D3DGraphicsDSV::Create()
{
    return D3DGraphicsDevice::GetDevice()->CreateDepthStencilView(mRefTex->mTex, mDesc, &mDSV);
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
D3DGraphicsSRV::D3DGraphicsSRV(D3DGraphicsTexture2D* _pTex2D, D3D11_SHADER_RESOURCE_VIEW_DESC* _pDesc)
    : D3DResourceView(_pTex2D), mDesc(_pDesc)
{
    Helper::HRT(Create(), "HRESULT Failed To CreateShaderResourceView()");
}

D3DGraphicsSRV::~D3DGraphicsSRV()
{
    SAFE_RELEASE(mSRV);
}

void D3DGraphicsSRV::Release()
{
    SAFE_RELEASE(mSRV);
    delete this;
}

HRESULT D3DGraphicsSRV::Create()
{
    return D3DGraphicsDevice::GetDevice()->CreateShaderResourceView(mRefTex->mTex, mDesc, &mSRV);
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
    : D3DResourceView(nullptr), GraphicsResourceKeyHandler(_path)
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
    HRESULT hr;
    ScratchImage Image;
    std::wstring ext = mName.substr(mName.find_last_of(L".") + 1);
    if (ext == L"dds")
    {
        if (S_OK != LoadFromDDSFile(mName.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, Image)) {
            Display::Console::Log("Hresult Failed to GraphicsTexture2D::LoadFromDDSFile.");
        }
    }
    else if (ext == L"tga")
    {
        TexMetadata metadata;
        if (S_OK != LoadFromTGAFile(mName.c_str(), &metadata, Image)) {
            Display::Console::Log("Hresult Failed to GraphicsTexture2D::LoadFromTGAFile.");
        }
    }
    else
    {
        if (S_OK != LoadFromWICFile(mName.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, Image)) {
            Display::Console::Log("Hresult Failed to GraphicsTexture2D::LoadFromWICFile.");
        }
    }
    ID3D11Device* pDevice = D3DGraphicsDevice::GetDevice();
    if (pDevice)
    {
        CreateShaderResourceView(
            pDevice
            , Image.GetImages()
            , Image.GetImageCount()
            , Image.GetMetadata()
            , &mSRV);
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

