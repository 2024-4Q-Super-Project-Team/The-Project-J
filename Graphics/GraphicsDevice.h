#pragma once
#include "IGraphics.h"

SET_UTID(_UTID_GRAPHICS_DEVICE, eUTID::GraphicsDevice);

class GraphicsDevice
	: public IGraphicsDevice
{
public:
	explicit GraphicsDevice(ID3D11Device* _pDevice, IDXGIFactory* _pFactory, IDXGIAdapter3* _pAdapter);
	virtual ~GraphicsDevice();
	GraphicsDevice(GraphicsDevice&) = delete;
	GraphicsDevice& operator=(const GraphicsDevice&) = delete;
	GraphicsDevice(GraphicsDevice&&) noexcept = default;
	GraphicsDevice& operator=(GraphicsDevice&&) noexcept = default;
public:
    virtual UTID    GetTypeID() override;
    virtual HRESULT QueryInterface(const UTID _UTID, void** _ppObject) override;
	virtual void    Release() override;
public:
	virtual HRESULT CreateRenderTarget(HWND _hwnd, IGraphicsRenderTarget** _ppRenderTarget) override;
    //virtual HRESULT CreateInputLayout()
    virtual HRESULT CreateShader(eShaderStage _type, const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel, IShader** _ppShader) override;
    virtual HRESULT CreateTexture2D(const WCHAR* _path, ITexture2D** _ppTex2D) override;
    virtual HRESULT CreateBuffer(const WCHAR* _name, const BufferDesc* _pBufDesc, const SubResourceData* _pSubRscDesc, void** _ppBuffer) override;
    virtual HRESULT CreateInputLayout(const WCHAR* _name, const InputLayoutDesc* _pIADesc, UINT _arrSize, IVertexShader* _pVertexShader, IInputLayout** _ppInputLayout) override;
    virtual HRESULT CreateSamplerState(const WCHAR* _name, SamplerDesc* _pDesc, ISamplerState** _ppSamplerState) override;
public:
	ID3D11Device*	mDevice;
	IDXGIFactory*	mFactory;
	IDXGIAdapter3*	mAdapter;
};

