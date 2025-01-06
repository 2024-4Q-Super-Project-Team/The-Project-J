#pragma once
#include "IGraphics.h"

class GraphicsDevice;

SET_UTID(_UTID_GRAPHICS_BUFFER, eUTID::Buffer);
SET_UTID(_UTID_GRAPHICS_VERTEXBUFFER, eUTID::VertexBuffer);
SET_UTID(_UTID_GRAPHICS_INDEXBUFFER, eUTID::IndexBuffer);
SET_UTID(_UTID_GRAPHICS_CONSTANTBUFFER, eUTID::ConstantBuffer);

class GraphicsBuffer
{
public:
    explicit GraphicsBuffer(const WCHAR* _name
        , const BufferDesc* _pBufferDesc
        , const SubResourceData* _pBufferSubDesc
        , GraphicsDevice* _pDevice);
    virtual ~GraphicsBuffer();
    GraphicsBuffer(GraphicsBuffer&) = delete;
    GraphicsBuffer& operator=(const GraphicsBuffer&) = delete;
    GraphicsBuffer(GraphicsBuffer&&) noexcept = default;
    GraphicsBuffer& operator=(GraphicsBuffer&&) noexcept = default;
public:
    const WCHAR*    mName;
    ID3D11Buffer*   mBuffer;
};

class GraphicsVertexBuffer 
    : public IVertexBuffer
    , public GraphicsBuffer
{
public:
    explicit GraphicsVertexBuffer(const WCHAR* _name
        , const BufferDesc* _pBufferDesc
        , const SubResourceData* _pBufferSubDesc
        , GraphicsDevice* _pDevice);
    virtual ~GraphicsVertexBuffer();
public:
    virtual UTID    GetTypeID() override;
    virtual HRESULT QueryInterface(const UTID _UTID, void** _ppObject) override;
    virtual void    Release() override;
public:
    virtual const WCHAR* GetName() override;
public:
    virtual HRESULT Bind(IGraphicsRenderer* _renderer) override;
    virtual HRESULT Reset(IGraphicsRenderer* _renderer) override;
public:
    virtual IVertexBuffer& SetSlot(UINT _startSlot) override;
    virtual IVertexBuffer& SetStride(UINT _stride) override;
    virtual IVertexBuffer& SetOffset(UINT _offset) override;
    virtual UINT GetStride() override;
    virtual UINT GetOffset() override;
public:
    // 버텍스버퍼 크기 및 오프셋
    UINT mSlot;
    UINT mStride;
    UINT mOffset;
};

class GraphicsIndexBuffer
    : public IIndexBuffer
    , public GraphicsBuffer
{
public:
    explicit GraphicsIndexBuffer(const WCHAR* _name
        , const BufferDesc* _pBufferDesc
        , const SubResourceData* _pBufferSubDesc
        , GraphicsDevice* _pDevice);
    virtual ~GraphicsIndexBuffer();
public:
    virtual UTID    GetTypeID() override;
    virtual HRESULT QueryInterface(const UTID _UTID, void** _ppObject) override;
    virtual void    Release() override;
public:
    virtual const WCHAR* GetName() override;
public:
    virtual HRESULT Bind(IGraphicsRenderer* _renderer) override;
    virtual HRESULT Reset(IGraphicsRenderer* _renderer) override;
public:
    virtual IIndexBuffer& SetFormat(UINT _format) override;
    virtual IIndexBuffer& SetOffset(UINT _offset) override;
    virtual UINT GetFormat() override;
    virtual UINT GetOffset() override;
public:
    // 인덱스버퍼 포맷(자료형) 유형 및 오프셋
    UINT mFormat;
    UINT mOffset;
};

class GraphicsConstantBuffer
    : public IConstantBuffer
    , public GraphicsBuffer
{
public:
    explicit GraphicsConstantBuffer(const WCHAR* _name
        , const BufferDesc* _pBufferDesc
        , const SubResourceData* _pBufferSubDesc
        , GraphicsDevice* _pDevice);
    virtual ~GraphicsConstantBuffer();
public:
    virtual UTID    GetTypeID() override;
    virtual HRESULT QueryInterface(const UTID _UTID, void** _ppObject) override;
    virtual void    Release() override;
public:
    virtual const WCHAR* GetName() override;
public:
    virtual IConstantBuffer& SetSlot(UINT _startSlot) override;
    virtual IConstantBuffer& SetShaderStage(eShaderStage _stage) override;
public:
    virtual HRESULT Bind(IGraphicsRenderer* _renderer) override;
    virtual HRESULT Reset(IGraphicsRenderer* _renderer) override;
public:
    UINT            mSlot;
    eShaderStage    mShaderStage;
};