#include "pch.h"
#include "GraphicsInputLayout.h"
#include "GraphicsDevice.h"

GraphicsInputLayout::GraphicsInputLayout(const WCHAR* _name
    , const InputLayoutDesc* _pIADesc, UINT _arrSize
    , IVertexShader* _pVertexShader, GraphicsDevice* _pDevice)
    : mName(_name), mInputLayout(nullptr)
{
    std::vector<D3D11_INPUT_ELEMENT_DESC> layouts;
    for (UINT i = 0; i < _arrSize; ++i)
    {
        for (UINT j = 0; j < _pIADesc[i].NumSemantics; ++j)
        {
            D3D11_INPUT_ELEMENT_DESC desc = {};
            desc.SemanticName           = _pIADesc[i].SemanticName;
            desc.SemanticIndex          = j;
            desc.Format                 = _pIADesc[i].Format;
            desc.InputSlot              = _pIADesc[i].InputSlot;
            desc.AlignedByteOffset      = _pIADesc[i].AlignedByteOffset;
            desc.InputSlotClass         = _pIADesc[i].InputSlotClass;
            desc.InstanceDataStepRate   = _pIADesc[i].InstanceDataStepRate;
            layouts.push_back(std::move(desc));
        }
    }
    ID3D11Device* pDevice = _pDevice->mDevice;
    if (pDevice)
    {
        HRESULT hr = pDevice->CreateInputLayout(
            layouts.data(),
            static_cast<UINT>(layouts.size()),
            _pVertexShader->GetBufferPointer(),
            _pVertexShader->GetBufferSize(),
            &mInputLayout);
        if (FAILED(hr))
        {
            if(mInputLayout)
                mInputLayout->Release();
            mInputLayout = nullptr;
            throw std::runtime_error("Hresult Failed to Create GraphicsInputLayout::CreateInputLayout.");
        }
    }
}

GraphicsInputLayout::~GraphicsInputLayout()
{
}

UTID GraphicsInputLayout::GetTypeID()
{
    return _UTID_GRAPHICS_INPUTLAYOUT;
}

HRESULT GraphicsInputLayout::QueryInterface(const UTID _UTID, void** _ppObject)
{
    if (_ppObject == nullptr)
    {
        return E_INVALIDARG;
    }
    *_ppObject = nullptr;
    switch (_UTID)
    {
    case _UTID_GRAPHICS_INPUTLAYOUT:
        *_ppObject = static_cast<GraphicsInputLayout*>(this);
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

void GraphicsInputLayout::Release()
{
    if (mInputLayout)
    {
        mInputLayout->Release();
    }
    mInputLayout = nullptr;
}

const WCHAR* GraphicsInputLayout::GetName()
{
    return mName;
}
