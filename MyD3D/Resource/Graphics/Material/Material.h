#pragma once
#include "Resource/Resource.h"
#include "Graphics/ConstantBuffer.h"

class Texture2D;
class D3DGraphicsVertexShader;
class D3DGraphicsPixelShader;
struct MaterialCBuffer;

// 여러 머티리얼 객체들이 공유할 수 있는 참조용 리소스
class MaterialResource : public Resource
{
public: _READ_ONLY
    RESOURCE_TYPE(eResourceType::Material);
      explicit MaterialResource(std::wstring_view _name);
      virtual ~MaterialResource();
public:
    void SetMaterialMap(eMaterialMapType _mapType, const std::wstring& _pTexPath);
    void SetMaterialProperty(MaterialProperty* _pProp);
    void SetBlendingMode(eBlendingMode _type);
    const std::wstring& GetMaterialMapPath(eMaterialMapType _mapType);
public:
    // 머티리얼이 사용할 각 맵 텍스쳐 경로
    std::wstring mMaterialMapPath[MATERIAL_MAP_SIZE];
    // 머티리얼 고유 속성
    MaterialProperty mMaterialProperty;
    // 블렌드 타입
    eBlendingMode mBlendMode = eBlendingMode::OPAQUE_BLEND;
    // 기본 머티리얼
    static std::shared_ptr<MaterialResource> DefaultMaterial;
};

// MaterialData를 참조하며, 독립적인 머티리얼 상수버퍼 값을 가지는 머티리얼 
// 머티리얼의 색상 정보(디퓨즈, 앰비언트, 스페큘러 등)와 속성들을 개별적으로 관리.
class Material
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
    // 독립적인 머티리얼 상수 버퍼. 이건 누구와도 공유하지 않음.
    MaterialCBuffer mMatCBuffer;
    // 셰이더를 머티리얼 리소스에게 공유받는다.
    std::shared_ptr<MaterialResource> mMaterialResource;
    // 텍스쳐는 머티리얼 리소스의 경로를 참조해 만듬.
    std::shared_ptr<Texture2D> mMaterialMaps[MATERIAL_MAP_SIZE];
    // 머티리얼이 사용할 버텍스 셰이더
    D3DGraphicsVertexShader* mVertexShader;
    // 머티리얼이 사용할 픽셀 셰이더
    D3DGraphicsPixelShader* mPixelShader;
};