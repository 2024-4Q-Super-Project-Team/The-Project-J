#include "pch.h"
#include "Material.h"
#include "Manager/GameManager.h"
#include "Resource/ResourceManager.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/Graphics/Texture/Texture.h"

#define SetMaterialEditor(typeEnum, label) \
if (mMaterialMaps[(UINT)typeEnum]) \
{ \
    ImGui::Separator();\
    if (ImGui::TreeNodeEx((label + uid).c_str(), ImGuiTreeNodeFlags_Selected)){ \
        ImGui::Checkbox(("Using " + std::string(label) + uid).c_str(), (bool*)&UseMap); \
        mMatCBuffer.SetUsingMap(typeEnum, UseMap); \
        std::wstring wTexPath = mMaterialResource->GetMaterialMapPath(typeEnum); \
        std::string TexPath = Helper::ToString(wTexPath); \
        ImGui::Text(TexPath.c_str()); \
        ImGui::Image((ImTextureID)mMaterialMaps[(UINT)typeEnum] \
            ->Texture->mSRV, ImVec2(150, 150)); \
        ImGui::TreePop(); \
    }\
}\

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

void MaterialResource::SetBlendingMode(eBlendType _type)
{
    mBlendMode = _type;
}

const std::wstring& MaterialResource::GetMaterialMapPath(eMaterialMapType _mapType)
{
    return  mMaterialMapPath[static_cast<UINT>(_mapType)];
}

void MaterialResource::EditorRendering()
{
    std::string name = Helper::ToString(mName);
    ImGui::Text(name.c_str());

}

Material::Material()
{
    SetVertexShader(GraphicsManager::GetVertexShader(eVertexShaderType::STANDARD));
    SetPixelShader(GraphicsManager::GetPixelShader(ePixelShaderType::DEFERRED_PBR));
}

Material::Material(std::shared_ptr<MaterialResource> _pMatResource)
    : mMaterialResource(_pMatResource)
{
    SetMaterial(_pMatResource);
    SetVertexShader(GraphicsManager::GetVertexShader(eVertexShaderType::STANDARD));
    SetPixelShader(GraphicsManager::GetPixelShader(ePixelShaderType::DEFERRED_PBR));
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

    //mVertexShader->Bind();
    //mPixelShader->Bind();

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

void Material::EditorRendering()
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    std::string name = Helper::ToString(mMaterialResource->GetName());

    EDITOR_COLOR_RESOURCE;
    if (ImGui::TreeNodeEx(("Material" + uid).c_str(), ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text(("Material : " + name).c_str());
        ImGui::Text("Diffuse : ");
        ImGui::ColorEdit3((uid + "Diffuse").c_str(), &mMatCBuffer.MatProp.DiffuseRGB.r);
        ImGui::Text("Ambient : ");
        ImGui::ColorEdit3((uid + "Ambient").c_str(), &mMatCBuffer.MatProp.AmbientRGB.r);
        ImGui::Text("Specular : ");
        ImGui::ColorEdit3((uid + "Specular").c_str(), &mMatCBuffer.MatProp.SpecularRGB.r);
        ImGui::Text("Roughness Scale : ");
        ImGui::DragFloat((uid + "Roughness Scale").c_str(), &mMatCBuffer.MatProp.RoughnessScale, 0.01f, 0.0f, 1.0f);
        ImGui::Text("Metallic Scale : ");
        ImGui::DragFloat((uid + "Metallic Scale").c_str(), &mMatCBuffer.MatProp.MetallicScale, 0.01f, 0.0f, 1.0f);
        ImGui::Text("AmbienOcclusion Scale : ");
        ImGui::DragFloat((uid + "AmbienOcclusion Scale").c_str(), &mMatCBuffer.MatProp.AmbienOcclusionScale, 0.01f, 0.0f, 1.0f);
        EDITOR_COLOR_EXTRA;
        for (int type = 0; type < MATERIAL_MAP_SIZE; ++type)
        {
            BOOL UseMap = mMatCBuffer.GetUsingMap((eMaterialMapType)type);
            switch (type)
            {
            case (UINT)eMaterialMapType::DIFFUSE:
                SetMaterialEditor(eMaterialMapType::DIFFUSE, "Diffuse Map");
                break;
            case (UINT)eMaterialMapType::SPECULAR:
                SetMaterialEditor(eMaterialMapType::SPECULAR, "Specular Map");
                break;
            case (UINT)eMaterialMapType::AMBIENT:
                SetMaterialEditor(eMaterialMapType::AMBIENT, "Ambient Map");
                break;
            case (UINT)eMaterialMapType::EMISSIVE:
                SetMaterialEditor(eMaterialMapType::EMISSIVE, "Emissive Map");
                break;
            case (UINT)eMaterialMapType::NORMAL:
                SetMaterialEditor(eMaterialMapType::NORMAL, "Normal Map");
                break;
            case (UINT)eMaterialMapType::ROUGHNESS:
                SetMaterialEditor(eMaterialMapType::ROUGHNESS, "Roughness Map");
                break;
            case (UINT)eMaterialMapType::OPACITY:
                SetMaterialEditor(eMaterialMapType::OPACITY, "Opacity Map");
                break;
            case (UINT)eMaterialMapType::METALNESS:
                SetMaterialEditor(eMaterialMapType::METALNESS, "Metalness Map");
                break;
            case (UINT)eMaterialMapType::AMBIENT_OCCLUSION:
                SetMaterialEditor(eMaterialMapType::AMBIENT_OCCLUSION, "AmbienOcclusion Map");
                break;
            default:
                break;
            }
        }
        EDITOR_COLOR_POP(1);
        ImGui::TreePop();
    }
    EDITOR_COLOR_POP(1);
}

//if (mMaterialMaps[(UINT)eMaterialMapType::DIFFUSE])
//{                                                                                          
//    ImGui::Text("Diffuse");
//    if (ImGui::TreeNodeEx(("Diffuse" + uid).c_str(), ImGuiTreeNodeFlags_OpenOnArrow))
//    {
//        ImGui::Checkbox(("IsUse##" + std::string("Diffuse") + uid).c_str(), (bool*)&UseMap);
//        mMatCBuffer.SetUsingMap(eMaterialMapType::DIFFUSE, UseMap);
//        std::wstring wTexPath = mMaterialResource->GetMaterialMapPath(eMaterialMapType::DIFFUSE);
//        std::string TexPath = Helper::ToString(wTexPath);
//        ImGui::Text(TexPath.c_str());
//        ImGui::Image((ImTextureID)mMaterialMaps[(UINT)eMaterialMapType::DIFFUSE]->Texture->mSRV, ImVec2(150, 150));
//    }
//}