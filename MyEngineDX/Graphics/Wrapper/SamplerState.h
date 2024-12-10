#pragma once

namespace Graphics
{
	class GraphicsDevice;
    struct SamplerDesc;

	class SamplerState
	{
	public:
		SamplerState(GraphicsDevice* _pDevice, SamplerDesc* _pDesc);
		~SamplerState();
	private:
		ID3D11SamplerState* mSaplerState;
    public:
        operator ID3D11SamplerState* () { return mSaplerState; }
	};
    struct SamplerDesc
    {
        D3D11_FILTER Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        D3D11_TEXTURE_ADDRESS_MODE AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        D3D11_TEXTURE_ADDRESS_MODE AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        D3D11_TEXTURE_ADDRESS_MODE AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        FLOAT MipLODBias;
        UINT MaxAnisotropy;
        D3D11_COMPARISON_FUNC ComparisonFunc = D3D11_COMPARISON_NEVER;
        FLOAT BorderColor[4];
        FLOAT MinLOD = 0.0f;
        FLOAT MaxLOD = D3D11_FLOAT32_MAX;
    };
}