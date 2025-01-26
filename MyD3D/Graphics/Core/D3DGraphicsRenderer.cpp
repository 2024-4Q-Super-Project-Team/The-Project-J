#include "pch.h"
#include "Graphics/Core/D3DGraphicsRenderer.h"
#include "Graphics/Core/D3DGraphicsDevice.h"
#include "Graphics/Core/D3DGraphicsRenderTarget.h"
#include "Graphics/GPUResource/D3DGraphicsTexture.h"

ID3D11DeviceContext* D3DGraphicsRenderer::mDeviceContext = nullptr;

BOOL D3DGraphicsRenderer::Initialize()
{
    mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    return TRUE;
}

void D3DGraphicsRenderer::Finalization()
{
    SAFE_RELEASE(mDeviceContext);
}

void D3DGraphicsRenderer::UseDepthTest(bool _bValue)
{
}

void D3DGraphicsRenderer::SetTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
    mDeviceContext->IASetPrimitiveTopology(topology);
}

void D3DGraphicsRenderer::DrawCall(UINT _numIndex, UINT _startIndex, INT _baseVertex)
{
    if (mDeviceContext)
        mDeviceContext->DrawIndexed(_numIndex, _startIndex, _baseVertex);
    else throw std::runtime_error("NullReference Exeption : D3DGraphicsRenderer::mDeviceContext");
}

void D3DGraphicsRenderer::DrawCall(IN UINT _numIndex, INT _baseVertex)
{
    if (mDeviceContext)
        mDeviceContext->Draw(_numIndex, _baseVertex);
    else throw std::runtime_error("NullReference Exeption : D3DGraphicsRenderer::mDeviceContext");
}

void D3DGraphicsRenderer::DrawVertexCall(IN UINT _vertexCount, IN UINT _startVertexLocation)
{
    if (mDeviceContext)
        mDeviceContext->Draw(_vertexCount, _startVertexLocation);
    else throw std::runtime_error("NullReference Exeption : D3DGraphicsRenderer::mDeviceContext");
}