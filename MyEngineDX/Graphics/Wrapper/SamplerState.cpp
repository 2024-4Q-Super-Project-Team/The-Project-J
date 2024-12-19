#include "pch.h"
#include "SamplerState.h"
#include "../GraphicsDevice.h"

namespace Graphics
{
	SamplerState::SamplerState(std::wstring_view _name, GraphicsDevice* _pDevice, D3D11_SAMPLER_DESC* _pDesc)
		: IGraphicsResource(_name)
	{
		ID3D11Device* pDevice = _pDevice->GetDevice();
		if (pDevice)
		{
			pDevice->CreateSamplerState(_pDesc, &mSaplerState);
		}
	}

	SamplerState::~SamplerState()
	{
		if (mSaplerState)
			mSaplerState->Release();
	}
}

