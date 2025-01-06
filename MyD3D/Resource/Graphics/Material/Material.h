#pragma once
#include "Resource/Resource.h"
#include "Graphics/ConstantBuffer.h"

class Texture2D;
class VertexShader;
class PixelShader;
struct MaterialCBuffer;

// ���� ��Ƽ���� ��ü���� ������ �� �ִ� ������ ���ҽ�
class MaterialResource : public Resource
{
public: _READ_ONLY
    RESOURCE_TYPE(eResourceType::Material);
    explicit MaterialResource(std::wstring_view _name);
    virtual ~MaterialResource();
public:
    void SetMaterialMap(eMaterialMapType _mapType, const std::wstring& _pTexPath);
    void SetMaterialProperty(MaterialProperty* _pProp);
    const std::wstring& GetMaterialMapPath(eMaterialMapType _mapType);
    std::shared_ptr<VertexShader> GetVertexShader();
    std::shared_ptr<PixelShader> GetPixelShader();
public:
    // ��Ƽ������ ����� �� �� �ؽ��� ���
    std::wstring mMaterialMapPath[MATERIAL_MAP_SIZE];
    // ��Ƽ���� ���� �Ӽ�
    MaterialProperty mMaterialProperty;
    // TODO : ���� �����н� Ÿ�Ե� �־�����Ѵ�.

    static std::shared_ptr<MaterialResource> DefaultMaterial;
};

// MaterialData�� �����ϸ�, �������� ��Ƽ���� ������� ���� ������ ��Ƽ���� 
// ��Ƽ������ ���� ����(��ǻ��, �ں��Ʈ, ����ŧ�� ��)�� �Ӽ����� ���������� ����.
class Material
{
public:
    explicit Material();
    explicit Material(std::shared_ptr<MaterialResource> _pMatResource);
    virtual ~Material();
public:
    void SetVertexShader(std::shared_ptr<VertexShader> _spVertexShader);
    void SetPixelShader(std::shared_ptr<PixelShader> _spVPixelShader);
    void SetMaterial(std::shared_ptr<MaterialResource> _pMaterial);
    void UseMaterialMapType(eMaterialMapType _type, BOOL _bUse);
    std::shared_ptr<Texture2D> GetMapTexture(eMaterialMapType _type);
public:
    void Bind();
public:
    // �������� ��Ƽ���� ��� ����. �̰� �����͵� �������� ����.
    MaterialCBuffer mMatCBuffer;
    // ���̴��� ��Ƽ���� ���ҽ����� �����޴´�.
    std::shared_ptr<MaterialResource> mMaterialResource;
    // �ؽ��Ĵ� ��Ƽ���� ���ҽ��� ��θ� ������ ����.
    std::shared_ptr<Texture2D> mMaterialMaps[MATERIAL_MAP_SIZE];
    // ��Ƽ������ ����� ���ؽ� ���̴�
    std::shared_ptr<VertexShader> mVertexShader;
    // ��Ƽ������ ����� �ȼ� ���̴�
    std::shared_ptr<PixelShader> mPixelShader;
};