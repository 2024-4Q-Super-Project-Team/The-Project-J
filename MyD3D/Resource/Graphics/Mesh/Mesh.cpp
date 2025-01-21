#include "pch.h"
#include "Mesh.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/ResourceManager.h"
#include "Resource/Graphics/Material/Material.h"
// Editor
#include "Editor/EditorManager.h"

// 리소스매니저에서 생성
std::shared_ptr<MeshResource> MeshResource::SkyCubeMesh;
std::shared_ptr<MeshResource> MeshResource::CubeMesh;
std::shared_ptr<MeshResource> MeshResource::PlainMesh;

MeshResource::MeshResource(ResourceHandle _handle, std::vector<Vertex>& _vertices, std::vector<UINT>& _indices)
    : Resource(_handle)
{
    SetID("Mesh : " + Helper::ToString(_handle.GetKey()));
    mVertices = std::move(_vertices);
    mIndices = std::move(_indices);
    
}

MeshResource::~MeshResource()
{
    SAFE_RELEASE(mVertexBuffer);
    SAFE_RELEASE(mIndexBuffer);
}

HRESULT MeshResource::Create()
{
    D3D11_BUFFER_DESC bufDesc = {};
    {   // 버텍스 버퍼
        bufDesc.ByteWidth = (UINT)(sizeof(Vertex) * mVertices.size());
        bufDesc.Usage = D3D11_USAGE_DEFAULT;
        bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufDesc.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA subData = {};
        subData.pSysMem = mVertices.data();
        mVertexBuffer = new D3DGraphicsVertexBuffer(&bufDesc, &subData);
        mVertexBuffer->SetStride(sizeof(Vertex));
        mVertexBuffer->SetOffset(0);
    }
    {   // 인덱스 버퍼
        bufDesc.ByteWidth = (UINT)(sizeof(UINT) * mIndices.size());
        bufDesc.Usage = D3D11_USAGE_DEFAULT;
        bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufDesc.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA subData = {};
        subData.pSysMem = mIndices.data();
        mIndexBuffer = new D3DGraphicsIndexBuffer(&bufDesc, &subData);
        mIndexBuffer->SetFormat(DXGI_FORMAT_R32_UINT);
        mIndexBuffer->SetOffset(0);
    }
    return S_OK;
}

void MeshResource::Bind()
{
    if (mVertexBuffer && mIndexBuffer)
    {
        mVertexBuffer->Bind();
        mIndexBuffer->Bind();
    }
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
    ResourceHandle handle = { eResourceType::Mesh, L"Default_SkyCube_Mesh", L"", L""};
    SkyCubeMesh = std::make_shared<MeshResource>(handle, vertices, indices);
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

    ResourceHandle handle = { eResourceType::Mesh, L"Default_Cube_Mesh", L"", L"" };
    CubeMesh = std::make_shared<MeshResource>(handle, vertices, indices);
    CubeMesh->Create();
}

void MeshResource::InitPlainMesh()
{
    // 평면의 Vertex 데이터
    std::vector<Vertex> vertices = {
        // 앞면 정점 (노멀: {0, 0, 1})
        Vertex({-1, -1, 0, 1}, {1, 0, 0, 1}, {0, 0, 1}, {1, 0, 0}),
        Vertex({ 1, -1, 0, 1}, {0, 1, 0, 1}, {0, 0, 1}, {1, 0, 0}),
        Vertex({ 1,  1, 0, 1}, {0, 0, 1, 1}, {0, 0, 1}, {1, 0, 0}),
        Vertex({-1,  1, 0, 1}, {1, 1, 0, 1}, {0, 0, 1}, {1, 0, 0}),

        // 뒷면 정점 (노멀: {0, 0, -1})
        Vertex({-1, -1, 0, 1}, {1, 0, 0, 1}, {0, 0, -1}, {-1, 0, 0}),
        Vertex({ 1, -1, 0, 1}, {0, 1, 0, 1}, {0, 0, -1}, {-1, 0, 0}),
        Vertex({ 1,  1, 0, 1}, {0, 0, 1, 1}, {0, 0, -1}, {-1, 0, 0}),
        Vertex({-1,  1, 0, 1}, {1, 1, 0, 1}, {0, 0, -1}, {-1, 0, 0}),
    };

    // 평면의 Index 데이터 (양면 정의)
    std::vector<UINT> indices = {
        // 앞면
        0, 1, 2,
        0, 2, 3,
        // 뒷면
        4, 6, 5,
        4, 7, 6
    };

    // 평면 메쉬 생성
    ResourceHandle handle = { eResourceType::Mesh, L"Default_Plain_Mesh", L"", L"" };
    PlainMesh = std::make_shared<MeshResource>(handle, vertices, indices);
    PlainMesh->Create();
}

void MeshResource::EditorRendering(EditorViewerType _viewerType)
{
    std::string name = Helper::ToString(GetKey());

    switch (_viewerType)
    {
    case EditorViewerType::DEFAULT:
    {
        ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_RESOURCE);
        auto flags = ImGuiSelectableFlags_AllowDoubleClick;
        if (ImGui::Selectable(uid.c_str(), false, flags))
        {
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                EditorManager::GetInspectorViewer()->SetFocusObject(this);
            }
        }
        EditorItemState state;
        state.mResourcePtr = this;
        state.mName = Helper::ToString(mHandle.GetKey());
        EditorDragNDrop::SendDragAndDropData(uid.c_str(), state);
        EDITOR_COLOR_POP(1);
        break;
    }
    case EditorViewerType::HIERARCHY:
        break;
    case EditorViewerType::INSPECTOR:
    {
        ImGui::Text("Vertex Count : %d", mVertices.size());
        ImGui::Text("Index Count  : %d", mIndices.size());
        ImGui::Text("Bone Count  : %d", mBoneArray.size());
    }
        break;
    default:
        break;
    }
}