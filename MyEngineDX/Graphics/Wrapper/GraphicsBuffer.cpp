#include "pch.h"
#include "GraphicsBuffer.h"
#include "../GraphicsDevice.h"

namespace Graphics
{
	Buffer::Buffer(std::wstring_view _name
		, GraphicsDevice* _pDevice
		, const D3D11_BUFFER_DESC* _pBufferDesc
		, const D3D11_SUBRESOURCE_DATA* _pBufferSubDesc)
		: IGraphicsResource(_name)
	{
		if (_pBufferDesc)
		{
			if (_pBufferSubDesc)
			{
				_pDevice->GetDevice()->CreateBuffer(_pBufferDesc, _pBufferSubDesc, &mBuffer);
			}
			else
			{
				_pDevice->GetDevice()->CreateBuffer(_pBufferDesc, nullptr, &mBuffer);
			}
		}
	}
	Buffer::~Buffer()
	{
		SAFE_RELEASE(mBuffer)
	}
}

