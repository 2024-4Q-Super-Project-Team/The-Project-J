#pragma once

namespace Graphics
{
	class GraphicsDevice;
	struct BufferDesc;
	struct SubBufferDesc;

	class Buffer
	{
	public:
		Buffer(GraphicsDevice* _pDevice, BufferDesc* _pBufferDesc, SubBufferDesc* _pBufferSubDesc = nullptr);
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


