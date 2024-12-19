#pragma once

namespace Graphics
{
	class Texture;

	// FBX�� �ε��� �� �������� ����. 
	class MaterialResource : public IGraphicsResource
	{
		using MaterialMapArray = std::array<std::shared_ptr<Texture>, static_cast<UINT>(eMaterialMapType::SIZE)>;
	public:
		explicit MaterialResource(std::wstring_view _name);
		virtual ~MaterialResource();
	public:
		void SetMaterialMap(eMaterialMapType _mapType, std::shared_ptr<Texture> _pTexture) {
			mMaterialMaps[static_cast<UINT>(_mapType)] = _pTexture; 
		}
		std::shared_ptr<Texture> GetMaterialMap(eMaterialMapType _mapType) { 
			return mMaterialMaps[static_cast<UINT>(_mapType)];
		}
	private:
		MaterialMapArray mMaterialMaps;
	};
	// MaterialResource�� �����͸� ������ ���������� ����ϴ� ����
	class MaterialState
	{
	public:
		explicit MaterialState(MaterialResource* _spMaterialResource);
		virtual ~MaterialState();
	public:

		const std::wstring& GetName();

		void		DiffuseColor(RGBA _rgba);
		RGBA&		DiffuseColor();
		void		AmbientColor(RGBA _rgba);
		RGBA&		AmbientColor();
		void		SpecularColor(RGBA _rgba);
		RGBA&		SpecularColor();
		void		SpecularPower(FLOAT _power);
		FLOAT&		SpecularPower();
		void		UseMaterialMapType(eMaterialMapType _type, BOOL _bUse);
		BOOL		UseMaterialMapType(eMaterialMapType _type);
		std::shared_ptr<Texture> GetMapTexture(eMaterialMapType _type);
	private:
		MaterialResource* mMaterialResource;
		CMaterialBuffer mCBuffer;
	};
}