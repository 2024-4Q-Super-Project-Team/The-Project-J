#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Interface/IGraphics.h"

class D3DGraphicShader 
    : public I3DGraphicsGPUResource
    , public GraphicsResourceKeyHandler
{
public:
    explicit D3DGraphicShader(std::wstring_view _path);
    virtual ~D3DGraphicShader() = default;
public:
    virtual void Release() override;
protected:
    ID3DBlob* mBlob;
};

class D3DGraphicsVertexShader : public D3DGraphicShader
{
public:
    explicit D3DGraphicsVertexShader(std::wstring_view _path);
    virtual ~D3DGraphicsVertexShader() = default;
public:
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
private:
    ID3D11VertexShader* mVertexShader;
    ID3D11InputLayout*  mInputLayout;
};

class D3DGraphicsPixelShader : public D3DGraphicShader
{
public:
    explicit D3DGraphicsPixelShader(std::wstring_view _path);
    virtual ~D3DGraphicsPixelShader() = default;
public:
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
private:
    ID3D11PixelShader* mPixelShader;
};
