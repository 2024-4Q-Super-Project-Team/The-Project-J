#include "pch.h"
#include "Mesh.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/ResourceManager.h"
#include "Resource/Graphics/Material/Material.h"
// Editor
#include "Editor/EditorManager.h"

// ���ҽ��Ŵ������� ����
MeshResource* MeshResource::SkyCubeMesh;
MeshResource* MeshResource::CubeMesh;
MeshResource* MeshResource::PlainMesh;

MeshResource::MeshResource(ResourceHandle _handle, std::vector<Vertex>& _vertices, std::vector<UINT>& _indices)
    : Resource(_handle)
{
    SetEID("Mesh : " + Helper::ToString(_handle.GetKey()));
    mVertices = std::move(_vertices);
    mIndices = std::move(_indices);
    // JSON_TODO : �ڵ��� Path��ο� Json������ �ִ��� Ȯ��
    // ������ �ش� JSON�� ���� ����
    // ������ JSON�� ����� �⺻ �� ���
}

MeshResource::~MeshResource()
{
    SAFE_RELEASE(mVertexBuffer);
    SAFE_RELEASE(mIndexBuffer);
}

void MeshResource::Create()
{
    D3D11_BUFFER_DESC bufDesc = {};
    {   // ���ؽ� ����
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
    {   // �ε��� ����
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
}

void MeshResource::Bind()
{
    if (mVertexBuffer && mIndexBuffer)
    {
        mVertexBuffer->Bind();
        mIndexBuffer->Bind();
    }
}

MeshResource* MeshResource::GetSkyCubeMesh()
{
    if (!SkyCubeMesh)
    {
        // ��ī�� ť���� Vertex ������
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


        // ��ī�� ť�� �޽� ����
        ResourceHandle handle = { eResourceType::MeshResource, L"Default_SkyCube_Mesh", L"", L"" };
        SkyCubeMesh = new MeshResource(handle, vertices, indices);
        SkyCubeMesh->Create();
    }
    return SkyCubeMesh;
}


MeshResource* MeshResource::GetCubeMesh()
{
    if (!CubeMesh)
    {
        // ť���� Vertex ������
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

        // ť���� Index ������
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

        ResourceHandle handle = { eResourceType::MeshResource, L"Default_Cube_Mesh", L"", L"" };
        CubeMesh = new MeshResource(handle, vertices, indices);
        CubeMesh->Create();
    }
    return CubeMesh;
}

MeshResource* MeshResource::GetPlainMesh()
{
    if (!PlainMesh)
    {
        // ����� Vertex ������
        std::vector<Vertex> vertices = {
            // �ո� ���� (���: {0, 0, 1})
            Vertex({-1, -1, 0, 1}, {1, 0, 0, 1}, {0, 0, 1}, {1, 0, 0}),
            Vertex({ 1, -1, 0, 1}, {0, 1, 0, 1}, {0, 0, 1}, {1, 0, 0}),
            Vertex({ 1,  1, 0, 1}, {0, 0, 1, 1}, {0, 0, 1}, {1, 0, 0}),
            Vertex({-1,  1, 0, 1}, {1, 1, 0, 1}, {0, 0, 1}, {1, 0, 0}),

            // �޸� ���� (���: {0, 0, -1})
            Vertex({-1, -1, 0, 1}, {1, 0, 0, 1}, {0, 0, -1}, {-1, 0, 0}),
            Vertex({ 1, -1, 0, 1}, {0, 1, 0, 1}, {0, 0, -1}, {-1, 0, 0}),
            Vertex({ 1,  1, 0, 1}, {0, 0, 1, 1}, {0, 0, -1}, {-1, 0, 0}),
            Vertex({-1,  1, 0, 1}, {1, 1, 0, 1}, {0, 0, -1}, {-1, 0, 0}),
        };

        // ����� Index ������ (��� ����)
        std::vector<UINT> indices = {
            // �ո�
            0, 1, 2,
            0, 2, 3,
            // �޸�
            4, 6, 5,
            4, 7, 6
        };

        // ��� �޽� ����
        ResourceHandle handle = { eResourceType::MeshResource, L"Default_Plain_Mesh", L"", L"" };
        PlainMesh = new MeshResource(handle, vertices, indices);
        PlainMesh->Create();
    }
    return PlainMesh;
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
        if (ImGui::Selectable(GetEID(), false, flags))
        {
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                EditorManager::GetInspectorViewer()->SetFocusObject(this);
            }
        }
        EditorItemState state;
        state.mResourcePtr = this;
        state.mName = name;
        EditorDragNDrop::SendDragAndDropData(GetEID(), state);
        EDITOR_COLOR_POP(1);
        break;
    }
    case EditorViewerType::HIERARCHY:
        break;
    case EditorViewerType::INSPECTOR:
    {
        Resource::EditorRendering(_viewerType);
        ImGui::Text("Vertex Count : %d", mVertices.size());
        ImGui::Text("Index Count  : %d", mIndices.size());
        ImGui::Text("Bone Count  : %d", mBoneArray.size());
    }
        break;
    default:
        break;
    }
}