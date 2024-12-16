#include "pch.h"
#include "Material.h"

namespace Graphics
{
	MaterialInfo::MaterialInfo(std::wstring_view _name)
		: IGraphicsResource(_name)
	{
	}
	MaterialInfo::~MaterialInfo()
	{
	}
	void MaterialInfo::SetMaterialMap(eMaterialMapType _mapType, std::shared_ptr<Texture> _pTexture)
	{
		mMaterialMaps[static_cast<UINT>(_mapType)] =  _pTexture;
	}
	inline std::shared_ptr<Texture> MaterialInfo::GetMaterialMap(eMaterialMapType _mapType)
	{
		return mMaterialMaps[static_cast<UINT>(_mapType)];
	}
	MaterialState::MaterialState(MaterialInfo* _pMaterialInfo)
		: mMaterialInfo(_pMaterialInfo)
	{
		for (int mapType = 0; mapType < static_cast<UINT>(eMaterialMapType::SIZE); ++mapType)
		{
			mCBuffer.UseMap[mapType] = TRUE;
		}
	}
	MaterialState::~MaterialState()
	{
	}
	inline const std::wstring& MaterialState::GetName()
	{
		return mMaterialInfo->GetName();
	}
	inline void MaterialState::DiffuseColor(RGBA _rgba)
	{
		mCBuffer.DiffuseRGB = _rgba;
	}
	inline RGBA& MaterialState::DiffuseColor()
	{
		return mCBuffer.DiffuseRGB;
	}
	inline void MaterialState::AmbientColor(RGBA _rgba)
	{
		mCBuffer.AmbientRGB = _rgba;
	}
	inline RGBA& MaterialState::AmbientColor()
	{
		return mCBuffer.AmbientRGB;
	}
	inline void MaterialState::SpecularColor(RGBA _rgba)
	{
		mCBuffer.SpecularRGB = _rgba;
	}
	inline RGBA& MaterialState::SpecularColor()
	{
		return mCBuffer.SpecularRGB;
	}
	inline void MaterialState::SpecularPower(FLOAT _power)
	{
		mCBuffer.SpecularPower = _power;
	}
	inline FLOAT& MaterialState::SpecularPower()
	{
		return mCBuffer.SpecularPower;
	}
	inline void MaterialState::UseMaterialMapType(eMaterialMapType _type, BOOL _bUse)
	{
		mCBuffer.UseMap[static_cast<UINT>(_type)] = _bUse;
	}
	inline BOOL MaterialState::UseMaterialMapType(eMaterialMapType _type)
	{
		return mCBuffer.UseMap[static_cast<UINT>(_type)];
	}
	inline std::shared_ptr<Texture> MaterialState::GetMapTexture(eMaterialMapType _type)
	{
		return mMaterialInfo->GetMaterialMap(_type);
	}
}