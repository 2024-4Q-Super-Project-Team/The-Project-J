#pragma once
#include "Resource/Resource.h"
#include "Vertex.h"

class D3DGraphicsVertexBuffer;
class D3DGraphicsIndexBuffer;
class MaterialResource;
class Material;
class Bone;

class MeshResource
    : public Resource
{
public:
    RESOURCE_TYPE(Mesh);
    explicit MeshResource(std::wstring_view _name, std::vector<Vertex>& _vertices, std::vector<UINT>& _indices);
    virtual ~MeshResource();
public:
    HRESULT Create();
    void    Bind();
public:
    D3DGraphicsVertexBuffer* mVertexBuffer;
    D3DGraphicsIndexBuffer*  mIndexBuffer;
    std::vector<Vertex>      mVertices;
    std::vector<UINT>	     mIndices;

    // 각 정점(Vertex)에게 인덱싱을 통해 본 정보를 참조시키기 위한 테이블
    std::vector<Bone*>  mBoneArray;
    // 렌더러에서 트랜스폼을 등록할 때 찾기위한 본 매핑테이블
    std::unordered_map<std::wstring, Bone*> mBoneTable;

    static std::shared_ptr<MeshResource> SkyCubeMesh;
    static std::shared_ptr<MeshResource> CubeMesh;
    static std::shared_ptr<MeshResource> PlainMesh;
public:
    static void InitSkyCubeMesh();
    static void InitCubeMesh();
    static void InitPlainMesh();
public: 
	virtual void EditorRendering(EditorViewerType _viewerType) override;
};