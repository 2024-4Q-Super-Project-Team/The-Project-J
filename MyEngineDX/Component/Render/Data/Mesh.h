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
	// 렌더러에서 사용하는 클래스.
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
		// 각 원소가 어느 본팔레트의 인덱스를 참조할지
		INT		BoneIndices[4]	 = { -1,-1,-1,-1 };
		// 본 인덱스를 통해 매칭된 본에서 몇의 가중치만큼 영향을 받을건지
		float	BoneWeights[4]	 =  { 0.0f,0.0f,0.0f,0.0f };
	};

}

