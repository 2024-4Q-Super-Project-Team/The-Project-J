#include "pch.h"
#include "D3DGraphicsBuffer.h"
#include "Graphics/Core/D3DGraphicsDevice.h"
#include "Graphics/Core/D3DGraphicsRenderer.h"

D3DGraphicsBuffer::D3DGraphicsBuffer(const D3D11_BUFFER_DESC* _pBufferDesc, D3D11_SUBRESOURCE_DATA* _pBufferSubDesc)
{
    auto pDevice = D3DGraphicsDevice::GetDevice();
    if (_pBufferSubDesc)
    {
        Helper::HRT(pDevice->CreateBuffer(_pBufferDesc, _pBufferSubDesc, &mBuffer));
    }
    else
    {
        Helper::HRT(pDevice->CreateBuffer(_pBufferDesc, nullptr, &mBuffer));
    }
}

D3DGraphicsBuffer::~D3DGraphicsBuffer()
{
    SAFE_RELEASE(mBuffer);
}

void D3DGraphicsBuffer::Release()
{
    SAFE_RELEASE(mBuffer);
    delete this;
}

D3DGraphicsVertexBuffer::D3DGraphicsVertexBuffer(const D3D11_BUFFER_DESC* _pBufferDesc, D3D11_SUBRESOURCE_DATA* _pBufferSubDesc)
    : D3DGraphicsBuffer(_pBufferDesc, _pBufferSubDesc)
{
}

HRESULT D3DGraphicsVertexBuffer::Create()
{
    return S_OK;
}

HRESULT D3DGraphicsVertexBuffer::Bind()
{
    auto pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    if (pDeviceContext)
    {
        pDeviceContext->IASetVertexBuffers(0, 1, &mBuffer, &mStride, &mOffset);
        return S_OK;
    }
    return E_FAIL;
}

HRESULT D3DGraphicsVertexBuffer::Reset()
{
    ID3D11Buffer* nullBuffer = nullptr;
    auto pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    if (pDeviceContext)
    {
        pDeviceContext->IASetVertexBuffers(0, 1, &nullBuffer, &mStride, &mOffset);
        return S_OK;
    }
    return E_FAIL;
}

D3DGraphicsIndexBuffer::D3DGraphicsIndexBuffer(const D3D11_BUFFER_DESC* _pBufferDesc, D3D11_SUBRESOURCE_DATA* _pBufferSubDesc)
    : D3DGraphicsBuffer(_pBufferDesc, _pBufferSubDesc)
{
}

HRESULT D3DGraphicsIndexBuffer::Create()
{
    return S_OK;
}

HRESULT D3DGraphicsIndexBuffer::Bind()
{
    auto pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    if (pDeviceContext)
    {
        pDeviceContext->IASetIndexBuffer(mBuffer, mFormat, mOffset);
        return S_OK;
    }
    return E_FAIL;
}

HRESULT D3DGraphicsIndexBuffer::Reset()
{
    auto pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    if (pDeviceContext)
    {
        pDeviceContext->IASetIndexBuffer(nullptr, mFormat, mOffset);
        return S_OK;
    }
    return E_FAIL;
}

D3DGraphicsConstantBuffer::D3DGraphicsConstantBuffer(const D3D11_BUFFER_DESC* _pBufferDesc, D3D11_SUBRESOURCE_DATA* _pBufferSubDesc)
    : D3DGraphicsBuffer(_pBufferDesc, _pBufferSubDesc)
{
}

HRESULT D3DGraphicsConstantBuffer::Create()
{
    return S_OK;
}

HRESULT D3DGraphicsConstantBuffer::Bind()
{
    auto pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    if (pDeviceContext)
    {
        switch (mStage)
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
    return E_FAIL;
}

HRESULT D3DGraphicsConstantBuffer::Reset()
{
    ID3D11Buffer* nullBuffer = nullptr;
    auto pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    if (pDeviceContext)
    {
        switch (mStage)
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
    return E_FAIL;
}

void D3DGraphicsConstantBuffer::UpdateGPUResoure(IN const void* _srcData)
{
    auto pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    if (pDeviceContext)
    {
        pDeviceContext->UpdateSubresource(mBuffer, 0, nullptr, _srcData, 0, 0);
    }
}
