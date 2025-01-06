#pragma once
#include "Resource/Resource.h"
#include "Graphics/Vertex/Vertex.h"

class IVertexBuffer;
class IIndexBuffer;
class MaterialResource;
class Material;
class Bone;

class MeshResource : public Resource
{
public:
    RESOURCE_TYPE(eResourceType::Mesh);
    explicit MeshResource(std::wstring_view _name, std::vector<Vertex>& _vertices, std::vector<UINT>& _indices);
    virtual ~MeshResource();
public:
    HRESULT Create();
    void    Bind();
public:
    IVertexBuffer*      mVertexBuffer;
    IIndexBuffer*       mIndexBuffer;
    std::vector<Vertex> mVertices;
    std::vector<UINT>	mIndices;

    // �� ����(Vertex)���� �ε����� ���� �� ������ ������Ű�� ���� ���̺�
    std::vector<Bone*>  mBoneArray;
    // ���������� Ʈ�������� ����� �� ã������ �� �������̺�
    std::unordered_map<std::wstring, Bone*> mBoneTable;

    static std::shared_ptr<MeshResource> SkyCubeMesh;
    static std::shared_ptr<MeshResource> CubeMesh;
    static std::shared_ptr<MeshResource> PlainMesh;
public:
    static void InitSkyCubeMesh();
    static void InitCubeMesh();
    static void InitPlainMesh();
};