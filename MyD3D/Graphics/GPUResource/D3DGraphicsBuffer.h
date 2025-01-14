#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Interface/IGraphics.h"

class D3DGraphicsBuffer 
    : public I3DGraphicsGPUResource
{
public:
    explicit D3DGraphicsBuffer(const D3D11_BUFFER_DESC* _pBufferDesc, D3D11_SUBRESOURCE_DATA* _pBufferSubDesc);
    virtual ~D3DGraphicsBuffer();
public:
    virtual void Release() override;
public:
    ID3D11Buffer* mBuffer;
};

class D3DGraphicsVertexBuffer
    : public D3DGraphicsBuffer
    , public DataReadSTDBindHandler
{
public:
    explicit D3DGraphicsVertexBuffer(const D3D11_BUFFER_DESC* _pBufferDesc, D3D11_SUBRESOURCE_DATA* _pBufferSubDesc);
    virtual ~D3DGraphicsVertexBuffer() = default;
public:
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
};

class D3DGraphicsIndexBuffer
    : public D3DGraphicsBuffer
    , public DataReadSTDBindHandler
{
public:
    explicit D3DGraphicsIndexBuffer(const D3D11_BUFFER_DESC* _pBufferDesc, D3D11_SUBRESOURCE_DATA* _pBufferSubDesc);
    virtual ~D3DGraphicsIndexBuffer() = default;
public:
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
};

class D3DGraphicsConstantBuffer
    : public D3DGraphicsBuffer
    , public RenderStageBindHandler
    , public RegisterSlotBindHandler
    , public UpdateGPUResourceHandler
{
public:
    explicit D3DGraphicsConstantBuffer(const D3D11_BUFFER_DESC* _pBufferDesc, D3D11_SUBRESOURCE_DATA* _pBufferSubDesc);
    virtual ~D3DGraphicsConstantBuffer() = default;
public:
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
    virtual void UpdateGPUResoure(IN const void* _srcData) override;
};