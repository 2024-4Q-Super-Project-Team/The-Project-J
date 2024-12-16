#pragma once

using namespace DirectX;

namespace Graphics
{
	class GraphicsDevice;
	class IGraphicsResource;

	class Texture : public IGraphicsResource
	{
	public:
		explicit Texture(GraphicsDevice* _pDevice, std::wstring_view _path);
		virtual ~Texture();
		Texture(Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture(Texture&&) noexcept = default;
		Texture& operator=(Texture&&) noexcept = default;
	public:
	private:
		ID3D11ShaderResourceView* mTexRV;
	public:
		operator ID3D11ShaderResourceView* () { return mTexRV; }
	};
}


