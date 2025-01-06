#include "pch.h"
#include "GraphicsShader.h"
#include "GraphicsDevice.h"
#include "GraphicsRenderer.h"
#include "GraphicsInputLayout.h"
/////////////////////////////////////////////////////////////////////////////////////////
/// VertexShader ////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
GraphicsShader::GraphicsShader(const WCHAR* _path)
    : mName(_path)
    , mBlob(nullptr)
{
    std::wstring path = _path;

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

GraphicsShader::GraphicsShader(const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel, GraphicsDevice* _pDevice)
{
    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    dwShaderFlags |= D3DCOMPILE_DEBUG;
    dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
    ID3DBlob* pErrorBlob = nullptr;
    HRESULT hr = D3DCompileFromFile(_path, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _entryPoint, _shaderModel,
        dwShaderFlags, 0, &mBlob, &pErrorBlob);
    if (FAILED(hr))
    {
        if (pErrorBlob)
        {
            MessageBoxA(NULL, static_cast<char*>(pErrorBlob->GetBufferPointer()), "CompileShaderFromFile", MB_OK);
            std::wstring fileWName = _path;
            std::string fileName;
            fileName.assign(fileWName.begin(), fileWName.end());
            throw std::runtime_error("Hresult Failed to Create GraphicsShader::D3DCompileFromFile. (" + fileName + ")");
            pErrorBlob->Release();
        }
    }
}

GraphicsShader::~GraphicsShader()
{
    if(mBlob)
        mBlob->Release();
}

GraphicsVertexShader::GraphicsVertexShader(const WCHAR* _path, GraphicsDevice* _pDevice)
    : GraphicsShader(_path)
    , mVertexShader(nullptr)
    , mInputLayout(nullptr)
{
    Create(_pDevice);
}

GraphicsVertexShader::GraphicsVertexShader(const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel, GraphicsDevice* _pDevice)
    : GraphicsShader(_path, _entryPoint, _shaderModel, _pDevice)
    , mVertexShader(nullptr)
    , mInputLayout(nullptr)
{
    Create(_pDevice);
}

GraphicsVertexShader::~GraphicsVertexShader()
{
}

UTID GraphicsVertexShader::GetTypeID()
{
    return _UTID_GRAPHICS_VERTEXSHADER;
}

HRESULT GraphicsVertexShader::QueryInterface(const UTID _UTID, void** _ppObject)
{
    if (_ppObject == nullptr)
    {
        return E_INVALIDARG;
    }
    *_ppObject = nullptr;
    switch (_UTID)
    {
    case _UTID_GRAPHICS_VERTEXSHADER:
        *_ppObject = static_cast<GraphicsVertexShader*>(this);
        break;
    case _UTID_GRAPHICS_UNKNOWN:
        *_ppObject = static_cast<IGraphicsUnknown*>(this);
        break;
    default:
        *_ppObject = nullptr;
        return E_NOINTERFACE;
        break;
    }
    return (*_ppObject) ? S_OK : E_NOINTERFACE;
}

void GraphicsVertexShader::Release()
{

}

const WCHAR* GraphicsVertexShader::GetName()
{
    return mName;
}

LPVOID __stdcall GraphicsVertexShader::GetBufferPointer()
{
    return mBlob->GetBufferPointer();
}

SIZE_T __stdcall GraphicsVertexShader::GetBufferSize()
{
    return mBlob->GetBufferSize();
}

HRESULT GraphicsVertexShader::Bind(IGraphicsRenderer* _renderer)
{
    if (_renderer == nullptr)
    {
        return E_INVALIDARG;
    }
    GraphicsRenderer* pRenderer;
    if (S_OK == _renderer->QueryInterface(_UTID_GRAPHICS_RENDERER, (void**)&pRenderer))
    {
        ID3D11DeviceContext* pDeviceContext = pRenderer->mDeviceContext;
        if (pDeviceContext == nullptr) return E_FAIL;
        pDeviceContext->IASetInputLayout(mInputLayout);
        pDeviceContext->VSSetShader(mVertexShader, nullptr, 0);
        return S_OK;
    }
    else
    {
        return E_NOINTERFACE;
    }
}

HRESULT GraphicsVertexShader::Reset(IGraphicsRenderer* _renderer)
{
    if (_renderer == nullptr)
    {
        return E_INVALIDARG;
    }
    GraphicsRenderer* pRenderer;
    if (S_OK == _renderer->QueryInterface(_UTID_GRAPHICS_RENDERER, (void**)&pRenderer))
    {
        ID3D11DeviceContext* pDeviceContext = pRenderer->mDeviceContext;
        if (pDeviceContext == nullptr) return E_FAIL;
        if (mInputLayout)
        {
            ID3D11InputLayout* nullInputLayout = nullptr;
            pDeviceContext->IASetInputLayout(nullInputLayout);
        }
        ID3D11VertexShader* nullShadser = nullptr;
        pDeviceContext->VSSetShader(nullShadser, nullptr, 0);
        return S_OK;
    }
    else
    {
        return E_NOINTERFACE;
    }
}
void GraphicsVertexShader::Create(GraphicsDevice* _pDevice)
{
    HRESULT hr = _pDevice->mDevice->CreateVertexShader(
        mBlob->GetBufferPointer(), mBlob->GetBufferSize(), nullptr, &mVertexShader);
    if (FAILED(hr))
    {
        throw std::runtime_error("Hresult Failed to Create GraphicsVertexShader::CreateVertexShader->CreateVertexShader()...");
    }
    ID3D11ShaderReflection* pReflector = NULL;
    D3DReflect(mBlob->GetBufferPointer(), mBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
    D3D11_SHADER_DESC shaderDesc;
    pReflector->GetDesc(&shaderDesc);

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
    hr = _pDevice->mDevice->CreateInputLayout(
        inputLayoutDesc.data(),
        static_cast<UINT>(inputLayoutDesc.size()),
        mBlob->GetBufferPointer(),
        mBlob->GetBufferSize(),
        &mInputLayout);
    if (FAILED(hr))
    {
        throw std::runtime_error("Hresult Failed to Create GraphicsVertexShader::CreateVertexShader->CreateInputLayout()...");
    }
    pReflector->Release();
}
/////////////////////////////////////////////////////////////////////////////////////////
/// PixelShader /////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

GraphicsPixelShader::GraphicsPixelShader(const WCHAR* _path, GraphicsDevice* _pDevice)
    : GraphicsShader(_path)
    , mPixelShader(nullptr)
{
    Create(_pDevice);
}

GraphicsPixelShader::GraphicsPixelShader(const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel, GraphicsDevice* _pDevice)
    : GraphicsShader(_path, _entryPoint, _shaderModel, _pDevice)
    , mPixelShader(nullptr)
{
    Create(_pDevice);
}

GraphicsPixelShader::~GraphicsPixelShader()
{
}

UTID GraphicsPixelShader::GetTypeID()
{
    return _UTID_GRAPHICS_PIXELSHADER;
}

HRESULT GraphicsPixelShader::QueryInterface(const UTID _UTID, void** _ppObject)
{
    if (_ppObject == nullptr)
    {
        return E_INVALIDARG;
    }
    *_ppObject = nullptr;
    switch (_UTID)
    {
    case _UTID_GRAPHICS_PIXELSHADER:
        *_ppObject = static_cast<GraphicsPixelShader*>(this);
        break;
    case _UTID_GRAPHICS_UNKNOWN:
        *_ppObject = static_cast<IGraphicsUnknown*>(this);
        break;
    default:
        *_ppObject = nullptr;
        return E_NOINTERFACE;
        break;
    }
    return (*_ppObject) ? S_OK : E_NOINTERFACE;
}

void GraphicsPixelShader::Release()
{
    if (mPixelShader)
    {
        mPixelShader->Release();
    }
    mPixelShader = nullptr;
}

const WCHAR* GraphicsPixelShader::GetName()
{
    return mName;
}

LPVOID __stdcall GraphicsPixelShader::GetBufferPointer()
{
    return mBlob->GetBufferPointer();
}

SIZE_T __stdcall GraphicsPixelShader::GetBufferSize()
{
    return mBlob->GetBufferSize();
}

HRESULT GraphicsPixelShader::Bind(IGraphicsRenderer* _renderer)
{
    if (_renderer == nullptr)
    {
        return E_INVALIDARG;
    }
    GraphicsRenderer* pRenderer;
    if (S_OK == _renderer->QueryInterface(_UTID_GRAPHICS_RENDERER, (void**)&pRenderer))
    {
        ID3D11DeviceContext* pDeviceContext = pRenderer->mDeviceContext;
        if (pDeviceContext == nullptr) return E_FAIL;
        pDeviceContext->PSSetShader(mPixelShader, nullptr, 0);
        return S_OK;
    }
    else
    {
        return E_NOINTERFACE;
    }
}

HRESULT GraphicsPixelShader::Reset(IGraphicsRenderer* _renderer)
{
    if (_renderer == nullptr)
    {
        return E_INVALIDARG;
    }
    GraphicsRenderer* pRenderer;
    if (S_OK == _renderer->QueryInterface(_UTID_GRAPHICS_RENDERER, (void**)&pRenderer))
    {
        ID3D11DeviceContext* pDeviceContext = pRenderer->mDeviceContext;
        if (pDeviceContext == nullptr) return E_FAIL;
        ID3D11PixelShader* nullShader = nullptr;
        pDeviceContext->PSSetShader(nullShader, nullptr, 0);
        return S_OK;
    }
    else
    {
        return E_NOINTERFACE;
    }
}

void GraphicsPixelShader::Create(GraphicsDevice* _pDevice)
{
    HRESULT hr = _pDevice->mDevice->CreatePixelShader(
        mBlob->GetBufferPointer(), mBlob->GetBufferSize(), nullptr, &mPixelShader);
    if (FAILED(hr))
    {
        throw std::runtime_error("Hresult Failed to Create GraphicsPixelShader::CreatePixelShader->CreatePixelShader()...");
    }
}
