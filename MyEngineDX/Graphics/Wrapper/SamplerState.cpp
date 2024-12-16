#include "pch.h"
#include "SamplerState.h"
#include "Graphics/GraphicsDevice.h"

namespace Graphics
{
	SamplerState::SamplerState(std::wstring_view _name, GraphicsDevice* _pDevice, SamplerDesc* _pDesc)
		: IGraphicsResource(_name)
	{
		D3D11_SAMPLER_DESC sampDesc = {};
		if (_pDesc)
		{
			memcpy(&sampDesc, _pDesc, sizeof(D3D11_SAMPLER_DESC));
		}
		ID3D11Device* pDevice = _pDevice->GetDevice();
		if (pDevice)
		{
			pDevice->CreateSamplerState(&sampDesc, &mSaplerState);
		}
	}

	SamplerState::~SamplerState()
	{
		if (mSaplerState)
			mSaplerState->Release();
	}
}

