#pragma once
#include "Resource/Resource.h"
#include "Graphics/ConstantBuffer.h"

class Texture2DResource;
class D3DGraphicsVertexShader;
class D3DGraphicsPixelShader;
struct MaterialCBuffer;

// ���� ��Ƽ���� ��ü���� ������ �� �ִ� ������ ���ҽ�
class MaterialResource 
    : public Resource
{
public: _READ_ONLY
    RESOURCE_TYPE(MaterialResource);
      explicit MaterialResource(ResourceHandle _handle);
      virtual ~MaterialResource();
public:
    virtual void    Create() override;
    void            Bind();
public:
    void SetMateirlaMapHandle(eMaterialMapType _type, ResourceHandle& _handle);
    void SetMaterialProperty(MaterialProperty* _pProp);
    void SetBlendingMode(eBlendModeType _type);
    void SetCullingMode(eRasterizerStateType _type);
public:
    // ��Ƽ���� �ؽ���
    ResourceHandle          mMaterialMapTextureHandle[MATERIAL_MAP_SIZE];
    Texture2DResource*      mMaterialMapTexture[MATERIAL_MAP_SIZE] = { nullptr, };
    // ��Ƽ���� ���� �Ӽ�
    MaterialProperty        mMaterialProperty;
    // ���� Ÿ��
    eBlendModeType          mBlendMode = eBlendModeType::OPAQUE_BLEND;
    // �ø� Ÿ��
    eRasterizerStateType    mRasterMode = eRasterizerStateType::BACKFACE_CULLING;
public:
    // �⺻ ��Ƽ����  
    static MaterialResource* DefaultMaterial;
    static void InitDefaultMaterial();
public: 
	virtual void EditorRendering(EditorViewerType _viewerType) override;
};

//// MaterialData�� �����ϸ�, �������� ��Ƽ���� ������� ���� ������ ��Ƽ���� 
//// ��Ƽ������ ���� ����(��ǻ��, �ں��Ʈ, ����ŧ�� ��)�� �Ӽ����� ���������� ����.
//class Material
//    : public IEditorObject
//{
//public:
//    explicit Material();
//    explicit Material(MaterialResource* _pMatResource);
//    virtual ~Material();
//public:
//    void SetVertexShader(D3DGraphicsVertexShader* _pVertexShader);
//    void SetPixelShader(D3DGraphicsPixelShader* _pVPixelShader);
//    void SetMaterial(MaterialResource* _pMaterial);
//    void UseMaterialMapType(eMaterialMapType _type, BOOL _bUse);
//    Texture2DResource* GetMapTexture(eMaterialMapType _type);
//public:
//    void Bind();
//public:
//    // �������� ��Ƽ���� ��� ����. �̰� �����͵� �������� ����.
//    MaterialCBuffer             mMatCBuffer;
//    // ���̴��� ��Ƽ���� ���ҽ����� �����޴´�.
//    MaterialResource*           mMaterialResource;
//    // ��Ƽ������ ����� ���ؽ� ���̴�
//    D3DGraphicsVertexShader*    mVertexShader;
//    // ��Ƽ������ ����� �ȼ� ���̴�
//    D3DGraphicsPixelShader*     mPixelShader;
//public:
//	virtual void EditorRendering(EditorViewerType _viewerType) override;
//};