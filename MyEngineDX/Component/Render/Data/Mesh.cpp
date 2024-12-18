#include "pch.h"
#include "Mesh.h"
#include "Graphics/GraphicsManager.h"

namespace Graphics
{
	MeshResource::MeshResource(GraphicsDevice* _pDevice, std::wstring_view _name,
		std::vector<Vertex>& _vertices, std::vector<UINT>& _indices)
		: IGraphicsResource(_name)
	{
		// TODO : 메쉬 처리 필요
		mVertices = std::move(_vertices);
		mIndices = std::move(_indices);

		D3D11_BUFFER_DESC bufDesc = {};
		{
			// ===========버텍스 버퍼===========
			bufDesc.ByteWidth = sizeof(Vertex) * mVertices.size();
			bufDesc.Usage = D3D11_USAGE_DEFAULT;
			bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufDesc.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA subDesc = {};
			subDesc.pSysMem = mVertices.data();
			mVertexBuffer = new Graphics::VertexBuffer(mName + L"_VertexBuffer", _pDevice, &bufDesc, &subDesc);
			mVertexBuffer->SetStride(sizeof(Vertex));
		}
		{
			// ===========인덱스 버퍼===========
			bufDesc.ByteWidth = sizeof(UINT) * mIndices.size();
			bufDesc.Usage = D3D11_USAGE_DEFAULT;
			bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufDesc.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA subDesc = {};
			subDesc.pSysMem = mIndices.data();
			mIndexBuffer = new Graphics::IndexBuffer(mName + L"_IndexBuffer", _pDevice, &bufDesc, &subDesc);
			mIndexBuffer->SetFormat(DXGI_FORMAT_R32_UINT);
		}
	}

	MeshResource::~MeshResource()
	{
		SAFE_DELETE(mVertexBuffer)
		SAFE_DELETE(mIndexBuffer)
	}

	MeshState::MeshState(MeshResource* _pMeshResource, MaterialState* _pMaterial)
		: mMeshResource(_pMeshResource), mMaterialState(_pMaterial)
	{
	}
	MeshState::~MeshState()
	{
	}
	BOOL MeshState::Bind(GraphicsManager* _graphicsManager)
	{
		Renderer* pRenderer = _graphicsManager->GetRenderer();
		/*if (pRenderer)
		{
			pRenderer->BindVertexBuffer(0, 1, mMeshResource)
		}*/
		return FALSE;
	}
}