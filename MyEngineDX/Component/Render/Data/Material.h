#pragma once

namespace Graphics
{
	class Texture;

	// FBX�� �ε��� �� �������� ����. 
	class MaterialInfo : public IGraphicsResource
	{
		using MaterialMapArray = std::array<std::shared_ptr<Texture>, static_cast<UINT>(eMaterialMapType::SIZE)>;
	public:
		explicit MaterialInfo(std::wstring_view _name);
		virtual ~MaterialInfo();
	public:
		inline void SetMaterialMap(eMaterialMapType _mapType, std::shared_ptr<Texture> _pTexture);
		inline std::shared_ptr<Texture> GetMaterialMap(eMaterialMapType _mapType);
	private:
		MaterialMapArray mMaterialMaps;
	};
	// ���������� ����� MaterialInfo�� ������ �޾ƿ���
	class MaterialState
	{
	public:
		explicit MaterialState(MaterialInfo* _spMaterialInfo);
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
		MaterialInfo* mMaterialInfo;
		CMaterialBuffer mCBuffer;
	};
}