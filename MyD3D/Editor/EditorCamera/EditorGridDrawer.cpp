#include "pch.h"
#include "EditorGridDrawer.h"
#include "Graphics/GraphicsManager.h"

std::vector<GridVertex>     EditorGridDrawer::mVertexies;
D3DGraphicsVertexBuffer*    EditorGridDrawer::mVsBuffer = nullptr;

BOOL EditorGridDrawer::Initialize()
{
	return 0;
}

void EditorGridDrawer::Finalization()
{
	SAFE_RELEASE(mVsBuffer);
}

void EditorGridDrawer::SetGrid(UINT _size, UINT _unit)
{
    std::vector<GridVertex> Vertexies;

    SAFE_RELEASE(mVsBuffer);
    mVertexies.clear();
	// XZ ��鿡 �׸��� ����
	float halfSize = _size * _unit * 0.5f;

    // Z�� ���� ��
    for (int i = 0; i <= _size; ++i)
    {
        float position = -halfSize + i * _unit;
        Vertexies.emplace_back(Vector4(position, 0.0f, -halfSize, 1.0f), Vector4(0.5f, 0.5f, 0.5f, 1.0f));
        Vertexies.emplace_back(Vector4(position, 0.0f,  halfSize, 1.0f), Vector4(0.5f, 0.5f, 0.5f, 1.0f));
    }
    
    // X�� ���� ��
    for (int i = 0; i <= _size; ++i)
    {
        float position = -halfSize + i * _unit;
        Vertexies.emplace_back(Vector4(-halfSize, 0.0f, position, 1.0f), Vector4(0.5f, 0.5f, 0.5f, 1.0f));
        Vertexies.emplace_back(Vector4( halfSize, 0.0f, position, 1.0f), Vector4(0.5f, 0.5f, 0.5f, 1.0f));
    }

    // X�� ���� �� (������)
    float thickness = 0.1f; // �� �β��� ���� ������
    for (float offset = -thickness; offset <= thickness; offset += 0.05f)
    {
        Vertexies.emplace_back(Vector4(-halfSize, 0.0f, offset, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f)); // ������
        Vertexies.emplace_back(Vector4(halfSize, 0.0f, offset, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));  // ����
    }

    // Z�� ���� �� (�ʷϻ�)
    for (float offset = -thickness; offset <= thickness; offset += 0.05f)
    {
        Vertexies.emplace_back(Vector4(offset, 0.0f, -halfSize, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f)); // ������
        Vertexies.emplace_back(Vector4(offset, 0.0f, halfSize, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f));  // ����
    }

    mVertexies = std::move(Vertexies);
    // ���� ���� ����
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = (UINT)(sizeof(GridVertex) * mVertexies.size());
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = mVertexies.data();

    mVsBuffer = new D3DGraphicsVertexBuffer(&bufferDesc, &initData);
    mVsBuffer->SetStride(sizeof(GridVertex));
}

void EditorGridDrawer::DrawGrid()
{
    if (mVsBuffer)
    {
        D3DGraphicsRenderer::SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        GraphicsManager::GetVertexShader(eVertexShaderType::GRID)->Bind();
        GraphicsManager::GetPixelShader(ePixelShaderType::GRID)->Bind();

        mVsBuffer->Bind();
        D3DGraphicsRenderer::DrawCall(mVertexies.size(),0);

        GraphicsManager::GetVertexShader(eVertexShaderType::GRID)->Reset();
        GraphicsManager::GetPixelShader(ePixelShaderType::GRID)->Reset();
        D3DGraphicsRenderer::SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }
}
