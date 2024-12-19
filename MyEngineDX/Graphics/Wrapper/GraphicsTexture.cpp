#include "pch.h"
#include "GraphicsTexture.h"
#include "../GraphicsDevice.h"

namespace Graphics
{
	Texture::Texture(GraphicsDevice* _pDevice, std::wstring_view _path)
		: IGraphicsResource(_path.data())
		, mTexRV(nullptr)
	{
		HRESULT hr;
		ScratchImage Image;
		std::wstring ext = mName.substr(_path.find_last_of(L".") + 1);
		if (ext == L"dds")
		{
			hr = LoadFromDDSFile(mName.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, Image);
		}
		else
		{
			hr = LoadFromWICFile(mName.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, Image);
		}
		ID3D11Device* pDevice = _pDevice->GetDevice();
		if (pDevice)
		{
			hr = CreateShaderResourceView(
				pDevice
				, Image.GetImages()
				, Image.GetImageCount()
				, Image.GetMetadata()
				, &mTexRV);
		}
	}
	Texture::~Texture()
	{
		if (mTexRV)
			mTexRV->Release();
	}
}