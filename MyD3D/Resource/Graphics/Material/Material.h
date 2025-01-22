#pragma once
#include "Resource/Resource.h"
#include "Graphics/ConstantBuffer.h"

class Texture2DResource;
class D3DGraphicsVertexShader;
class D3DGraphicsPixelShader;
struct MaterialCBuffer;

// 여러 머티리얼 객체들이 공유할 수 있는 참조용 리소스
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
    // 머티리얼 텍스쳐
    ResourceHandle          mMaterialMapTextureHandle[MATERIAL_MAP_SIZE];
    Texture2DResource*      mMaterialMapTexture[MATERIAL_MAP_SIZE] = { nullptr, };
    // 머티리얼 고유 속성
    MaterialProperty        mMaterialProperty;
    // 블렌드 타입
    eBlendModeType          mBlendMode = eBlendModeType::OPAQUE_BLEND;
    // 컬링 타입
    eRasterizerStateType    mRasterMode = eRasterizerStateType::BACKFACE_CULLING;
public:
    // 기본 머티리얼  
    static MaterialResource* DefaultMaterial;
    static void InitDefaultMaterial();
public: 
	virtual void EditorRendering(EditorViewerType _viewerType) override;
};

//// MaterialData를 참조하며, 독립적인 머티리얼 상수버퍼 값을 가지는 머티리얼 
//// 머티리얼의 색상 정보(디퓨즈, 앰비언트, 스페큘러 등)와 속성들을 개별적으로 관리.
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
//    // 독립적인 머티리얼 상수 버퍼. 이건 누구와도 공유하지 않음.
//    MaterialCBuffer             mMatCBuffer;
//    // 셰이더를 머티리얼 리소스에게 공유받는다.
//    MaterialResource*           mMaterialResource;
//    // 머티리얼이 사용할 버텍스 셰이더
//    D3DGraphicsVertexShader*    mVertexShader;
//    // 머티리얼이 사용할 픽셀 셰이더
//    D3DGraphicsPixelShader*     mPixelShader;
//public:
//	virtual void EditorRendering(EditorViewerType _viewerType) override;
//};