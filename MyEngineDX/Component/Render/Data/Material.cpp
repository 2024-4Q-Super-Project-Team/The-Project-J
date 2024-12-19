#include "pch.h"
#include "Material.h"
#include "Graphics/GraphicsFramework.h"

namespace Graphics
{
	MaterialResource::MaterialResource(std::wstring_view _name)
		: IGraphicsResource(_name)
	{
	}
	MaterialResource::~MaterialResource()
	{
	}
	MaterialState::MaterialState(MaterialResource* _pMaterialResource)
		: mMaterialResource(_pMaterialResource)
	{
		for (int mapType = 0; mapType < static_cast<UINT>(eMaterialMapType::SIZE); ++mapType)
		{
			mCBuffer.UseMap[mapType] = TRUE;
		}
	}
	MaterialState::~MaterialState()
	{
	}
	const std::wstring& MaterialState::GetName()
	{
		return mMaterialResource->GetName();
	}
	void MaterialState::DiffuseColor(RGBA _rgba)
	{
		mCBuffer.DiffuseRGB = _rgba;
	}
	RGBA& MaterialState::DiffuseColor()
	{
		return mCBuffer.DiffuseRGB;
	}
	void MaterialState::AmbientColor(RGBA _rgba)
	{
		mCBuffer.AmbientRGB = _rgba;
	}
	RGBA& MaterialState::AmbientColor()
	{
		return mCBuffer.AmbientRGB;
	}
	void MaterialState::SpecularColor(RGBA _rgba)
	{
		mCBuffer.SpecularRGB = _rgba;
	}
	RGBA& MaterialState::SpecularColor()
	{
		return mCBuffer.SpecularRGB;
	}
	void MaterialState::SpecularPower(FLOAT _power)
	{
		mCBuffer.SpecularPower = _power;
	}
	FLOAT& MaterialState::SpecularPower()
	{
		return mCBuffer.SpecularPower;
	}
	void MaterialState::UseMaterialMapType(eMaterialMapType _type, BOOL _bUse)
	{
		mCBuffer.UseMap[static_cast<UINT>(_type)] = _bUse;
	}
	BOOL MaterialState::UseMaterialMapType(eMaterialMapType _type)
	{
		return mCBuffer.UseMap[static_cast<UINT>(_type)];
	}
	std::shared_ptr<Texture> MaterialState::GetMapTexture(eMaterialMapType _type)
	{
		return mMaterialResource->GetMaterialMap(_type);
	}
}