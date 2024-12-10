#pragma once

using namespace DirectX;

namespace Graphics
{
	class GraphicsDevice;

	class GraphicsTextrue
	{
	public:
		GraphicsTextrue(GraphicsDevice* _pDevice, std::wstring_view _path);
		~GraphicsTextrue();
	public:
	private:
		std::wstring mKey;
		ID3D11ShaderResourceView* mTexRV;
	public:
		operator ID3D11ShaderResourceView* () { return mTexRV; }
	};
}


