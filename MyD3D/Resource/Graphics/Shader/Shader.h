#pragma once
#include "Resource/Resource.h"

class InputLayout;
class IVertexShader;
class IPixelShader;

class VertexShader
    : public Resource
{
public:
    RESOURCE_TYPE(eResourceType::VertexShader);
    explicit VertexShader(std::wstring_view _path);
    explicit VertexShader(std::wstring_view _path, LPCSTR _entryPoint, LPCSTR _shaderModel);
    virtual ~VertexShader();
public:
    inline IVertexShader* GetShader() { return mVertexShader; }
    void Bind();
private:
    IGraphicsRenderer*  mRenderer;
    IVertexShader*      mVertexShader;
};

class PixelShader
    : public Resource
{
public:
    RESOURCE_TYPE(eResourceType::PixelShader);
    explicit PixelShader(std::wstring_view _path);
    explicit PixelShader(std::wstring_view _path, LPCSTR _entryPoint, LPCSTR _shaderModel);
    virtual ~PixelShader();
public:
    inline IPixelShader* GetShader() { return mPixelShader; }
    void Bind();
private:
    IGraphicsRenderer* mRenderer;
    IPixelShader* mPixelShader;
};
