#include "pch.h"
#include "Shader.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/Graphics/InputLayout/InputLayout.h"

VertexShader::VertexShader(std::wstring_view _path)
    : Resource(_path)
    , mVertexShader(nullptr)
{
    IGraphicsDevice* pDevice = GraphicsManager::GetDevice();
    if (pDevice)
    {
        if (FAILED(pDevice->CreateShader(eShaderStage::VS, _path.data(), "", "", (IShader**)&mVertexShader)))
        {
            throw std::runtime_error("Hresult Failed to VertexShader::VertexShader->CreateShader()...");
        }
    }
}

VertexShader::VertexShader(std::wstring_view _path, LPCSTR _entryPoint, LPCSTR _shaderModel)
    : Resource(_path)
{
    IGraphicsDevice* pDevice = GraphicsManager::GetDevice();
    if (pDevice)
    {
        if (FAILED(pDevice->CreateShader(eShaderStage::VS, _path.data(), _entryPoint, _shaderModel, (IShader**)&mVertexShader)))
        {
            throw std::runtime_error("Hresult Failed to VertexShader::VertexShader->CreateShader()...");
        }
    }
}

VertexShader::~VertexShader()
{
    SAFE_RELEASE(mVertexShader);
}

void VertexShader::Bind()
{
    if (mRenderer)
    {
        mVertexShader->Bind(GraphicsManager::GetRenderer());
    }
}

PixelShader::PixelShader(std::wstring_view _path)
    : Resource(_path)
    , mPixelShader(nullptr)
{
    IGraphicsDevice* pDevice = GraphicsManager::GetDevice();
    if (pDevice)
    {
        if (FAILED(pDevice->CreateShader(eShaderStage::PS, _path.data(), "", "", (IShader**)&mPixelShader)))
        {
            throw std::runtime_error("Hresult Failed to PixelShader::PixelShader->CreateShader()...");
        }
    }
}

PixelShader::PixelShader(std::wstring_view _path, LPCSTR _entryPoint, LPCSTR _shaderModel)
    : Resource(_path)
{
    IGraphicsDevice* pDevice = GraphicsManager::GetDevice();
    if (pDevice)
    {
        if (FAILED(pDevice->CreateShader(eShaderStage::PS, _path.data(), _entryPoint, _shaderModel, (IShader**)&mPixelShader)))
        {
            throw std::runtime_error("Hresult Failed to PixelShader::PixelShader->CreateShader()...");
        }
    }
}

PixelShader::~PixelShader()
{
    SAFE_RELEASE(mPixelShader);
}

void PixelShader::Bind()
{
    if (mRenderer)
    {
        mPixelShader->Bind(GraphicsManager::GetRenderer());
    }
}
