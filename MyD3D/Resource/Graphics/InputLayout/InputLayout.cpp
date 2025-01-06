#include "pch.h"
#include "InputLayout.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/Graphics/Shader/Shader.h"

InputLayout::InputLayout(std::wstring_view _name, std::vector<InputLayoutDesc>& _pILDesc, VertexShader* _pVertexShader)
    : Resource(_name)
{
    if (_pILDesc.empty())
    {
        throw std::runtime_error("Invailed Args to VertexShader::VertexShader->_pILDesc...");
    }
    if (!_pVertexShader)
    {
        throw std::runtime_error("Null Pointer Exception to VertexShader::VertexShader->_pVertexShader...");
    }
    IGraphicsDevice* pDevice = GraphicsManager::GetDevice();
    if (pDevice)
    {
        if (FAILED(pDevice->CreateInputLayout(_name.data(), &_pILDesc.front(), _pILDesc.size(), _pVertexShader->GetShader(), &mInputLayout)))
        {
            throw std::runtime_error("Hresult Failed to InputLayout::InputLayout->CreateInputLayout()...");
        }
    }
}

InputLayout::~InputLayout()
{
    SAFE_RELEASE(mInputLayout);
}
