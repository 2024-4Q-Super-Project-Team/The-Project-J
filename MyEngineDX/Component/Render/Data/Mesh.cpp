#include "pch.h"
#include "Mesh.h"

namespace Graphics
{
	MeshInfo::MeshInfo(GraphicsDevice* _pDevice, std::wstring_view _name,
		std::vector<Vertex>& _vertices, std::vector<UINT>& _indices)
		: IGraphicsResource(_name)
	{
		// TODO : �޽� ó�� �ʿ�
		mVertices = std::move(_vertices);
		mIndices = std::move(_indices);

		mVertexBuffStride = sizeof(Vertex);
		mVertexBuffOffset = 0;

		D3D11_BUFFER_DESC bufDesc = {};
		{
			// ===========���ؽ� ����===========
			bufDesc.ByteWidth = sizeof(Vertex) * mVertices.size();
			bufDesc.Usage = D3D11_USAGE_DEFAULT;
			bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufDesc.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA subDesc = {};
			subDesc.pSysMem = mVertices.data();
			mVertexBuffer = new Graphics::Buffer(mName + L"_VertexBuffer", _pDevice, &bufDesc, &subDesc);
		}
		{
			// ===========�ε��� ����===========
			bufDesc.ByteWidth = sizeof(UINT) * mIndices.size();
			bufDesc.Usage = D3D11_USAGE_DEFAULT;
			bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufDesc.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA subDesc = {};
			subDesc.pSysMem = mIndices.data();
			mIndexBuffer = new Graphics::Buffer(mName + L"_IndexBuffer", _pDevice, &bufDesc, &subDesc);
		}
	}

	MeshInfo::~MeshInfo()
	{
		SAFE_DELETE(mVertexBuffer)
		SAFE_DELETE(mIndexBuffer)
	}

	MeshState::MeshState(MeshInfo* _pMeshInfo, MaterialState* _pMaterial)
		: mMeshInfo(_pMeshInfo), mMaterialState(_pMaterial)
	{
	}
	MeshState::~MeshState()
	{
	}
}