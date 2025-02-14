#include "pch.h"
#include "Material.h"
#include "Manager/GameManager.h"
#include "Resource/ResourceManager.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/Graphics/Texture/Texture.h"
// Editor
#include "Editor/EditorManager.h"

MaterialResource* MaterialResource::DefaultMaterial;

MaterialResource::MaterialResource(ResourceHandle _handle)
    : Resource(_handle)
{
    SetEID("Mateiral : " + Helper::ToString(_handle.GetKey()));
}

MaterialResource::~MaterialResource()
{
}

void MaterialResource::Create()
{
    for (int i = 0; i < MATERIAL_MAP_SIZE; ++i)
    {
        if (mMaterialMapTextureHandle[i].GetPath() != L"")
        {
            ResourceManager::RegisterResourceHandle(mMaterialMapTextureHandle[i]);
            ResourceManager::Alloc_Resource(mMaterialMapTextureHandle[i]);
            mMaterialMapTexture[i] = ResourceManager::GetResource<Texture2DResource>(mMaterialMapTextureHandle[i]);
            if (mMaterialMapTexture[i])
            {
                mMaterialMapTexture[i]->Texture->SetBindStage(eShaderStage::PS);
                mMaterialMapTexture[i]->Texture->SetBindSlot(i);
                if (i == (int)eMaterialMapType::ROUGHNESS) mMaterialProperty.RoughnessScale = 1.0f;
                if (i == (int)eMaterialMapType::METALNESS) mMaterialProperty.MetallicScale = 1.0f;
                if (i == (int)eMaterialMapType::AMBIENT_OCCLUSION) mMaterialProperty.AmbienOcclusionScale = 1.0f;
            }
        }
    }
}

void MaterialResource::Bind()
{
    GraphicsManager::GetRasterizerState(mRasterMode)->Bind();
    for (int i = 0; i < MATERIAL_MAP_SIZE; ++i)
    {
        if (mMaterialMapTexture[i])
        {
            mMaterialMapTexture[i]->Texture->SetBindStage(eShaderStage::PS);
            mMaterialMapTexture[i]->Texture->SetBindSlot(i);
            mMaterialMapTexture[i]->Texture->Bind();
        }
    }
}

void MaterialResource::SetMateirlaMapHandle(eMaterialMapType _type, ResourceHandle& _handle)
{
    mMaterialMapTextureHandle[(UINT)_type] = _handle;
}

void MaterialResource::SetMaterialProperty(MaterialProperty* _pProp)
{
    if (_pProp)
    {
        mMaterialProperty = *_pProp;
    }
}

void MaterialResource::SetBlendingMode(eBlendModeType _type)
{
    mBlendMode = _type;
}

void MaterialResource::SetCullingMode(eRasterizerStateType _type)
{
    mRasterMode = _type;
}

MaterialResource* MaterialResource::GetDefaultMaterial()
{
    if (!DefaultMaterial)
    {
        ResourceHandle handle = { eResourceType::MaterialResource, L"Default_Materail", L"", L"" };
        DefaultMaterial = new MaterialResource(handle);
    }
    return DefaultMaterial;
}

void MaterialResource::FreeDefaultMaterial()
{
}

json MaterialResource::Serialize()
{
    json ret;

    ret["id"] = GiveId();
    ColorF diffuse = mMaterialProperty.DiffuseRGB;
    ColorF ambient = mMaterialProperty.AmbientRGB;
    ColorF specular = mMaterialProperty.SpecularRGB;

    json mprop;
    mprop["diffuse"] = { diffuse.r,diffuse.g, diffuse.b, diffuse.a };
    mprop["ambient"] = { ambient.r, ambient.g, ambient.b, ambient.a };
    mprop["specular"] = { specular.r, specular.g, specular.b, specular.a };
    mprop["roughness"] = mMaterialProperty.RoughnessScale;
    mprop["metallic"] = mMaterialProperty.MetallicScale;
    mprop["ao"] = mMaterialProperty.AmbienOcclusionScale;

    ret["diffuse map handle"] = mMaterialMapTextureHandle[(UINT)eMaterialMapType::DIFFUSE].Serialize();
    ret["specular map handle"] = mMaterialMapTextureHandle[(UINT)eMaterialMapType::SPECULAR].Serialize();
    ret["ambient map handle"] = mMaterialMapTextureHandle[(UINT)eMaterialMapType::AMBIENT].Serialize();
    ret["emissive map handle"] = mMaterialMapTextureHandle[(UINT)eMaterialMapType::EMISSIVE].Serialize();
    ret["normal map handle"] = mMaterialMapTextureHandle[(UINT)eMaterialMapType::NORMAL].Serialize();
    ret["roughness map handle"] = mMaterialMapTextureHandle[(UINT)eMaterialMapType::ROUGHNESS].Serialize();
    ret["opacity map handle"] = mMaterialMapTextureHandle[(UINT)eMaterialMapType::OPACITY].Serialize();
    ret["metalness map handle"] = mMaterialMapTextureHandle[(UINT)eMaterialMapType::METALNESS].Serialize();
    ret["ambientocclusion map handle"] = mMaterialMapTextureHandle[(UINT)eMaterialMapType::AMBIENT_OCCLUSION].Serialize();

    ret["property"] = mprop;
    ret["blend type"] = mBlendMode;
    ret["raster type"] = mRasterMode;

    return ret;
}

void MaterialResource::Deserialize(json& j)
{
    if (j.contains("property"))
    {
        json propJson = j["property"];

        for (int i = 0; i < 4; i++)
        {
            if (propJson.contains("diffuse"))
                mMaterialProperty.DiffuseRGB[i] = propJson["diffuse"][i].get<float>();
            if (propJson.contains("ambient"))
                mMaterialProperty.AmbientRGB[i] = propJson["ambient"][i].get<float>();
            if (propJson.contains("specular"))
                mMaterialProperty.SpecularRGB[i] = propJson["specular"][i].get<float>();
        }

        if (propJson.contains("roughness"))
            mMaterialProperty.RoughnessScale = propJson["roughness"].get<float>();
        if (propJson.contains("metallic"))
            mMaterialProperty.MetallicScale = propJson["metallic"].get<float>();
        if (propJson.contains("ao"))
            mMaterialProperty.AmbienOcclusionScale = propJson["ao"].get<float>();
    }
    if (j.contains("diffuse map handle"))
        mMaterialMapTextureHandle[(UINT)eMaterialMapType::DIFFUSE].Deserialize(j["diffuse map handle"]);
    if (j.contains("specular map handle"))
        mMaterialMapTextureHandle[(UINT)eMaterialMapType::SPECULAR].Deserialize(j["specular map handle"]);
    if (j.contains("ambient map handle"))
        mMaterialMapTextureHandle[(UINT)eMaterialMapType::AMBIENT].Deserialize(j["ambient map handle"]);
    if (j.contains("emissive map handle"))
        mMaterialMapTextureHandle[(UINT)eMaterialMapType::EMISSIVE].Deserialize(j["emissive map handle"]);
    if (j.contains("normal map handle"))
        mMaterialMapTextureHandle[(UINT)eMaterialMapType::NORMAL].Deserialize(j["normal map handle"]);
    if (j.contains("roughness map handle"))
        mMaterialMapTextureHandle[(UINT)eMaterialMapType::ROUGHNESS].Deserialize(j["roughness map handle"]);
    if (j.contains("opacity map handle"))
        mMaterialMapTextureHandle[(UINT)eMaterialMapType::OPACITY].Deserialize(j["opacity map handle"]);
    if (j.contains("metalness map handle"))
        mMaterialMapTextureHandle[(UINT)eMaterialMapType::METALNESS].Deserialize(j["metalness map handle"]);
    if (j.contains("ambientocclusion map handle"))
        mMaterialMapTextureHandle[(UINT)eMaterialMapType::AMBIENT_OCCLUSION].Deserialize(j["ambientocclusion map handle"]);

    if (j.contains("blend type"))
        mBlendMode = static_cast<eBlendModeType>(j["blend type"].get<int>());
    if (j.contains("raster type"))
        mRasterMode = static_cast<eRasterizerStateType>(j["raster type"].get<int>());
}


void MaterialResource::EditorRendering(EditorViewerType _viewerType)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

    switch (_viewerType)
    {
    case EditorViewerType::DEFAULT:
    {
        std::string name = Helper::ToString(GetKey());
        ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_RESOURCE);
        auto flags = ImGuiSelectableFlags_AllowDoubleClick;
        if (ImGui::Selectable(GetEID(), false, flags))
        {
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                EditorManager::GetInspectorViewer()->SetFocusObject(this);
            }
        }
        EditorItemState state;
        state.mResourcePtr = this;
        state.mName = Helper::ToString(mHandle.GetKey());
        EditorDragNDrop::SendDragAndDropData(GetEID(), state);
        EDITOR_COLOR_POP(1);
        break;
    }
    case EditorViewerType::HIERARCHY:
        break;
    case EditorViewerType::INSPECTOR:
    {
        Resource::EditorRendering(_viewerType);

        {
            ImGui::Text("Diffuse : ");
            ImGui::ColorEdit3((uid + "Diffuse").c_str(), &mMaterialProperty.DiffuseRGB.r);
            ImGui::Text("Ambient : ");
            ImGui::ColorEdit3((uid + "Ambient").c_str(), &mMaterialProperty.AmbientRGB.r);
            ImGui::Text("Specular : ");
            ImGui::ColorEdit3((uid + "Specular").c_str(), &mMaterialProperty.SpecularRGB.r);
            ImGui::Text("Roughness Scale : ");
            ImGui::DragFloat((uid + "Roughness Scale").c_str(), &mMaterialProperty.RoughnessScale, 0.01f, 0.0f, 1.0f);
            ImGui::Text("Metallic Scale : ");
            ImGui::DragFloat((uid + "Metallic Scale").c_str(), &mMaterialProperty.MetallicScale, 0.01f, 0.0f, 1.0f);
            ImGui::Text("AmbienOcclusion Scale : ");
            ImGui::DragFloat((uid + "AmbienOcclusion Scale").c_str(), &mMaterialProperty.AmbienOcclusionScale, 0.01f, 0.0f, 1.0f);
        }

        ImGui::Separator();

        {   // 블렌드 타입
            const char* renderMode[] = { "Opaque", "Transparent" };
            int SelectIndex = (int)mBlendMode; // 현재 선택된 항목 (인덱스)

            ImGui::Text("Blend Mode");
            if (ImGui::Combo((uid + "Blend Mode").c_str(), &SelectIndex, renderMode, IM_ARRAYSIZE(renderMode)))
            {
                mBlendMode = (eBlendModeType)SelectIndex;
            }
        }

        ImGui::Separator();

        {   // 컬링 타입
            const char* cullingMode[] = { "None_Culling", "BackFace_Culling", "FrontFace_Culling"};
            int SelectIndex = (int)mRasterMode; // 현재 선택된 항목 (인덱스)

            ImGui::Text("Blend Mode");
            if (ImGui::Combo((uid + "Culling Mode").c_str(), &SelectIndex, cullingMode, IM_ARRAYSIZE(cullingMode)))
            {
                mRasterMode = (eRasterizerStateType)SelectIndex;
            }
        }

        for (int type = 0; type < MATERIAL_MAP_SIZE; ++type)
        {
            eMaterialMapType mapType = (eMaterialMapType)type;
            
            switch (mapType)
            {
            case eMaterialMapType::DIFFUSE:
                ShowMaerialMap(eMaterialMapType::DIFFUSE, "Diffuse Map");
                break;
            case eMaterialMapType::SPECULAR:
                ShowMaerialMap(eMaterialMapType::SPECULAR, "Specular Map");
                break;
            case eMaterialMapType::AMBIENT:
                ShowMaerialMap(eMaterialMapType::AMBIENT, "Ambient Map");
                break;
            case eMaterialMapType::EMISSIVE:
                ShowMaerialMap(eMaterialMapType::EMISSIVE, "Emissive Map");
                break;
            case eMaterialMapType::NORMAL:
                ShowMaerialMap(eMaterialMapType::NORMAL, "Normal Map");
                break;
            case eMaterialMapType::ROUGHNESS:
                ShowMaerialMap(eMaterialMapType::ROUGHNESS, "Roughness Map");
                break;
            case eMaterialMapType::OPACITY:
                ShowMaerialMap(eMaterialMapType::OPACITY, "Opacity Map");
                break;
            case eMaterialMapType::METALNESS:
                ShowMaerialMap(eMaterialMapType::METALNESS, "Metalness Map");
                break;
            case eMaterialMapType::AMBIENT_OCCLUSION:
                ShowMaerialMap(eMaterialMapType::AMBIENT_OCCLUSION, "AmbientOcclusion Map");
                break;
            case eMaterialMapType::SIZE:
                break;
            default:
                break;
            }
        }
        break;
    }
    default:
        break;
    }
}

void MaterialResource::ShowMaerialMap(eMaterialMapType _matType, const char* mapLabel)
{
    ImGui::Separator();
    ImGui::Text(mapLabel);
    std::string widgetID = "NULL " + std::string(mapLabel);
    if (mMaterialMapTexture[(UINT)_matType])
    {
        widgetID = mMaterialMapTexture[(UINT)_matType]->GetEID();
        mMaterialMapTexture[(UINT)_matType]->EditorRendering(EditorViewerType::DEFAULT);
        ImGui::PushID(widgetID.c_str());
        if (ImGui::IsItemHovered() && Input::IsMouseUp(Mouse::RIGHT)) {

            mMaterialMapTextureHandle[(UINT)_matType] = ResourceHandle();
            mMaterialMapTexture[(UINT)_matType] = nullptr;
        }
        else
        {
            mMaterialMapTexture[(UINT)_matType]->EditorRendering(EditorViewerType::INSPECTOR);
        }
        ImGui::PopID();
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL);
        ImGui::Selectable(widgetID.c_str(), false, ImGuiSelectableFlags_Highlight);
        EDITOR_COLOR_POP(1);
    }
    if (EditorDragNDrop::ReceiveDragAndDropResourceData<Texture2DResource>(widgetID.c_str(), &mMaterialMapTextureHandle[(UINT)_matType]))
    {
        if (mMaterialMapTextureHandle[(UINT)_matType].GetResourceType() == eResourceType::Texture2DResource) {

            mMaterialMapTexture[(UINT)_matType] = ResourceManager::GetResource<Texture2DResource>(mMaterialMapTextureHandle[(UINT)_matType]);
        }
    }
}

//Material::Material()
//{
//    SetVertexShader(GraphicsManager::GetVertexShader(eVertexShaderType::STANDARD));
//    SetPixelShader(GraphicsManager::GetPixelShader(ePixelShaderType::DEFERRED_PBR));
//    SetEID("Mateiral : " + Helper::ToString(_handle.GetKey()));
//}
//
//Material::Material(MaterialResource* _pMatResource)
//    : mMaterialResource(_pMatResource)
//{
//    SetMaterial(_pMatResource);
//    SetVertexShader(GraphicsManager::GetVertexShader(eVertexShaderType::STANDARD));
//    SetPixelShader(GraphicsManager::GetPixelShader(ePixelShaderType::DEFERRED_PBR));
//}
//
//Material::~Material()
//{
//}
//
//void Material::SetVertexShader(D3DGraphicsVertexShader* _pVertexShader)
//{
//    mVertexShader = _pVertexShader;
//}
//
//void Material::SetPixelShader(D3DGraphicsPixelShader* _pVPixelShader)
//{
//    mPixelShader = _pVPixelShader;
//}
//
//void Material::SetMaterial(MaterialResource* _pMaterial)
//{
//    mMaterialResource = _pMaterial;
//    mMaterialProperty = _pMaterial->mMaterialProperty;
//   
//}
//
//void Material::UseMaterialMapType(eMaterialMapType _type, BOOL _bUse)
//{
//    mMatCBuffer.SetUsingMap(_type, _bUse);
//}
//
//void Material::Bind()
//{
//    GraphicsManager::GetConstantBuffer(eCBufferType::Material)->UpdateGPUResoure(&mMatCBuffer);
//
//    //mVertexShader->Bind();
//    //mPixelShader->Bind();
//
//    for (int i = 0; i < MATERIAL_MAP_SIZE; ++i)
//    {
//        if (mMaterialResource)
//        {
//            if (mMaterialResource->mMaterialMapTexture[i])
//            {
//                if (mMatCBuffer.GetUsingMap((eMaterialMapType)i) == TRUE)
//                    mMaterialResource->mMaterialMapTexture[i]->Texture->Bind();
//                if (mMatCBuffer.GetUsingMap((eMaterialMapType)i) == FALSE)
//                    mMaterialResource->mMaterialMapTexture[i]->Texture->Reset();
//            }
//        }
//    }
//}

//void Material::EditorRendering(EditorViewerType _viewerType)
//{ 
//    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
//    std::string name = Helper::ToString(mMaterialResource->GetKey());
//
//    switch (_viewerType)
//    {
//    case EditorViewerType::DEFAULT:
//    {
//        ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_RESOURCE);
//        auto flags = ImGuiSelectableFlags_AllowDoubleClick;
//        if (ImGui::Selectable(GetEID(), false, flags))
//        {
//            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
//            {
//                EditorManager::GetInspectorViewer()->SetFocusObject(this);
//            }
//        }
//        EDITOR_COLOR_POP(1);
//        break;
//    }
//    case EditorViewerType::HIERARCHY:
//        break;
//    case EditorViewerType::INSPECTOR:
//    {
//        ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_RESOURCE);
//        ImGui::Text(("Material : " + name).c_str());
//        ImGui::Text("Diffuse : ");
//        ImGui::ColorEdit3((uid + "Diffuse").c_str(), &mMaterialProperty.DiffuseRGB.r);
//        ImGui::Text("Ambient : ");
//        ImGui::ColorEdit3((uid + "Ambient").c_str(), &mMaterialProperty.AmbientRGB.r);
//        ImGui::Text("Specular : ");
//        ImGui::ColorEdit3((uid + "Specular").c_str(), &mMaterialProperty.SpecularRGB.r);
//        ImGui::Text("Roughness Scale : ");
//        ImGui::DragFloat((uid + "Roughness Scale").c_str(), &mMaterialProperty.RoughnessScale, 0.01f, 0.0f, 1.0f);
//        ImGui::Text("Metallic Scale : ");
//        ImGui::DragFloat((uid + "Metallic Scale").c_str(), &mMaterialProperty.MetallicScale, 0.01f, 0.0f, 1.0f);
//        ImGui::Text("AmbienOcclusion Scale : ");
//        ImGui::DragFloat((uid + "AmbienOcclusion Scale").c_str(), &mMaterialProperty.AmbienOcclusionScale, 0.01f, 0.0f, 1.0f);
//        EDITOR_COLOR_POP(1);
//        ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_EXTRA);
//        for (int type = 0; type < MATERIAL_MAP_SIZE; ++type)
//        {
//            BOOL UseMap = mMatCBuffer.GetUsingMap((eMaterialMapType)type);
//            switch (type)
//            {
//            case (UINT)eMaterialMapType::DIFFUSE:
//                if (ImGui::Checkbox(("Using Diffuse Map" + uid).c_str(), (bool*)&UseMap))
//                {
//                    mMatCBuffer.SetUsingMap(eMaterialMapType::DIFFUSE, UseMap);
//                }
//                SHOW_MATERIAL_MAP_PROP(eMaterialMapType::DIFFUSE, "Diffuse Map");
//                break;
//            case (UINT)eMaterialMapType::SPECULAR:
//                if (ImGui::Checkbox(("Using Specular Map" + uid).c_str(), (bool*)&UseMap))
//                {
//                    mMatCBuffer.SetUsingMap(eMaterialMapType::SPECULAR, UseMap);
//                }
//                SHOW_MATERIAL_MAP_PROP(eMaterialMapType::SPECULAR, "Specular Map");
//                break;
//            case (UINT)eMaterialMapType::AMBIENT:
//                if (ImGui::Checkbox(("Using Ambient Map" + uid).c_str(), (bool*)&UseMap))
//                {
//                    mMatCBuffer.SetUsingMap(eMaterialMapType::AMBIENT, UseMap);
//                }
//                SHOW_MATERIAL_MAP_PROP(eMaterialMapType::AMBIENT, "Ambient Map");
//                break;
//            case (UINT)eMaterialMapType::EMISSIVE:
//                if (ImGui::Checkbox(("Using Emissive Map" + uid).c_str(), (bool*)&UseMap))
//                {
//                    mMatCBuffer.SetUsingMap(eMaterialMapType::EMISSIVE, UseMap);
//                }
//                SHOW_MATERIAL_MAP_PROP(eMaterialMapType::EMISSIVE, "Emissive Map");
//                break;
//            case (UINT)eMaterialMapType::NORMAL:
//                if (ImGui::Checkbox(("Using Normal Map" + uid).c_str(), (bool*)&UseMap))
//                {
//                    mMatCBuffer.SetUsingMap(eMaterialMapType::NORMAL, UseMap);
//                }
//                SHOW_MATERIAL_MAP_PROP(eMaterialMapType::NORMAL, "Normal Map");
//                break;
//            case (UINT)eMaterialMapType::ROUGHNESS:
//                if (ImGui::Checkbox(("Using Roughness Map" + uid).c_str(), (bool*)&UseMap))
//                {
//                    mMatCBuffer.SetUsingMap(eMaterialMapType::ROUGHNESS, UseMap);
//                }
//                SHOW_MATERIAL_MAP_PROP(eMaterialMapType::ROUGHNESS, "Roughness Map");
//                break;
//            case (UINT)eMaterialMapType::OPACITY:
//                if (ImGui::Checkbox(("Using Opacity Map" + uid).c_str(), (bool*)&UseMap))
//                {
//                    mMatCBuffer.SetUsingMap(eMaterialMapType::OPACITY, UseMap);
//                }
//                SHOW_MATERIAL_MAP_PROP(eMaterialMapType::OPACITY, "Opacity Map");
//                break;
//            case (UINT)eMaterialMapType::METALNESS:
//                if (ImGui::Checkbox(("Using Metalness Map" + uid).c_str(), (bool*)&UseMap))
//                {
//                    mMatCBuffer.SetUsingMap(eMaterialMapType::METALNESS, UseMap);
//                }
//                SHOW_MATERIAL_MAP_PROP(eMaterialMapType::METALNESS, "Metalness Map");
//                break;
//            case (UINT)eMaterialMapType::AMBIENT_OCCLUSION:
//                if (ImGui::Checkbox(("Using AmbienOcclusion Map" + uid).c_str(), (bool*)&UseMap))
//                {
//                    mMatCBuffer.SetUsingMap(eMaterialMapType::AMBIENT_OCCLUSION, UseMap);
//                }
//                SHOW_MATERIAL_MAP_PROP(eMaterialMapType::AMBIENT_OCCLUSION, "AmbienOcclusion Map");
//                break;
//            default:
//                break;
//            }
//        }
//        EDITOR_COLOR_POP(1);
//        break;
//    }
//    default:
//        break;
//    }
//}