#include "pch.h"
#include "Material.h"
#include "Manager/GameManager.h"
#include "Resource/ResourceManager.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/Graphics/Texture/Texture.h"

std::shared_ptr<MaterialResource> MaterialResource::DefaultMaterial = std::make_shared<MaterialResource>(L"Default_Material");

MaterialResource::MaterialResource(std::wstring_view _name)
    : Resource(_name)
{
}

MaterialResource::~MaterialResource()
{
}

void MaterialResource::SetMaterialMap(eMaterialMapType _mapType, const std::wstring& _pTexPath)
{
    mMaterialMapPath[static_cast<UINT>(_mapType)] = _pTexPath;
}

void MaterialResource::SetMaterialProperty(MaterialProperty* _pProp)
{
    if (_pProp)
    {
        mMaterialProperty = *_pProp;
    }
}

void MaterialResource::SetBlendingMode(eBlendingMode _type)
{
    mBlendMode = _type;
}

const std::wstring& MaterialResource::GetMaterialMapPath(eMaterialMapType _mapType)
{
    return  mMaterialMapPath[static_cast<UINT>(_mapType)];
}

Material::Material()
{
    SetVertexShader(GraphicsManager::GetVertexShader(eVertexShaderType::STANDARD));
    SetPixelShader(GraphicsManager::GetPixelShader(ePixelShaderType::PBR));
}

Material::Material(std::shared_ptr<MaterialResource> _pMatResource)
    : mMaterialResource(_pMatResource)
{
    SetMaterial(_pMatResource);
    SetVertexShader(GraphicsManager::GetVertexShader(eVertexShaderType::STANDARD));
    SetPixelShader(GraphicsManager::GetPixelShader(ePixelShaderType::PBR));
}

Material::~Material()
{
}

void Material::SetVertexShader(D3DGraphicsVertexShader* _pVertexShader)
{
    mVertexShader = _pVertexShader;
}

void Material::SetPixelShader(D3DGraphicsPixelShader* _pVPixelShader)
{
    mPixelShader = _pVPixelShader;
}

void Material::SetMaterial(std::shared_ptr<MaterialResource> _pMaterial)
{
    mMaterialResource = _pMaterial;
    mMatCBuffer.MatProp = _pMaterial->mMaterialProperty;
    for (int i = 0; i < MATERIAL_MAP_SIZE; ++i)
    {
        if (mMaterialResource->mMaterialMapPath[i] != L"")
        {
            mMaterialMaps[i] = ResourceManager::AddResource<Texture2D>(mMaterialResource->mMaterialMapPath[i]);
            if (mMaterialMaps[i])
            {
                mMaterialMaps[i]->Texture->SetBindStage(eShaderStage::PS);
                mMaterialMaps[i]->Texture->SetBindSlot(i);
                mMatCBuffer.SetUsingMap((eMaterialMapType)i, TRUE);
                if (i == (int)eMaterialMapType::ROUGHNESS) mMatCBuffer.MatProp.RoughnessScale = 1.0f;
                if (i == (int)eMaterialMapType::METALNESS) mMatCBuffer.MatProp.MetallicScale = 1.0f;
                if (i == (int)eMaterialMapType::AMBIENT_OCCLUSION) mMatCBuffer.MatProp.AmbienOcclusionScale = 1.0f;
            }
        }
    }
}

void Material::UseMaterialMapType(eMaterialMapType _type, BOOL _bUse)
{
    mMatCBuffer.SetUsingMap(_type, _bUse);
}

std::shared_ptr<Texture2D> Material::GetMapTexture(eMaterialMapType _type)
{
    return mMaterialMaps[static_cast<UINT>(_type)];
}

void Material::Bind()
{
    GraphicsManager::GetConstantBuffer(eCBufferType::Material)->UpdateGPUResoure(&mMatCBuffer);

    mVertexShader->Bind();
    mPixelShader->Bind();

    for (int i = 0; i < MATERIAL_MAP_SIZE; ++i)
    {
        if (mMaterialMaps[i])
        {
            if (mMatCBuffer.GetUsingMap((eMaterialMapType)i) == TRUE)
                mMaterialMaps[i]->Texture->Bind();
            if (mMatCBuffer.GetUsingMap((eMaterialMapType)i) == FALSE)
                mMaterialMaps[i]->Texture->Reset();
        }
    }
}
