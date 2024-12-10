#include "pch.h"
#include "GraphicsTextrue.h"
#include "../GraphicsDevice.h"

namespace Graphics
{
	GraphicsTextrue::GraphicsTextrue(GraphicsDevice* _pDevice, std::wstring_view _path)
		: mTexRV(nullptr)
		, mKey(_path.data())
	{
		HRESULT hr;
		ScratchImage Image;
		std::wstring ext = mKey.substr(_path.find_last_of(L".") + 1);
		if (ext == L"dds")
		{
			hr = LoadFromDDSFile(mKey.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, Image);
		}
		else
		{
			hr = LoadFromWICFile(mKey.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, Image);
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
	GraphicsTextrue::~GraphicsTextrue()
	{
		if (mTexRV)
			mTexRV->Release();
	}
}