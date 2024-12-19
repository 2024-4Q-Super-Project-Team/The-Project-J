#pragma once

namespace Graphics
{
	class GraphicsDevice;
	class Renderer;
	struct BufferDesc;
	struct SubBufferDesc;

	class ConstantBuffer;
	class VertexBuffer;
	class IndexBuffer;

	class Buffer : public IGraphicsResource
	{
	public:
		explicit Buffer(std::wstring_view _name
			, GraphicsDevice* _pDevice
			, const D3D11_BUFFER_DESC* _pBufferDesc
			, const D3D11_SUBRESOURCE_DATA* _pBufferSubDesc = nullptr);
		virtual ~Buffer();
	private:
		ID3D11Buffer* mBuffer;
	public:
		operator ID3D11Buffer* () { return mBuffer; }
	};

	class ConstantBuffer : public Buffer
	{
	public:
		using Buffer::Buffer;
		virtual ~ConstantBuffer() = default;
	};

	class VertexBuffer : public Buffer 
	{
	public:
		using Buffer::Buffer;
		virtual ~VertexBuffer() = default;
	public:
		void SetStride(UINT _stride) { mStride = _stride; }
		UINT GetStride() { return mStride; }
		void SetOffset(UINT _offset) { mOffset = _offset; }
		UINT GetOffset() { return mOffset; }
	private:
		// 버텍스버퍼 크기 및 옵셋
		UINT mStride = 0;
		UINT mOffset = 0;
	};

	class IndexBuffer : public Buffer 
	{
	public:
		using Buffer::Buffer;
		virtual ~IndexBuffer() = default;
	public:
		void SetFormat(UINT _format) { mFormat = _format; }
		UINT GetFormat() { return mFormat; }
		void SetOffset(UINT _offset) { mOffset = _offset; }
		UINT GetOffset() { return mOffset; }
	public:
		UINT mFormat;
		UINT mOffset;
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


