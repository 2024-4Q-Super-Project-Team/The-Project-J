#include "pch.h"
#include "GraphicsTexture.h"
#include "GraphicsDevice.h"
#include "GraphicsRenderer.h"

/////////////////////////////////////////////////////////////////////////////////////////
/// Texture2D ///////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
GraphicsTexture2D::GraphicsTexture2D(const std::wstring& _path, GraphicsDevice* _pDevice)
	: mTexRV(nullptr)
	, mName(_path.data())
    , mShaderStage(eShaderStage::PASS)
    , mSlot(0)
{
	HRESULT hr;
	ScratchImage Image;
	std::wstring ext = _path.substr(_path.find_last_of(L".") + 1);
	if (ext == L"dds")
	{
		hr = LoadFromDDSFile(_path.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, Image);
		if(FAILED(hr)) throw std::runtime_error("Hresult Failed to GraphicsTexture2D::LoadFromDDSFile.");
	}
    else if (ext == L"tga")
    {
        TexMetadata metadata;
        hr = LoadFromTGAFile(_path.c_str(), &metadata, Image);
        if (FAILED(hr)) throw std::runtime_error("Hresult Failed to GraphicsTexture2D::LoadFromDDSFile.");
    }
	else
	{
		hr = LoadFromWICFile(_path.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, Image);
		if (FAILED(hr)) throw std::runtime_error("Hresult Failed to GraphicsTexture2D::LoadFromDDSFile.");
	}
	ID3D11Device* pDevice = _pDevice->mDevice;
	if (pDevice)
	{
		hr = CreateShaderResourceView(
			pDevice
			, Image.GetImages()
			, Image.GetImageCount()
			, Image.GetMetadata()
			, &mTexRV);
        if (FAILED(hr))
        {
            if (mTexRV)
                mTexRV->Release();
            mTexRV = nullptr;
            throw std::runtime_error("Hresult Failed to GraphicsTexture2D::CreateShaderResourceView.");
        }
	}
}

GraphicsTexture2D::~GraphicsTexture2D()
{
}

UTID GraphicsTexture2D::GetTypeID()
{
    return _UTID_GRAPHICS_TEXTURE2D;
}

HRESULT GraphicsTexture2D::QueryInterface(const UTID _UTID, void** _ppObject)
{
    if (_ppObject == nullptr)
    {
        return E_INVALIDARG;
    }
    *_ppObject = nullptr;
    switch (_UTID)
    {
    case _UTID_GRAPHICS_TEXTURE2D:
        *_ppObject = static_cast<GraphicsTexture2D*>(this);
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

void GraphicsTexture2D::Release()
{
    if (mTexRV)
        delete mTexRV;
    mTexRV = nullptr;

    delete this;
}

const WCHAR* GraphicsTexture2D::GetName()
{
	return mName;
}

HRESULT GraphicsTexture2D::Bind(IGraphicsRenderer* _renderer)
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
            pDeviceContext->VSSetShaderResources(mSlot, 1, &mTexRV);
            break;
        case eShaderStage::PS:
            pDeviceContext->PSSetShaderResources(mSlot, 1, &mTexRV);
            break;
        case eShaderStage::GS:
            pDeviceContext->GSSetShaderResources(mSlot, 1, &mTexRV);
            break;
        case eShaderStage::HS:
            pDeviceContext->HSSetShaderResources(mSlot, 1, &mTexRV);
            break;
        case eShaderStage::DS:
            pDeviceContext->DSSetShaderResources(mSlot, 1, &mTexRV);
            break;
        case eShaderStage::CS:
            pDeviceContext->CSSetShaderResources(mSlot, 1, &mTexRV);
            break;
        case eShaderStage::ALL:
            pDeviceContext->VSSetShaderResources(mSlot, 1, &mTexRV);
            pDeviceContext->PSSetShaderResources(mSlot, 1, &mTexRV);
            pDeviceContext->GSSetShaderResources(mSlot, 1, &mTexRV);
            pDeviceContext->HSSetShaderResources(mSlot, 1, &mTexRV);
            pDeviceContext->DSSetShaderResources(mSlot, 1, &mTexRV);
            pDeviceContext->CSSetShaderResources(mSlot, 1, &mTexRV);
            break;
        default:
            return E_INVALIDARG;
        }
        return S_OK;
    }
    return E_NOINTERFACE;
}

HRESULT GraphicsTexture2D::Reset(IGraphicsRenderer* _renderer)
{
    if (_renderer == nullptr)
    {
        return E_INVALIDARG;
    }
    GraphicsRenderer* pRenderer;
    if (S_OK == _renderer->QueryInterface(_UTID_GRAPHICS_RENDERER, (void**)&pRenderer))
    {
        ID3D11DeviceContext* pDeviceContext = pRenderer->mDeviceContext;
        ID3D11ShaderResourceView* nullResource = nullptr;
        if (pDeviceContext == nullptr) return E_FAIL;
        switch (mShaderStage)
        {
        case eShaderStage::PASS:
            break;
        case eShaderStage::VS:
            pDeviceContext->VSSetShaderResources(mSlot, 1, &nullResource);
            break;
        case eShaderStage::PS:
            pDeviceContext->PSSetShaderResources(mSlot, 1, &nullResource);
            break;
        case eShaderStage::GS:
            pDeviceContext->GSSetShaderResources(mSlot, 1, &nullResource);
            break;
        case eShaderStage::HS:
            pDeviceContext->HSSetShaderResources(mSlot, 1, &nullResource);
            break;
        case eShaderStage::DS:
            pDeviceContext->DSSetShaderResources(mSlot, 1, &nullResource);
            break;
        case eShaderStage::CS:
            pDeviceContext->CSSetShaderResources(mSlot, 1, &nullResource);
            break;
        case eShaderStage::ALL:
            pDeviceContext->VSSetShaderResources(mSlot, 1, &nullResource);
            pDeviceContext->PSSetShaderResources(mSlot, 1, &nullResource);
            pDeviceContext->GSSetShaderResources(mSlot, 1, &nullResource);
            pDeviceContext->HSSetShaderResources(mSlot, 1, &nullResource);
            pDeviceContext->DSSetShaderResources(mSlot, 1, &nullResource);
            pDeviceContext->CSSetShaderResources(mSlot, 1, &nullResource);
            break;
        default:
            return E_INVALIDARG;
        }
        return S_OK;
    }
    return E_NOINTERFACE;
}

ITexture& GraphicsTexture2D::SetSlot(UINT _startSlot)
{
    mSlot = _startSlot;
    return *this;
}

ITexture& GraphicsTexture2D::SetShaderStage(eShaderStage _stage)
{
    mShaderStage = _stage;
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////
/// Texture3D ///////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

GraphicsTexture3D::GraphicsTexture3D(GraphicsDevice* _pDevice, const std::wstring& _path)
	: mTexRV(nullptr)
	, mName(_path.data())
    , mShaderStage(eShaderStage::PASS)
    , mSlot(0)
{
}

GraphicsTexture3D::~GraphicsTexture3D()
{
}

UTID GraphicsTexture3D::GetTypeID()
{
    return _UTID_GRAPHICS_TEXTURE3D;
}

HRESULT GraphicsTexture3D::QueryInterface(const UTID _UTID, void** _ppObject)
{
    return E_NOTIMPL;
}

void GraphicsTexture3D::Release()
{
}

const WCHAR* GraphicsTexture3D::GetName()
{
	return mName;
}

HRESULT GraphicsTexture3D::Bind(IGraphicsRenderer* _renderer)
{
    return E_NOTIMPL;
}

HRESULT GraphicsTexture3D::Reset(IGraphicsRenderer* _renderer)
{
    return E_NOTIMPL;
}

ITexture& GraphicsTexture3D::SetSlot(UINT _startSlot)
{
    mSlot = _startSlot;
    return *this;
}

ITexture& GraphicsTexture3D::SetShaderStage(eShaderStage _stage)
{
    mShaderStage = _stage;
    return *this;
}