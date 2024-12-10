#include "pch.h"
#include "GraphicsBuffer.h"

namespace Graphics
{
	Buffer::Buffer(GraphicsDevice* _pDevice, BufferDesc* _pBufferDesc, SubBufferDesc* _pBufferSubDesc)
	{
		if (_pBufferDesc)
		{
			D3D11_BUFFER_DESC bDesc = {};
			memcpy(&bDesc, _pBufferDesc, sizeof(D3D11_BUFFER_DESC));
			if (_pBufferSubDesc)
			{
				D3D11_SUBRESOURCE_DATA sDesc = {};
				memcpy(&sDesc, _pBufferSubDesc, sizeof(D3D11_SUBRESOURCE_DATA));
				_pDevice->GetDevice()->CreateBuffer(&bDesc, &sDesc, &mBuffer);
			}
			else
			{
				_pDevice->GetDevice()->CreateBuffer(&bDesc, nullptr, &mBuffer);
			}
		}
	}

	Buffer::~Buffer()
	{
	}
}

