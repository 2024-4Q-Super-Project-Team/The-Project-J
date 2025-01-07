#include "pch.h"
#include "D3DGraphicsShader.h"
#include "Graphics/Core/D3DGraphicsDevice.h"
#include "Graphics/Core/D3DGraphicsRenderer.h"

D3DGraphicShader::D3DGraphicShader(std::wstring_view _path)
    : GraphicsResourceKeyHandler(_path)
{
    std::wstring path = _path.data();
    // .cso 파일을 읽어들임
    std::ifstream file(path.c_str(), std::ios::ate | std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open compiled shader file: " + std::string(path.begin(), path.end()));
    }
    std::size_t fileSize = static_cast<std::size_t>(file.tellg());
    file.seekg(0);
    // ID3DBlob 생성 및 데이터 복사
    HRESULT hr = D3DCreateBlob(fileSize, &mBlob);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create ID3DBlob for shader data.");
    }
    file.read(static_cast<char*>(mBlob->GetBufferPointer()), fileSize);
    file.close();
}

void D3DGraphicShader::Release()
{
    SAFE_RELEASE(mBlob);
    delete this;
}

D3DGraphicsVertexShader::D3DGraphicsVertexShader(std::wstring_view _path)
    : D3DGraphicShader(_path)
{
    Create();
}

HRESULT D3DGraphicsVertexShader::Create()
{
    auto pDevice = D3DGraphicsDevice::GetDevice();

    Helper::HRT(pDevice->CreateVertexShader(
        mBlob->GetBufferPointer(), mBlob->GetBufferSize(), nullptr, &mVertexShader)
        , "Hresult Failed to Create GraphicsVertexShader::CreateVertexShader->CreateVertexShader()...");
    
    ID3D11ShaderReflection* pReflector = NULL;
    D3D11_SHADER_DESC shaderDesc;
    Helper::HRT(D3DReflect(mBlob->GetBufferPointer(), mBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector));
    Helper::HRT(pReflector->GetDesc(&shaderDesc));

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
    for (UINT inputIndex = 0; inputIndex < shaderDesc.InputParameters; inputIndex++)
    {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
        pReflector->GetInputParameterDesc(inputIndex, &paramDesc);

        D3D11_INPUT_ELEMENT_DESC elemDesc = {};
        elemDesc.SemanticName = paramDesc.SemanticName;
        elemDesc.SemanticIndex = paramDesc.SemanticIndex;
        elemDesc.InputSlot = 0;
        elemDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        elemDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        elemDesc.InstanceDataStepRate = 0;

        if (paramDesc.Mask == 1)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elemDesc.Format = DXGI_FORMAT_R32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elemDesc.Format = DXGI_FORMAT_R32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elemDesc.Format = DXGI_FORMAT_R32_FLOAT;
        }
        else if (paramDesc.Mask <= 3)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elemDesc.Format = DXGI_FORMAT_R32G32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elemDesc.Format = DXGI_FORMAT_R32G32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elemDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
        }
        else if (paramDesc.Mask <= 7)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elemDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elemDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elemDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        }
        else if (paramDesc.Mask <= 15)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elemDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elemDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elemDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        }
        inputLayoutDesc.push_back(elemDesc);
    }
    Helper::HRT(pDevice->CreateInputLayout(
        inputLayoutDesc.data(),
        static_cast<UINT>(inputLayoutDesc.size()),
        mBlob->GetBufferPointer(),
        mBlob->GetBufferSize(),
        &mInputLayout),
        "Hresult Failed to Create GraphicsVertexShader::CreateVertexShader->CreateInputLayout()...");
    
    pReflector->Release();

    return S_OK;
}

HRESULT D3DGraphicsVertexShader::Bind()
{
    ID3D11DeviceContext* pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    if (pDeviceContext)
    {
        pDeviceContext->IASetInputLayout(mInputLayout);
        pDeviceContext->VSSetShader(mVertexShader, nullptr, 0);
        return S_OK;
    }
    return E_FAIL;
}

HRESULT D3DGraphicsVertexShader::Reset()
{
    ID3D11DeviceContext* pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    if (pDeviceContext)
    {
        ID3D11InputLayout* nullInputLayout = nullptr;
        ID3D11VertexShader* nullShadser = nullptr;
        pDeviceContext->IASetInputLayout(nullInputLayout);
        pDeviceContext->VSSetShader(nullShadser, nullptr, 0);
        return S_OK;
    }
    return E_FAIL;
}

D3DGraphicsPixelShader::D3DGraphicsPixelShader(std::wstring_view _path)
    : D3DGraphicShader(_path)
{
    Create();
}

HRESULT D3DGraphicsPixelShader::Create()
{
    auto pDevice = D3DGraphicsDevice::GetDevice();
    Helper::HRT(pDevice->CreatePixelShader(mBlob->GetBufferPointer(), 
        mBlob->GetBufferSize(),
        nullptr,
        &mPixelShader),
        "Hresult Failed to Create GraphicsPixelShader::CreatePixelShader->CreatePixelShader()...");
    return S_OK;
}

HRESULT D3DGraphicsPixelShader::Bind()
{
    ID3D11DeviceContext* pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    if (pDeviceContext)
    {
        pDeviceContext->PSSetShader(mPixelShader, nullptr, 0);
        return S_OK;
    }
    return E_FAIL;
}

HRESULT D3DGraphicsPixelShader::Reset()
{
    ID3D11DeviceContext* pDeviceContext = D3DGraphicsRenderer::mDeviceContext;
    if (pDeviceContext)
    {
        ID3D11PixelShader* nullShadser = nullptr;
        pDeviceContext->PSSetShader(nullShadser, nullptr, 0);
        return S_OK;
    }
    return E_FAIL;
}
