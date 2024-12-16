#pragma once

namespace Graphics
{
	class GraphicsDevice;
	struct BufferDesc;
	struct SubBufferDesc;

	class Buffer : public IGraphicsResource
	{
	public:
		Buffer(std::wstring_view _name
			, GraphicsDevice* _pDevice
			, const D3D11_BUFFER_DESC* _pBufferDesc
			, const D3D11_SUBRESOURCE_DATA* _pBufferSubDesc = nullptr);
		~Buffer();
	private:
		ID3D11Buffer* mBuffer;
	public:
		operator ID3D11Buffer* () { return mBuffer; }
	};

	struct BufferDesc
	{
		UINT ByteWidth;
		UINT Usage;
		UINT BindFlags;
		UINT CPUAccessFlags;
		UINT MiscFlags;
		UINT StructureByteStride;
	};
	struct SubBufferDesc
	{
		const void* pSysMem;
		UINT SysMemPitch;
		UINT SysMemSlicePitch;
	};
}


