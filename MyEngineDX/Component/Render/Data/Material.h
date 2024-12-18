#pragma once

namespace Graphics
{
	class Texture;

	// FBX를 로드할 때 가져오는 정보. 
	class MaterialResource : public IGraphicsResource
	{
		using MaterialMapArray = std::array<std::shared_ptr<Texture>, static_cast<UINT>(eMaterialMapType::SIZE)>;
	public:
		explicit MaterialResource(std::wstring_view _name);
		virtual ~MaterialResource();
	public:
		inline void SetMaterialMap(eMaterialMapType _mapType, std::shared_ptr<Texture> _pTexture);
		inline std::shared_ptr<Texture> GetMaterialMap(eMaterialMapType _mapType);
	private:
		MaterialMapArray mMaterialMaps;
	};
	// 렌더러에서 사용할 MaterialResource의 정보를 받아오는
	class MaterialState
	{
	public:
		explicit MaterialState(MaterialResource* _spMaterialResource);
		virtual ~MaterialState();
	public:

		inline const std::wstring&	GetName();
		inline void		DiffuseColor(RGBA _rgba);
		inline RGBA&	DiffuseColor();
		inline void		AmbientColor(RGBA _rgba);
		inline RGBA&	AmbientColor();
		inline void		SpecularColor(RGBA _rgba);
		inline RGBA&	SpecularColor();
		inline void		SpecularPower(FLOAT _power);
		inline FLOAT&	SpecularPower();
		inline void		UseMaterialMapType(eMaterialMapType _type, BOOL _bUse);
		inline BOOL		UseMaterialMapType(eMaterialMapType _type);
		inline std::shared_ptr<Texture> GetMapTexture(eMaterialMapType _type);
	private:
		MaterialResource* mMaterialResource;
		CMaterialBuffer mCBuffer;
	};
}