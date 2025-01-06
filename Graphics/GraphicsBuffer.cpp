#include "pch.h"
#include "GraphicsBuffer.h"
#include "GraphicsDevice.h"
#include "GraphicsRenderer.h"

GraphicsBuffer::GraphicsBuffer(const WCHAR* _name, const BufferDesc* _pBufferDesc, const SubResourceData* _pBufferSubDesc, GraphicsDevice* _pDevice)
    : mName(_name)
    , mBuffer(nullptr)
{
    if (!_pBufferDesc)
    {
        throw std::runtime_error("Hresult Failed to InvalidArgs GraphicsBuffer::GraphicsBuffer.");
    }
    ID3D11Device* pDevice = _pDevice->mDevice;
    if (pDevice)
    {
        D3D11_BUFFER_DESC bufDesc       = {};
        HRESULT hr;
        bufDesc.ByteWidth               = _pBufferDesc->ByteWidth;
        bufDesc.Usage                   = _pBufferDesc->Usage;
        bufDesc.BindFlags               = _pBufferDesc->BindFlags;
        bufDesc.CPUAccessFlags          = _pBufferDesc->CPUAccessFlags;
        bufDesc.MiscFlags               = _pBufferDesc->MiscFlags;
        bufDesc.StructureByteStride     = _pBufferDesc->StructureByteStride;
        if (_pBufferSubDesc)
        {
            D3D11_SUBRESOURCE_DATA subData = {};
            subData.pSysMem             = _pBufferSubDesc->pSysMem;
            subData.SysMemPitch         = _pBufferSubDesc->SysMemPitch;
            subData.SysMemSlicePitch    = _pBufferSubDesc->SysMemSlicePitch;
            hr = pDevice->CreateBuffer(&bufDesc, &subData, &mBuffer);
        }
        else
        {
            hr = pDevice->CreateBuffer(&bufDesc, nullptr, &mBuffer);
        }

        if (FAILED(hr))
        {
            if (mBuffer)
                mBuffer->Release();
            mBuffer = nullptr;
            throw std::runtime_error("Hresult Failed to Create GraphicsBuffer::GraphicsBuffer...CreateBuffer()");
        }
    }
}

GraphicsBuffer::~GraphicsBuffer()
{
}
/////////////////////////////////////////////////////////////////////////////////////////
/// VertexBuffer ////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
GraphicsVertexBuffer::GraphicsVertexBuffer(
    const WCHAR* _name,
    const BufferDesc* _pBufferDesc,
    const SubResourceData* _pBufferSubDesc,
    GraphicsDevice* _pDevice)
    : GraphicsBuffer(_name, _pBufferDesc, _pBufferSubDesc, _pDevice)
    , mStride(_pBufferDesc->BufferStride)
    , mOffset(_pBufferDesc->BufferOffset)
    , mSlot(0)
{
}
GraphicsVertexBuffer::~GraphicsVertexBuffer()
{
}
UTID GraphicsVertexBuffer::GetTypeID()
{
    return _UTID_GRAPHICS_VERTEXBUFFER;
}

HRESULT GraphicsVertexBuffer::QueryInterface(const UTID _UTID, void** _ppObject)
{
    if (_ppObject == nullptr)
    {
        return E_INVALIDARG;
    }
    *_ppObject = nullptr;
    switch (_UTID)
    {
    case _UTID_GRAPHICS_VERTEXBUFFER:
        *_ppObject = static_cast<GraphicsVertexBuffer*>(this);
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

void GraphicsVertexBuffer::Release()
{
    if (mBuffer)
        mBuffer->Release();
    mBuffer = nullptr;

    delete this;
}

const WCHAR* GraphicsVertexBuffer::GetName()
{
    return mName;
}

HRESULT GraphicsVertexBuffer::Bind(IGraphicsRenderer* _renderer)
{
    if (_renderer == nullptr)
    {
        return E_INVALIDARG;
    }
    GraphicsRenderer* pRenderer;
    if(S_OK == _renderer->QueryInterface(_UTID_GRAPHICS_RENDERER, (void**)&pRenderer))
    {
        pRenderer->mDeviceContext->IASetVertexBuffers(0, 1, &mBuffer, &mStride, &mOffset);
    }
    else
    {
        return E_INVALIDARG;
    }
    return S_OK;
}

HRESULT GraphicsVertexBuffer::Reset(IGraphicsRenderer* _renderer)
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
        ID3D11Buffer* nullBuffer = nullptr;
        pRenderer->mDeviceContext->IASetVertexBuffers(0, 1, &nullBuffer, &mStride, &mOffset);
    }
    else
    {
        return E_INVALIDARG;
    }
    return S_OK;
}

IVertexBuffer& GraphicsVertexBuffer::SetSlot(UINT _startSlot)
{
    mSlot = _startSlot;
    return *this;
}

IVertexBuffer& GraphicsVertexBuffer::SetStride(UINT _stride)
{
    mStride = _stride;
    return *this;
}

IVertexBuffer& GraphicsVertexBuffer::SetOffset(UINT _offset)
{
    mOffset = _offset;
    return *this;
}

UINT GraphicsVertexBuffer::GetStride()
{
    return mStride;
}

UINT GraphicsVertexBuffer::GetOffset()
{
    return mOffset;
}
/////////////////////////////////////////////////////////////////////////////////////////
/// IndexBuffer /////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
GraphicsIndexBuffer::GraphicsIndexBuffer(
    const WCHAR* _name,
    const BufferDesc* _pBufferDesc,
    const SubResourceData* _pBufferSubDesc,
    GraphicsDevice* _pDevice)
    : GraphicsBuffer(_name, _pBufferDesc, _pBufferSubDesc, _pDevice)
    , mFormat(_pBufferDesc->BufferFormat)
    , mOffset(_pBufferDesc->BufferOffset)
{
}
GraphicsIndexBuffer::~GraphicsIndexBuffer()
{
}
UTID GraphicsIndexBuffer::GetTypeID()
{
    return _UTID_GRAPHICS_INDEXBUFFER;
}

HRESULT GraphicsIndexBuffer::QueryInterface(const UTID _UTID, void** _ppObject)
{
    if (_ppObject == nullptr)
    {
        return E_INVALIDARG;
    }
    *_ppObject = nullptr;
    switch (_UTID)
    {
    case _UTID_GRAPHICS_INDEXBUFFER:
        *_ppObject = static_cast<GraphicsIndexBuffer*>(this);
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

void GraphicsIndexBuffer::Release()
{
    if (mBuffer)
        mBuffer->Release();
    mBuffer = nullptr;

    delete this;
}

const WCHAR* GraphicsIndexBuffer::GetName()
{
    return mName;
}

HRESULT GraphicsIndexBuffer::Bind(IGraphicsRenderer* _renderer)
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
        pDeviceContext->IASetIndexBuffer(mBuffer, (DXGI_FORMAT)mFormat, mOffset);
    }
    else
    {
        return E_INVALIDARG;
    }
    return S_OK;
}

HRESULT GraphicsIndexBuffer::Reset(IGraphicsRenderer* _renderer)
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
        ID3D11Buffer* nullBuffer = nullptr;
        pDeviceContext->IASetIndexBuffer(nullBuffer, (DXGI_FORMAT)mFormat, mOffset);
    }
    else
    {
        return E_INVALIDARG;
    }
    return S_OK;
}

IIndexBuffer& GraphicsIndexBuffer::SetFormat(UINT _format)
{
    mFormat = _format;
    return *this;
}

IIndexBuffer& GraphicsIndexBuffer::SetOffset(UINT _offset)
{
    mOffset = _offset;
    return *this;
}

UINT GraphicsIndexBuffer::GetFormat()
{
    return mFormat;
}

UINT GraphicsIndexBuffer::GetOffset()
{
    return mOffset;
}
/////////////////////////////////////////////////////////////////////////////////////////
/// ConstantBuffer //////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
GraphicsConstantBuffer::GraphicsConstantBuffer(
    const WCHAR* _name,
    const BufferDesc* _pBufferDesc, 
    const SubResourceData* _pBufferSubDesc,
    GraphicsDevice* _pDevice)
    : GraphicsBuffer(_name, _pBufferDesc, _pBufferSubDesc, _pDevice)
    , mShaderStage(eShaderStage::PASS)
    , mSlot(0)
{
}
GraphicsConstantBuffer::~GraphicsConstantBuffer()
{
}

UTID GraphicsConstantBuffer::GetTypeID()
{
    return _UTID_GRAPHICS_CONSTANTBUFFER;
}

HRESULT GraphicsConstantBuffer::QueryInterface(const UTID _UTID, void** _ppObject)
{
    if (_ppObject == nullptr)
    {
        return E_INVALIDARG;
    }
    *_ppObject = nullptr;
    switch (_UTID)
    {
    case _UTID_GRAPHICS_CONSTANTBUFFER:
        *_ppObject = static_cast<GraphicsConstantBuffer*>(this);
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

void GraphicsConstantBuffer::Release()
{
    if (mBuffer)
        mBuffer->Release();
    mBuffer = nullptr;

    delete this;
}

const WCHAR* GraphicsConstantBuffer::GetName()
{
    return mName;
}

IConstantBuffer& GraphicsConstantBuffer::SetSlot(UINT _startSlot)
{
    mSlot = _startSlot;
    return *this;
}

IConstantBuffer& GraphicsConstantBuffer::SetShaderStage(eShaderStage _stage)
{
    mShaderStage = _stage;
    return *this;
}

HRESULT GraphicsConstantBuffer::Bind(IGraphicsRenderer* _renderer)
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
            pDeviceContext->VSSetConstantBuffers(mSlot, 1, &mBuffer);
            break;
        case eShaderStage::PS:
            pDeviceContext->PSSetConstantBuffers(mSlot, 1, &mBuffer);
            break;
        case eShaderStage::GS:
            pDeviceContext->GSSetConstantBuffers(mSlot, 1, &mBuffer);
            break;
        case eShaderStage::HS:
            pDeviceContext->HSSetConstantBuffers(mSlot, 1, &mBuffer);
            break;
        case eShaderStage::DS:
            pDeviceContext->DSSetConstantBuffers(mSlot, 1, &mBuffer);
            break;
        case eShaderStage::CS:
            pDeviceContext->CSSetConstantBuffers(mSlot, 1, &mBuffer);
            break;
        case eShaderStage::ALL:
            pDeviceContext->VSSetConstantBuffers(mSlot, 1, &mBuffer);
            pDeviceContext->PSSetConstantBuffers(mSlot, 1, &mBuffer);
            pDeviceContext->GSSetConstantBuffers(mSlot, 1, &mBuffer);
            pDeviceContext->HSSetConstantBuffers(mSlot, 1, &mBuffer);
            pDeviceContext->DSSetConstantBuffers(mSlot, 1, &mBuffer);
            pDeviceContext->CSSetConstantBuffers(mSlot, 1, &mBuffer);
            break;
        default:
            return E_INVALIDARG;
        }
        return S_OK;
    }
    return E_NOINTERFACE;
}

HRESULT GraphicsConstantBuffer::Reset(IGraphicsRenderer* _renderer)
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
        ID3D11Buffer* nullBuffer = nullptr;
        switch (mShaderStage)
        {
        case eShaderStage::PASS:
            break;
        case eShaderStage::VS:
            pDeviceContext->VSSetConstantBuffers(mSlot, 1, &nullBuffer);
            break;
        case eShaderStage::PS:
            pDeviceContext->PSSetConstantBuffers(mSlot, 1, &nullBuffer);
            break;
        case eShaderStage::GS:
            pDeviceContext->GSSetConstantBuffers(mSlot, 1, &nullBuffer);
            break;
        case eShaderStage::HS:
            pDeviceContext->HSSetConstantBuffers(mSlot, 1, &nullBuffer);
            break;
        case eShaderStage::DS:
            pDeviceContext->DSSetConstantBuffers(mSlot, 1, &nullBuffer);
            break;
        case eShaderStage::CS:
            pDeviceContext->CSSetConstantBuffers(mSlot, 1, &nullBuffer);
            break;
        case eShaderStage::ALL:
            pDeviceContext->VSSetConstantBuffers(mSlot, 1, &nullBuffer);
            pDeviceContext->PSSetConstantBuffers(mSlot, 1, &nullBuffer);
            pDeviceContext->GSSetConstantBuffers(mSlot, 1, &nullBuffer);
            pDeviceContext->HSSetConstantBuffers(mSlot, 1, &nullBuffer);
            pDeviceContext->DSSetConstantBuffers(mSlot, 1, &nullBuffer);
            pDeviceContext->CSSetConstantBuffers(mSlot, 1, &nullBuffer);
            break;
        default:
            return E_INVALIDARG;
        }
        return S_OK;
    }
}
