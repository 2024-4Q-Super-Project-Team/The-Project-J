#include "pch.h"
#include "GraphicsDesc.h"
#include "GraphicsDevice.h"
#include "GraphicsRenderTarget.h"
#include "GraphicsBuffer.h"
#include "GraphicsShader.h"
#include "GraphicsTexture.h"
#include "GraphicsInputLayout.h"
#include "GraphicsSamplerState.h"

GraphicsDevice::GraphicsDevice(ID3D11Device* _pDevice, IDXGIFactory* _pFactory, IDXGIAdapter3* _pAdapter)
    : mDevice(nullptr)
    , mFactory(nullptr)
    , mAdapter(nullptr)
{
	if (!_pDevice)
	{
        throw std::runtime_error("Hresult Failed to Create GraphicsDevice.");
	}
	else
	{
		if (_pDevice)
			mDevice = _pDevice;
		if (_pFactory)
			mFactory = _pFactory;
		if (_pAdapter)
			mAdapter = _pAdapter;
	}
}

GraphicsDevice::~GraphicsDevice()
{
}

UTID GraphicsDevice::GetTypeID()
{
    return _UTID_GRAPHICS_DEVICE;
}

HRESULT GraphicsDevice::QueryInterface(const UTID _UTID, void** _ppObject)
{
    if (_ppObject == nullptr)
    {
        return E_INVALIDARG;
    }
    *_ppObject = nullptr;
    switch (_UTID)
    {
    case _UTID_GRAPHICS_DEVICE:
        *_ppObject = static_cast<GraphicsDevice*>(this);
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

void GraphicsDevice::Release()
{
	if (mDevice)
		mDevice->Release();
	mDevice = nullptr;
	if (mFactory)
		mFactory->Release();
	if (mAdapter)
		mAdapter->Release();

    delete this;
}

HRESULT GraphicsDevice::CreateRenderTarget(HWND _hwnd, IGraphicsRenderTarget** _ppRenderTarget)
{
	if (!_hwnd || !_ppRenderTarget)
	{
		return E_INVALIDARG;
	}
	GraphicsRenderTarget* pRenderTarget;
	pRenderTarget = new GraphicsRenderTarget(this, _hwnd);
	if (pRenderTarget)
	{
		(*_ppRenderTarget) = pRenderTarget;
        return S_OK;
	}
    else
    {
        return E_OUTOFMEMORY;
    }
    return E_FAIL;
}

HRESULT GraphicsDevice::CreateShader(eShaderStage _type, const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel, IShader** _ppShader)
{
    if (_ppShader == nullptr ||
        _shaderModel == nullptr ||
        _entryPoint == nullptr)
    {
        return E_INVALIDARG;
    }
    IShader* pShader = nullptr;
    switch (_type)
    {
    case eShaderStage::PASS:
        return E_INVALIDARG;
        break;
    case eShaderStage::VS:
        if(_entryPoint == "")
            pShader = new GraphicsVertexShader(_path, this);
        else
            pShader = new GraphicsVertexShader(_path, _entryPoint, _shaderModel, this);
        break;
    case eShaderStage::PS:
        if (_entryPoint == "")
            pShader = new GraphicsPixelShader(_path, this);
        else
            pShader = new GraphicsPixelShader(_path, _entryPoint, _shaderModel, this);
        break;
    case eShaderStage::GS:
        return E_NOTIMPL;
        break;
    case eShaderStage::HS:
        return E_NOTIMPL;
        break;
    case eShaderStage::DS:
        return E_NOTIMPL;
        break;
    case eShaderStage::CS:
        return E_NOTIMPL;
        break;
    case eShaderStage::ALL:
        return E_NOTIMPL;
        break;
    default:
        break;
    }
    if (pShader)
    {
        (*_ppShader) = pShader;
        return S_OK;
    }
    else
    {
        return E_FAIL;
    }
    return E_FAIL;
}

HRESULT GraphicsDevice::CreateTexture2D(const WCHAR* _path, ITexture2D** _ppTex2D)
{
    if (!_ppTex2D)
    {
        return E_INVALIDARG;
    }
    GraphicsTexture2D* pTexture2D;
    pTexture2D = new GraphicsTexture2D(_path, this);
    if (pTexture2D)
    {
        (*_ppTex2D) = pTexture2D;
        return S_OK;
    }
    else
    {
        return E_OUTOFMEMORY;
    }
    return E_FAIL;
}

HRESULT GraphicsDevice::CreateBuffer(const WCHAR* _name, const BufferDesc* _pBufDesc, const SubResourceData* _pSubRscDesc, void** _ppBuffer)
{
    if (_ppBuffer == nullptr)
    {
        return E_INVALIDARG;
    }
    switch (_pBufDesc->BindFlags)
    {
    case D3D11_BIND_VERTEX_BUFFER: {
        GraphicsVertexBuffer* pVertexBuffer = new GraphicsVertexBuffer(_name, _pBufDesc, _pSubRscDesc, this);
        if (pVertexBuffer) (*_ppBuffer) = pVertexBuffer;
        else return E_FAIL;
        break;
    }
    case D3D11_BIND_INDEX_BUFFER: {
        GraphicsIndexBuffer* pIndexBuffer = new GraphicsIndexBuffer(_name, _pBufDesc, _pSubRscDesc, this);
        if (pIndexBuffer) (*_ppBuffer) = pIndexBuffer;
        else return E_FAIL;
        break;
    }
    case D3D11_BIND_CONSTANT_BUFFER: {
        GraphicsConstantBuffer* pConstantBuffer = new GraphicsConstantBuffer(_name, _pBufDesc, _pSubRscDesc, this);
        if (pConstantBuffer) (*_ppBuffer) = pConstantBuffer;
        else return E_FAIL;
        break;
    }
    default:
        return E_INVALIDARG;
    }
    return S_OK;
}

HRESULT GraphicsDevice::CreateInputLayout(const WCHAR* _name, const InputLayoutDesc* _pIADesc, UINT _arrSize, IVertexShader* _pVertexShader, IInputLayout** _ppInputLayout)
{
   /* if (!_pVertexShader || !_pIADesc)
    {
        return E_INVALIDARG;
    }
    GraphicsInputLayout* pInputLayout;
    pInputLayout = new GraphicsInputLayout(_name, _pIADesc, _arrSize, _pVertexShader, this);
    if (pInputLayout)
    {
        (*_ppInputLayout) = pInputLayout;
        return S_OK;
    }
    else
    {
        return E_OUTOFMEMORY;
    }
    return E_FAIL;*/
    return E_NOTIMPL;
}

HRESULT GraphicsDevice::CreateSamplerState(const WCHAR* _name, SamplerDesc* _pDesc, ISamplerState** _ppSamplerState)
{
    if (!_pDesc || !_ppSamplerState)
    {
        return E_INVALIDARG;
    }
    GraphicsSamplerState* pSamplerState;
    pSamplerState = new GraphicsSamplerState(_name, _pDesc, this);
    if (pSamplerState)
    {
        (*_ppSamplerState) = pSamplerState;
        return S_OK;
    }
    else
    {
        return E_OUTOFMEMORY;
    }
    return E_FAIL;
}
