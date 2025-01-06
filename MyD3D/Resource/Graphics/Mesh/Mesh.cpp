#include "pch.h"
#include "Mesh.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"
#include "Graphics/Vertex/Vertex.h"
#include "Resource/Graphics/Material/Material.h"

// 리소스매니저에서 생성
std::shared_ptr<MeshResource> MeshResource::SkyCubeMesh;
std::shared_ptr<MeshResource> MeshResource::CubeMesh;
std::shared_ptr<MeshResource> MeshResource::PlainMesh;

MeshResource::MeshResource(std::wstring_view _name, std::vector<Vertex>& _vertices, std::vector<UINT>& _indices)
    : Resource(_name)
{
    mVertices   = std::move(_vertices);
    mIndices    = std::move(_indices);
}

MeshResource::~MeshResource()
{
    SAFE_RELEASE(mVertexBuffer);
    SAFE_RELEASE(mIndexBuffer);
}

HRESULT MeshResource::Create()
{
    IGraphicsDevice* pDevice = GraphicsManager::GetDevice();
    if (!pDevice)
    {
        throw std::runtime_error("Null Pointer Exception to MeshResource::MeshResource...GraphicsManager::GetDevice()");
    }
    BufferDesc bufDesc = {};
    {   // 버텍스 버퍼
        bufDesc.BufferStride = sizeof(Vertex);
        bufDesc.BufferOffset = 0;
        bufDesc.ByteWidth = (UINT)(sizeof(Vertex) * mVertices.size());
        bufDesc.Usage = D3D11_USAGE_DEFAULT;
        bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufDesc.CPUAccessFlags = 0;
        SubResourceData vbData = {};
        vbData.pSysMem = mVertices.data();
        std::wstring bufferName = mName + L"_VertexBuffer";
        HRESULT hr = pDevice->CreateBuffer(bufferName.data(), &bufDesc, &vbData,
            (void**)&mVertexBuffer);
        if (FAILED(hr))
        {
            throw std::runtime_error("Hresult Failed to MeshResource::Create...CreateBuffer()");
        }
    }
    {   // 인덱스 버퍼
        bufDesc.BufferFormat = DXGI_FORMAT_R32_UINT;
        bufDesc.BufferOffset = 0;
        bufDesc.ByteWidth = (UINT)(sizeof(UINT) * mIndices.size());
        bufDesc.Usage = D3D11_USAGE_DEFAULT;
        bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufDesc.CPUAccessFlags = 0;
        SubResourceData ibData = {};
        ibData.pSysMem = mIndices.data();
        std::wstring bufferName = mName + L"_IndexBuffer";
        HRESULT hr = pDevice->CreateBuffer(bufferName.data(), &bufDesc, &ibData,
            (void**)&mIndexBuffer);
        if (FAILED(hr))
        {
            throw std::runtime_error("Hresult Failed to MeshResource::Create...CreateBuffer()");
        }
    }
    return S_OK;
}

void MeshResource::Bind()
{
    auto pRenderer = GraphicsManager::GetRenderer();
    if (!pRenderer) return;
    mVertexBuffer->Bind(pRenderer);
    mIndexBuffer->Bind(pRenderer);
}

void MeshResource::InitSkyCubeMesh()
{
    // 스카이 큐브의 Vertex 데이터
    std::vector<Vertex> vertices = {
        Vertex({ 1, -1, 1, 1 },     { 1, 1, 1, 1 },     { -1, 1, 1 }),
        Vertex({ -1, -1, 1, 1 },    { 1, 1, 1, 1 },     { -1, 1, 1 }),
        Vertex({ -1, -1, -1, 1 },   { 1, 1, 1, 1 },     { -1, 1, 1 }),
        Vertex({ 1, -1, -1, 1 },    { 1, 1, 1, 1 },     { -1, 1, 1 }),
        
        Vertex({ 1, 1, 1, 1 },      { 1, 1, 1, 1 },     { -1, -1, 1 }),
        Vertex({ 1, 1, -1, 1 },     { 1, 1, 1, 1 },     { -1, -1, 1 }),
        Vertex({ -1, 1, -1, 1 },    { 1, 1, 1, 1 },     { -1, -1, 1 }),
        Vertex({ -1, 1, 1, 1 },     { 1, 1, 1, 1 },     { -1, -1, 1 }),
        
        Vertex({ 1, -1, 1, 1 },     { 1, 1, 1, 1 },     { -1, -1, 1 }),
        Vertex({ 1, -1, -1, 1 },    { 1, 1, 1, 1 },     { -1, -1, 1 }),
        Vertex({ 1, 1, -1, 1 },     { 1, 1, 1, 1 },     { -1, -1, 1 }),
        Vertex({ 1, 1, 1, 1 },      { 1, 1, 1, 1 },     { -1, -1, 1 }),
        
        Vertex({ 1, -1, -1, 1 },    { 1, 1, 1, 1 },     { -1, -1, 1 }),
        Vertex({ -1, -1, -1, 1 },   { 1, 1, 1, 1 },     { -1, -1, 1 }),
        Vertex({ -1, 1, -1, 1 },    { 1, 1, 1, 1 },     { -1, -1, 1 }),
        Vertex({ 1, 1, -1, 1 },     { 1, 1, 1, 1 },     { -1, -1, 1 }),
        
        Vertex({ -1, -1, -1, 1 },   { 1, 1, 1, 1 },     { 1, -1, 1 }),
        Vertex({ -1, -1, 1, 1 },    { 1, 1, 1, 1 },     { 1, -1, 1 }),
        Vertex({ -1, 1, 1, 1 },     { 1, 1, 1, 1 },     { 1, -1, 1 }),
        Vertex({ -1, 1, -1, 1 },    { 1, 1, 1, 1 },     { 1, -1, 1 }),
        
        Vertex({ 1, 1, 1, 1 },      { 1, 1, 1, 1 },     { -1, -1, -1 }),
        Vertex({ -1, 1, 1, 1 },     { 1, 1, 1, 1 },     { -1, -1, -1 }),
        Vertex({ -1, -1, 1, 1 },    { 1, 1, 1, 1 },     { -1, -1, -1 }),
        Vertex({ 1, -1, 1, 1 },     { 1, 1, 1, 1 },     { -1, -1, -1 }),
    };

    std::vector<UINT> indices = {
    3, 2, 1, 3, 1, 0,  // Front face
    7, 6, 5, 7, 5, 4,  // Back face
    11, 10, 9, 11, 9, 8,  // Left face
    15, 14, 13, 15, 13, 12,  // Right face
    19, 18, 17, 19, 17, 16,  // Top face
    23, 22, 21, 23, 21, 20   // Bottom face
    };


    // 스카이 큐브 메쉬 생성
    SkyCubeMesh = std::make_shared<MeshResource>(L"Default_SkyCube_Mesh", vertices, indices);
    SkyCubeMesh->Create();
}


void MeshResource::InitCubeMesh()
{
    // 큐브의 Vertex 데이터
    std::vector<Vertex> vertices = {
        // Front face
        Vertex({-1, -1,  1, 1}, {1, 0, 0, 1}, { 0,  0,  1}, { 1,  0,  0}),
        Vertex({ 1, -1,  1, 1}, {0, 1, 0, 1}, { 0,  0,  1}, { 1,  0,  0}),
        Vertex({ 1,  1,  1, 1}, {0, 0, 1, 1}, { 0,  0,  1}, { 1,  0,  0}),
        Vertex({-1,  1,  1, 1}, {1, 1, 0, 1}, { 0,  0,  1}, { 1,  0,  0}),

        // Back face
        Vertex({-1, -1, -1, 1}, {1, 0, 1, 1}, { 0,  0, -1}, {-1,  0,  0}),
        Vertex({ 1, -1, -1, 1}, {0, 1, 1, 1}, { 0,  0, -1}, {-1,  0,  0}),
        Vertex({ 1,  1, -1, 1}, {1, 1, 1, 1}, { 0,  0, -1}, {-1,  0,  0}),
        Vertex({-1,  1, -1, 1}, {0, 0, 0, 1}, { 0,  0, -1}, {-1,  0,  0}),

        // Left face
        Vertex({-1, -1, -1, 1}, {1, 0, 0, 1}, {-1,  0,  0}, { 0,  0, -1}),
        Vertex({-1, -1,  1, 1}, {0, 1, 0, 1}, {-1,  0,  0}, { 0,  0, -1}),
        Vertex({-1,  1,  1, 1}, {0, 0, 1, 1}, {-1,  0,  0}, { 0,  0, -1}),
        Vertex({-1,  1, -1, 1}, {1, 1, 0, 1}, {-1,  0,  0}, { 0,  0, -1}),

        // Right face
        Vertex({ 1, -1, -1, 1}, {1, 0, 1, 1}, { 1,  0,  0}, { 0,  0,  1}),
        Vertex({ 1, -1,  1, 1}, {0, 1, 1, 1}, { 1,  0,  0}, { 0,  0,  1}),
        Vertex({ 1,  1,  1, 1}, {1, 1, 1, 1}, { 1,  0,  0}, { 0,  0,  1}),
        Vertex({ 1,  1, -1, 1}, {0, 0, 0, 1}, { 1,  0,  0}, { 0,  0,  1}),

        // Top face
        Vertex({-1,  1, -1, 1}, {1, 0, 0, 1}, { 0,  1,  0}, { 1,  0,  0}),
        Vertex({-1,  1,  1, 1}, {0, 1, 0, 1}, { 0,  1,  0}, { 1,  0,  0}),
        Vertex({ 1,  1,  1, 1}, {0, 0, 1, 1}, { 0,  1,  0}, { 1,  0,  0}),
        Vertex({ 1,  1, -1, 1}, {1, 1, 0, 1}, { 0,  1,  0}, { 1,  0,  0}),

        // Bottom face
        Vertex({-1, -1, -1, 1}, {1, 0, 1, 1}, { 0, -1,  0}, { 1,  0,  0}),
        Vertex({-1, -1,  1, 1}, {0, 1, 1, 1}, { 0, -1,  0}, { 1,  0,  0}),
        Vertex({ 1, -1,  1, 1}, {1, 1, 1, 1}, { 0, -1,  0}, { 1,  0,  0}),
        Vertex({ 1, -1, -1, 1}, {0, 0, 0, 1}, { 0, -1,  0}, { 1,  0,  0}),
    };

    // 큐브의 Index 데이터
    std::vector<UINT> indices = {
        // Front face
        0, 1, 2, 0, 2, 3,
        // Back face
        4, 5, 6, 4, 6, 7,
        // Left face
        8, 9, 10, 8, 10, 11,
        // Right face
        12, 13, 14, 12, 14, 15,
        // Top face
        16, 17, 18, 16, 18, 19,
        // Bottom face
        20, 21, 22, 20, 22, 23
    };

    CubeMesh = std::make_shared<MeshResource>(L"Default_Cube_Mesh", vertices, indices);
    CubeMesh->Create();
}

void MeshResource::InitPlainMesh()
{
    // 평면의 Vertex 데이터
    std::vector<Vertex> vertices = {
        // 평면의 정점 (정점 위치, 색상, 노멀, 탄젠트)
        Vertex({-1, -1, 0, 1}, {1, 0, 0, 1}, {0, 0, 1}, {1, 0, 0}),
        Vertex({ 1, -1, 0, 1}, {0, 1, 0, 1}, {0, 0, 1}, {1, 0, 0}),
        Vertex({ 1,  1, 0, 1}, {0, 0, 1, 1}, {0, 0, 1}, {1, 0, 0}),
        Vertex({-1,  1, 0, 1}, {1, 1, 0, 1}, {0, 0, 1}, {1, 0, 0}),
    };

    // 평면의 Index 데이터 (양면 정의)
    std::vector<UINT> indices = {
        // 앞면
        0, 1, 2,
        0, 2, 3,
        // 뒷면 (역방향 인덱스)
        2, 1, 0,
        3, 2, 0
    };

    // 평면 메쉬 생성
    PlainMesh = std::make_shared<MeshResource>(L"Default_Plain_Mesh", vertices, indices);
    PlainMesh->Create();
}


/*
 {
Vertex({ 1, -1, 1, 1 },     { 1, 1, 1, 1 },     { -1, 1, 1 }    ),
Vertex({ -1, -1, 1, 1 },    { 1, 1, 1, 1 },     { -1, 1, 1 }    ),
Vertex({ -1, -1, -1, 1 },   { 1, 1, 1, 1 },     { -1, 1, 1 }    ),
Vertex({ 1, -1, -1, 1 },    { 1, 1, 1, 1 },     { -1, 1, 1 }    ),

Vertex({ 1, 1, 1, 1 },      { 1, 1, 1, 1 },     { -1, -1, 1 }   ),
Vertex({ 1, 1, -1, 1 },     { 1, 1, 1, 1 },     { -1, -1, 1 }   ),
Vertex({ -1, 1, -1, 1 },    { 1, 1, 1, 1 },     { -1, -1, 1 }   ),
Vertex({ -1, 1, 1, 1 },     { 1, 1, 1, 1 },     { -1, -1, 1 }   ),

Vertex({ 1, -1, 1, 1 },     { 1, 1, 1, 1 },     { -1, -1, 1 }   ),
Vertex({ 1, -1, -1, 1 },    { 1, 1, 1, 1 },     { -1, -1, 1 }   ),
Vertex({ 1, 1, -1, 1 },     { 1, 1, 1, 1 },     { -1, -1, 1 }   ),
Vertex({ 1, 1, 1, 1 },      { 1, 1, 1, 1 },     { -1, -1, 1 }   ),

Vertex({ 1, -1, -1, 1 },    { 1, 1, 1, 1 },     { -1, -1, 1 }   ),
Vertex({ -1, -1, -1, 1 },   { 1, 1, 1, 1 },     { -1, -1, 1 }   ),
Vertex({ -1, 1, -1, 1 },    { 1, 1, 1, 1 },     { -1, -1, 1 }   ),
Vertex({ 1, 1, -1, 1 },     { 1, 1, 1, 1 },     { -1, -1, 1 }   ),

Vertex({ -1, -1, -1, 1 },   { 1, 1, 1, 1 },     { 1, -1, 1 }    ),
Vertex({ -1, -1, 1, 1 },    { 1, 1, 1, 1 },     { 1, -1, 1 }    ),
Vertex({ -1, 1, 1, 1 },     { 1, 1, 1, 1 },     { 1, -1, 1 }    ),
Vertex({ -1, 1, -1, 1 },    { 1, 1, 1, 1 },     { 1, -1, 1 }    ),

Vertex({ 1, 1, 1, 1 },      { 1, 1, 1, 1 },     { -1, -1, -1 }  ),
Vertex({ -1, 1, 1, 1 },     { 1, 1, 1, 1 },     { -1, -1, -1 }  ),
Vertex({ -1, -1, 1, 1 },    { 1, 1, 1, 1 },     { -1, -1, -1 }  ),
Vertex({ 1, -1, 1, 1 },     { 1, 1, 1, 1 },     { -1, -1, -1 }  ),
}
*/