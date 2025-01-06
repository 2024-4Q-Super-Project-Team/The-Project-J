#pragma once

class IGraphicsDevice;
class IGraphicsRenderer;
class IGraphicsRenderTarget;
class IGraphicsResource;

class IShader;
class IVertexShader;
class IPixelShader;

class IBuffer;
class IVertexBuffer;
class IIndexBuffer;
class IConstantBuffer;
class IInputLayout;

class ITexture;
class ITexture2D;
class ITexture3D;

class ISamplerState;

struct BufferDesc;
struct SubResourceData;
struct ViewportDesc;
struct InputLayoutDesc;
struct SamplerDesc;

SET_UTID(_UTID_GRAPHICS_UNKNOWN, eUTID::Unknown);

class IGraphicsUnknown
{
public:
    virtual UTID    GetTypeID() = 0;
    virtual HRESULT QueryInterface(const UTID _UTID, void** _ppObject) = 0;
    virtual void    Release() = 0;  
};

class IGraphics : public IGraphicsUnknown
{
};

class IBindHandler
{
public:
    virtual HRESULT Bind(IGraphicsRenderer* _renderer) = 0;
    virtual HRESULT Reset(IGraphicsRenderer* _renderer) = 0;
};

class IGraphicsDevice : public IGraphics
{
public:
	virtual HRESULT CreateRenderTarget(HWND _hwnd, IGraphicsRenderTarget** _ppRenderTarget) = 0;
    virtual HRESULT CreateShader(eShaderStage _type, const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel, IShader** _ppShader) = 0;
    virtual HRESULT CreateTexture2D(const WCHAR* _path, ITexture2D** _ppTex2D) = 0;
    virtual HRESULT CreateBuffer(const WCHAR* _name, const BufferDesc* _pBufDesc, const SubResourceData* _pSubRscDesc, void** _ppBuffer) = 0;
    virtual HRESULT CreateInputLayout(const WCHAR* _name, const InputLayoutDesc* _pIADesc, UINT _arrSize, IVertexShader* _pVertexShader, IInputLayout** _ppInputLayout) = 0;
    virtual HRESULT CreateSamplerState(const WCHAR* _name, SamplerDesc* _pDesc, ISamplerState** _ppSamplerState) = 0;
};

class IGraphicsRenderer : public IGraphics
{
public:
	virtual BOOL Clear() = 0;
	virtual BOOL BeginRender() = 0;
	virtual BOOL EndRender() = 0;
	virtual BOOL DrawCall(UINT _numIndex, UINT _startIndex, INT _baseVertex) = 0;

	virtual BOOL SetRenderTarget(IGraphicsRenderTarget* _pRenderTarget) = 0;

    virtual HRESULT BindConstantBuffer(eShaderStage _stage, UINT _startSlot, UINT _numBuff, IConstantBuffer* _pCBuff) = 0;

    virtual HRESULT BindViewport(ViewportDesc* _pViewportDesc) = 0;
    virtual HRESULT BindBuffer(IBuffer* _pBuffer) = 0;
    virtual HRESULT BindTexture(ITexture* _pBuffer) = 0;
    virtual HRESULT UpdateSubResource(IGraphicsResource* _pSubResource, UINT _dstSubresource, const D3D11_BOX* _dstBox, const void* _srcData, UINT _srcRowPitch, UINT _srcDepthPitch) = 0;
};

class IGraphicsRenderTarget : public IGraphics
{
public:
    virtual void SetClearColor(FLOAT* _rgba) = 0;
};

class IGraphicsResource : public IGraphics
{
public:
	virtual const WCHAR* GetName() = 0;
};

// Texture
class ITexture : public IGraphicsResource, public IBindHandler
{
public:
    virtual ITexture& SetSlot(UINT _startSlot) = 0;
    virtual ITexture& SetShaderStage(eShaderStage _stage) = 0;
};

class ITexture2D : public ITexture
{
};

class ITexture3D : public ITexture
{
};

// Buffer
class IBuffer : public IGraphicsResource, public IBindHandler
{
};

class IVertexBuffer : public IBuffer
{
public:
   virtual IVertexBuffer& SetSlot(UINT _startSlot) = 0;
   virtual IVertexBuffer& SetStride(UINT _stride) = 0;
   virtual IVertexBuffer& SetOffset(UINT _offset) = 0;
   virtual UINT GetStride() = 0;
   virtual UINT GetOffset() = 0;
};

class IIndexBuffer : public IBuffer
{
public:
    virtual IIndexBuffer& SetFormat(UINT _format) = 0;
    virtual IIndexBuffer& SetOffset(UINT _offset) = 0;
    virtual UINT GetFormat() = 0;
    virtual UINT GetOffset() = 0;
};

class IConstantBuffer : public IBuffer
{
public:
    virtual IConstantBuffer& SetSlot(UINT _startSlot) = 0;
    virtual IConstantBuffer& SetShaderStage(eShaderStage _stage) = 0;
};

// InputLayout
class IInputLayout : public IGraphicsResource
{

};

// Shader
class IShader : public IGraphicsResource, public IBindHandler
{
public:
    virtual LPVOID __stdcall GetBufferPointer() = 0;
    virtual SIZE_T __stdcall GetBufferSize() = 0;
};

class IVertexShader : public IShader
{
public:
};

class IPixelShader : public IShader
{

};

// SamplerState
class ISamplerState : public IGraphicsResource, public IBindHandler
{
public:
    virtual ISamplerState& SetSlot(UINT _startSlot) = 0;
    virtual ISamplerState& SetShaderStage(eShaderStage _stage) = 0;
};

