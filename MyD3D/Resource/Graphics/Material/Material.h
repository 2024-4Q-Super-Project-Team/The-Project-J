#pragma once
#include "Resource/Resource.h"
#include "Graphics/ConstantBuffer.h"

class Texture2D;
class D3DGraphicsVertexShader;
class D3DGraphicsPixelShader;
struct MaterialCBuffer;

// ���� ��Ƽ���� ��ü���� ������ �� �ִ� ������ ���ҽ�
class MaterialResource 
    : public Resource
{
public: _READ_ONLY
    RESOURCE_TYPE(Material);
      explicit MaterialResource(std::wstring_view _name);
      virtual ~MaterialResource();
public:
    void SetMaterialMap(eMaterialMapType _mapType, const std::wstring& _pTexPath);
    void SetMaterialProperty(MaterialProperty* _pProp);
    void SetBlendingMode(eBlendType _type);
    const std::wstring& GetMaterialMapPath(eMaterialMapType _mapType);
public:
    // ��Ƽ������ ����� �� �� �ؽ��� ���
    std::wstring mMaterialMapPath[MATERIAL_MAP_SIZE];
    // ��Ƽ���� ���� �Ӽ�
    MaterialProperty mMaterialProperty;
    // ���� Ÿ��
    eBlendType mBlendMode = eBlendType::OPAQUE_BLEND;
    // �⺻ ��Ƽ����
    static std::shared_ptr<MaterialResource> DefaultMaterial;
public: 
	virtual void EditorRendering(EditorViewerType _viewerType) override;
};

// MaterialData�� �����ϸ�, �������� ��Ƽ���� ������� ���� ������ ��Ƽ���� 
// ��Ƽ������ ���� ����(��ǻ��, �ں��Ʈ, ����ŧ�� ��)�� �Ӽ����� ���������� ����.
class Material
    : public IEditorObject
{
public:
    explicit Material();
    explicit Material(std::shared_ptr<MaterialResource> _pMatResource);
    virtual ~Material();
public:
    void SetVertexShader(D3DGraphicsVertexShader* _pVertexShader);
    void SetPixelShader(D3DGraphicsPixelShader* _pVPixelShader);
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
    D3DGraphicsVertexShader* mVertexShader;
    // ��Ƽ������ ����� �ȼ� ���̴�
    D3DGraphicsPixelShader* mPixelShader;
public:
	virtual void EditorRendering(EditorViewerType _viewerType) override;
};