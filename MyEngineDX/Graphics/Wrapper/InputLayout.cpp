#include "pch.h"
#include "InputLayout.h"
#include "../GraphicsDevice.h"
#include "Shader.h"

namespace Graphics
{
	InputLayout::InputLayout(GraphicsDevice* _pDevice, const InputLayoutDesc* _pIADesc, UINT _arrSize, VertexShader* _pVertexShader, HRESULT** _ppHr)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> layouts;
		for (UINT i = 0; i < _arrSize; ++i)
		{
			for (UINT j = 0; j < _pIADesc[i].NumSemantics; ++j)
			{
				D3D11_INPUT_ELEMENT_DESC desc = {};
				memcpy(&desc, &_pIADesc[i], sizeof(D3D11_INPUT_ELEMENT_DESC));
				desc.SemanticIndex = j;
				layouts.emplace_back(desc);
			}
		}
		ID3D11Device* pDevice = _pDevice->GetDevice();
		if (pDevice)
		{
			HRESULT hr = pDevice->CreateInputLayout(
				layouts.data(),
				static_cast<UINT>(layouts.size()),
				_pVertexShader->GetBufferPointer(),
				_pVertexShader->GetBufferSize(),
				&mInputLayout);
			if ((*_ppHr))
			{
				*(*_ppHr) = hr;
			}
		}
		
	}
	InputLayout::~InputLayout()
	{
		if(mInputLayout)
		mInputLayout->Release();
	}
}