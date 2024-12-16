#pragma once

namespace Graphics
{
	class GraphicsDevice;
	class Buffer;
	struct Vertex;

	class MeshInfo : public IGraphicsResource
	{
	public:
		explicit MeshInfo(GraphicsDevice* _pDevice, std::wstring_view _name, std::vector<Vertex>& _vertices, std::vector<UINT>& _indices);
		virtual ~MeshInfo();
	private:
		Buffer* mVertexBuffer;
		Buffer* mIndexBuffer;
		// ���ؽ����� ũ�� �� �ɼ�
		UINT mVertexBuffStride;
		UINT mVertexBuffOffset;
		std::vector<Vertex> mVertices;
		std::vector<UINT>	mIndices;
	};

	class MaterialState;
	// ���������� ����ϴ� Ŭ����.
	class MeshState
	{
	public:
		explicit MeshState(MeshInfo* _pMeshInfo, MaterialState* _pMaterial);
		virtual ~MeshState();
	public:
		inline auto GetName() { return mMeshInfo->GetName(); }
	public:
		MeshInfo*				 mMeshInfo;
		MaterialState*			 mMaterialState;
	};

	struct Vertex
	{
		Vector3 Position		 = { 0,0,0 };
		Vector4 Color			 = { 1,1,1,1 };
		Vector3 Normal			 = { 0,0,0 };
		Vector3 Tangent			 = { 0,0,0 };
		Vector2 TexCoord		 = { 0,0 };
		// �� ���Ұ� ��� ���ȷ�Ʈ�� ��������
		INT		BoneID[4]		 = { -1,-1,-1,-1 };
		// ID�� ��Ī�� �� �ȷ�Ʈ���� ���� ����ġ��ŭ ������ ��������
		float	BoneWeight[4]	=  { 0.0f,0.0f,0.0f,0.0f };
	};

}

