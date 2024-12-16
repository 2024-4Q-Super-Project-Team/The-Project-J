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
		// 버텍스버퍼 크기 및 옵셋
		UINT mVertexBuffStride;
		UINT mVertexBuffOffset;
		std::vector<Vertex> mVertices;
		std::vector<UINT>	mIndices;
	};

	class MaterialState;
	// 렌더러에서 사용하는 클래스.
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
		// 각 원소가 어느 본팔레트를 참조할지
		INT		BoneID[4]		 = { -1,-1,-1,-1 };
		// ID와 매칭된 본 팔레트에서 몇의 가중치만큼 영향을 받을건지
		float	BoneWeight[4]	=  { 0.0f,0.0f,0.0f,0.0f };
	};

}

