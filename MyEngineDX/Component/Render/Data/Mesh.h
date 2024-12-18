#pragma once

namespace Graphics
{
	class GraphicsDevice;
	class Buffer;
	struct Vertex;

	class MeshResource : public IGraphicsResource
	{
	public:
		explicit MeshResource(GraphicsDevice* _pDevice, std::wstring_view _name, std::vector<Vertex>& _vertices, std::vector<UINT>& _indices);
		virtual ~MeshResource();
	private:
		VertexBuffer* mVertexBuffer;
		IndexBuffer* mIndexBuffer;
		std::vector<Vertex> mVertices;
		std::vector<UINT>	mIndices;
	};

	class MaterialState;
	// ���������� ����ϴ� Ŭ����.
	class MeshState
	{
	public:
		explicit MeshState(MeshResource* _pMeshResource, MaterialState* _pMaterial);
		virtual ~MeshState();
	public:
		BOOL Bind(GraphicsManager* _graphicsManager);
	public:
		inline auto GetName() { return mMeshResource->GetName(); }
	public:
		MeshResource*				 mMeshResource;
		MaterialState*			 mMaterialState;
	};

	struct Vertex
	{
		Vector3 Position		 = { 0,0,0 };
		Vector4 Color			 = { 1,1,1,1 };
		Vector3 Normal			 = { 0,0,0 };
		Vector3 Tangent			 = { 0,0,0 };
		Vector2 TexCoord		 = { 0,0 };
		// �� ���Ұ� ��� ���ȷ�Ʈ�� �ε����� ��������
		INT		BoneIndices[4]	 = { -1,-1,-1,-1 };
		// �� �ε����� ���� ��Ī�� ������ ���� ����ġ��ŭ ������ ��������
		float	BoneWeights[4]	 =  { 0.0f,0.0f,0.0f,0.0f };
	};

}

