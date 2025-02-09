#include "pch.h"
#include "D3DGraphicsDefault.h"

D3D11_TEXTURE2D_DESC D3DGraphicsDefault::DefaultTextureDesc = {
	0,0,1,1,DXGI_FORMAT_UNKNOWN,DXGI_SAMPLE_DESC({1,0}),D3D11_USAGE_DEFAULT,0,0
};
D3D11_TEXTURE2D_DESC DefaultDSVTextureDesc = {
};

D3DGraphicsVertexBuffer* D3DGraphicsDefault::QuadFrameVertexBuffer = nullptr;
D3DGraphicsIndexBuffer*  D3DGraphicsDefault::QuadFrameIndexBuffer  = nullptr;
D3DGraphicsVertexBuffer* D3DGraphicsDefault::PointVertexBuffer = nullptr;

D3DGraphicsVertexBuffer* D3DGraphicsDefault::GetPointVertexBuffer()
{
    if (PointVertexBuffer == nullptr)
    {
        unsigned int stride = sizeof(float) * 4;
        unsigned int count = 1;
        float position[4] = { 0,0,0,1 };

        D3D11_BUFFER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //vertex buffer ¿ëµµ
        desc.CPUAccessFlags = 0;
        desc.ByteWidth = stride * count;

        D3D11_SUBRESOURCE_DATA data;
        ZeroMemory(&data, sizeof(data));
        data.pSysMem = &position;

        PointVertexBuffer = new D3DGraphicsVertexBuffer(&desc, &data);
        PointVertexBuffer->Create();

        PointVertexBuffer->SetStride(sizeof(float) * 4);
        PointVertexBuffer->SetOffset(0);
    }
    return PointVertexBuffer;
}

D3DGraphicsVertexBuffer* D3DGraphicsDefault::GetQuadFrameVertexBuffer()
{
    if (QuadFrameVertexBuffer == nullptr)
    {
        QuadFrameVertex vertices[] =
        {
            { {-1.0f,  1.0f, 0.0f, 1.0f}, {0.0f, 0.0f} }, // Top left
            { { 1.0f,  1.0f, 0.0f, 1.0f}, {1.0f, 0.0f} }, // Top right
            { {-1.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 1.0f} }, // Bottom left
            { { 1.0f, -1.0f, 0.0f, 1.0f}, {1.0f, 1.0f} }, // Bottom right
        };
        D3D11_BUFFER_DESC vertexBufferDesc = {};
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = sizeof(QuadFrameVertex) * 6;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA vertexData = {};
        vertexData.pSysMem = vertices;
        QuadFrameVertexBuffer = new D3DGraphicsVertexBuffer(&vertexBufferDesc, &vertexData);
        QuadFrameVertexBuffer->SetStride(sizeof(QuadFrameVertex));
        QuadFrameVertexBuffer->SetOffset(0);
    }
    return QuadFrameVertexBuffer;
}

D3DGraphicsIndexBuffer* D3DGraphicsDefault::GetQuadFrameIndexBuffer()
{
    if (QuadFrameIndexBuffer == nullptr)
    {
        UINT indices[] = { 0, 1, 2, 1, 3, 2 };
        D3D11_BUFFER_DESC indexBufferDesc = {};
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(UINT) * 6;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA indexData = {};
        indexData.pSysMem = indices;
        QuadFrameIndexBuffer = new D3DGraphicsIndexBuffer(&indexBufferDesc, &indexData);
        QuadFrameIndexBuffer->SetFormat(DXGI_FORMAT_R32_UINT);
        QuadFrameIndexBuffer->SetOffset(0);
    }
    return QuadFrameIndexBuffer;
}