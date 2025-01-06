#pragma once
#include "IGraphics.h"

class GraphicsDevice;
class GraphicsInputLayout;

SET_UTID(_UTID_GRAPHICS_SHADER, eUTID::Shader);
SET_UTID(_UTID_GRAPHICS_VERTEXSHADER, eUTID::VertexShader);
SET_UTID(_UTID_GRAPHICS_PIXELSHADER, eUTID::PixelShader);

class GraphicsShader
{
public:
    explicit GraphicsShader(const WCHAR* _path);
    explicit GraphicsShader(const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel, GraphicsDevice* _pDevice);
    virtual ~GraphicsShader();
protected:
    const WCHAR* mName;
    ID3DBlob*    mBlob;
};

class GraphicsVertexShader 
    : public GraphicsShader
    , public IVertexShader
{
public:
    explicit GraphicsVertexShader(const WCHAR* _path, GraphicsDevice* _pDevice);
    explicit GraphicsVertexShader(const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel, GraphicsDevice* _pDevice);
    virtual ~GraphicsVertexShader();
    GraphicsVertexShader(GraphicsVertexShader&) = delete;
    GraphicsVertexShader& operator=(const GraphicsVertexShader&) = delete;
    GraphicsVertexShader(GraphicsVertexShader&&) noexcept = default;
    GraphicsVertexShader& operator=(GraphicsVertexShader&&) noexcept = default;
public:
    virtual UTID                GetTypeID() override;
    virtual HRESULT             QueryInterface(const UTID _UTID, void** _ppObject) override;
    virtual void                Release() override;
public:
    virtual const WCHAR*        GetName() override;
public:
    virtual LPVOID __stdcall    GetBufferPointer() override;
    virtual SIZE_T __stdcall    GetBufferSize() override;
public:
    virtual HRESULT             Bind(IGraphicsRenderer* _renderer) override;
    virtual HRESULT             Reset(IGraphicsRenderer* _renderer) override;
public:
    ID3D11VertexShader*         mVertexShader;
    ID3D11InputLayout*          mInputLayout;
private:
    void Create(GraphicsDevice* _pDevice);
};

class GraphicsPixelShader
    : public GraphicsShader
    , public IPixelShader
{
public:
    explicit GraphicsPixelShader(const WCHAR* _path, GraphicsDevice* _pDevice);
    explicit GraphicsPixelShader(const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel, GraphicsDevice* _pDevice);
    virtual ~GraphicsPixelShader();
    GraphicsPixelShader(GraphicsPixelShader&) = delete;
    GraphicsPixelShader& operator=(const GraphicsPixelShader&) = delete;
    GraphicsPixelShader(GraphicsPixelShader&&) noexcept = default;
    GraphicsPixelShader& operator=(GraphicsPixelShader&&) noexcept = default;
public:
    virtual UTID                GetTypeID() override;
    virtual HRESULT             QueryInterface(const UTID _UTID, void** _ppObject) override;
    virtual void                Release() override;
public:
    virtual const WCHAR*        GetName() override;
public:
    virtual LPVOID __stdcall    GetBufferPointer() override;
    virtual SIZE_T __stdcall    GetBufferSize() override;
public:
    virtual HRESULT             Bind(IGraphicsRenderer* _renderer) override;
    virtual HRESULT             Reset(IGraphicsRenderer* _renderer) override;
public:
    ID3D11PixelShader*          mPixelShader;
private:
    void Create(GraphicsDevice* _pDevice);
};