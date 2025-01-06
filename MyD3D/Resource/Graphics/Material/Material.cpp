#include "pch.h"
#include "Material.h"
#include "Manager/GameManager.h"
#include "Resource/ResourceManager.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/Graphics/Shader/Shader.h"
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

const std::wstring& MaterialResource::GetMaterialMapPath(eMaterialMapType _mapType)
{
    return  mMaterialMapPath[static_cast<UINT>(_mapType)];
}

Material::Material()
{
    SetVertexShader(ResourceManager::GetResource<VertexShader>(L"resource/shader/Standard_VS.cso"));
    SetPixelShader(ResourceManager::GetResource<PixelShader>(L"resource/shader/PBR_PS.cso"));
}

Material::Material(std::shared_ptr<MaterialResource> _pMatResource)
    : mMaterialResource(_pMatResource)
{
    SetMaterial(_pMatResource);
    SetVertexShader(ResourceManager::GetResource<VertexShader>(L"resource/shader/Standard_VS.cso"));
    SetPixelShader(ResourceManager::GetResource<PixelShader>(L"resource/shader/PBR_PS.cso"));
}

Material::~Material()
{
}

void Material::SetVertexShader(std::shared_ptr<VertexShader> _spVertexShader)
{
    mVertexShader = _spVertexShader;
}

void Material::SetPixelShader(std::shared_ptr<PixelShader> _spVPixelShader)
{
    mPixelShader = _spVPixelShader;
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
                mMaterialMaps[i]->GetTexture2D()->
                    SetShaderStage(eShaderStage::PS).
                    SetSlot(i);
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
    GraphicsManager::UpdateConstantBuffer(eCBufferType::Material, &mMatCBuffer);

    mVertexShader->Bind();
    mPixelShader->Bind();

    for (int i = 0; i < MATERIAL_MAP_SIZE; ++i)
    {
        if (mMaterialMaps[i])
        {
            if(mMatCBuffer.GetUsingMap((eMaterialMapType)i) == TRUE)
                mMaterialMaps[i]->Bind();
            if(mMatCBuffer.GetUsingMap((eMaterialMapType)i) == FALSE)
                mMaterialMaps[i]->Reset();
        }
    }
}
