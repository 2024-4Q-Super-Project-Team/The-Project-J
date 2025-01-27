#include "pch.h"
#include "MeshRenderer.h"
#include "World/World.h"
#include "Object/Object.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/ResourceManager.h"
// Editor
#include "Editor/Handler/EditorDragNDrop.h"

MeshRenderer::MeshRenderer(Object* _owner)
    : RendererComponent(_owner)
    , mMesh(nullptr)
    , mMaterial(nullptr)
{
    SetEID("MeshRenderer");
    mType = eComponentType::MESH_RENDERER;
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Start()
{
    SetMesh(mMeshHandle);
    SetMaterial(mMaterialHandle);
}

void MeshRenderer::Tick()
{
}

void MeshRenderer::FixedUpdate()
{
}

void MeshRenderer::PreUpdate()
{
}

void MeshRenderer::Update()
{
}

void MeshRenderer::PostUpdate()
{
}

void MeshRenderer::PreRender()
{
}

void MeshRenderer::Render()
{
}

void MeshRenderer::PostRender()
{
}

void MeshRenderer::EditorUpdate()
{
    SetMesh(mMeshHandle);
    SetMaterial(mMaterialHandle);
}

void MeshRenderer::EditorGlobalUpdate()
{
    gameObject->GetOwnerWorld()->
        mNeedResourceHandleTable.insert(mMeshHandle.GetParentkey());
    gameObject->GetOwnerWorld()->
        mNeedResourceHandleTable.insert(mMaterialHandle.GetParentkey());
}

void MeshRenderer::EditorRender()
{
    if (mMesh)
    {
        EditorManager::mEditorCamera.PushDrawList(this);
    }
}

void MeshRenderer::Draw(Camera* _camera)
{
    if (mMesh)
    {
        mTransformMatrices.World        = XMMatrixTranspose(gameObject->transform->GetWorldMatrix());
        mTransformMatrices.View         = XMMatrixTranspose(_camera->GetView());
        mTransformMatrices.Projection   = XMMatrixTranspose(_camera->GetProjection());

        _camera->PushDrawList(this);
        //_camera->PushWireList(this);
    }
}

void MeshRenderer::Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable)
{
    auto clone = _owner->AddComponent<MeshRenderer>();
    clone->SetMesh(this->mMeshHandle);
    clone->SetMaterial(this->mMaterialHandle);
}

void MeshRenderer::DrawObject(Matrix& _view, Matrix& _projection)
{
    if (mMesh)
    {
        // 머티리얼 바인딩
        if (mMaterial)
        {
            mMaterial->Bind();
            GraphicsManager::GetConstantBuffer(eCBufferType::Material)->UpdateGPUResoure(&mMatCBuffer);
        }
        // 메쉬 바인딩
        mMesh->Bind();
        mTransformMatrices.World = XMMatrixTranspose(gameObject->transform->GetWorldMatrix());
        mTransformMatrices.View = XMMatrixTranspose(_view);
        mTransformMatrices.Projection = XMMatrixTranspose(_projection);
        // 트랜스폼 상수 버퍼 바인딩
        GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->UpdateGPUResoure(&mTransformMatrices);
        D3DGraphicsRenderer::DrawCall(static_cast<UINT>(mMesh->mIndices.size()), 0, 0);
    }
}

void MeshRenderer::DrawShadow(Light* _pLight)
{
    if (isCastShadow)
    {
        // '메쉬만' 바인딩
        if (mMesh)
        {
            mMesh->Bind();
            // View, Projection은 그림자의 V,P로 써야한다.
            mTransformMatrices.World = XMMatrixTranspose(gameObject->transform->GetWorldMatrix());
            mTransformMatrices.View = _pLight->GetProperty().ShadowView;
            mTransformMatrices.Projection = _pLight->GetProperty().ShadowProjection;
            // 트랜스폼 상수 버퍼 바인딩
            GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->UpdateGPUResoure(&mTransformMatrices);
            D3DGraphicsRenderer::DrawCall(static_cast<UINT>(mMesh->mIndices.size()), 0, 0);
        }
    }
}

void MeshRenderer::DrawWire()
{
    if (mMesh)
    {
        mMesh->Bind();
        DebugRenderer::UpdateWorld(gameObject->transform->GetWorldMatrix());
        D3DGraphicsRenderer::DrawCall(static_cast<UINT>(mMesh->mIndices.size()), 0, 0);
    }
}

void MeshRenderer::SetMesh(ResourceHandle _handle)
{
    if (_handle.GetResourceType() == eResourceType::MeshResource)
    {
        mMeshHandle = _handle;
        mMesh = ResourceManager::GetResource<MeshResource>(_handle);
    }
}
void MeshRenderer::SetMesh(MeshResource* _pResource)
{
    if (_pResource)
    {
        mMeshHandle = _pResource->GetHandle();
        mMesh = _pResource;
    }
}

void MeshRenderer::SetMaterial(ResourceHandle _handle)
{
    if (_handle.GetResourceType() == eResourceType::MaterialResource)
    {
        mMaterialHandle = _handle;
        auto MatResource = ResourceManager::GetResource<MaterialResource>(_handle);
        if (MatResource)
        {
            mMaterial = MatResource;
            mMatCBuffer.MatProp = mMaterial->mMaterialProperty;
        }
    }
}
void MeshRenderer::SetMaterial(MaterialResource* _pResource)
{
    if (_pResource)
    {
        mMaterialHandle = _pResource->GetHandle();
        mMaterial = _pResource;
        mMatCBuffer.MatProp = mMaterial->mMaterialProperty;
    }
}

MeshResource* MeshRenderer::GetMesh()
{
    return mMesh;
}
MaterialResource* MeshRenderer::GetMaterial()
{
    return mMaterial;
}

eBlendModeType MeshRenderer::GetBlendMode()
{
    if (mMaterial)
    {
        return mMaterial->mBlendMode;
    }
    return eBlendModeType::OPAQUE_BLEND;
}

Vector3 MeshRenderer::GetDistanceFromCamera(Camera* _camera)
{
    return _camera->GetDistance(gameObject->transform);
}

eRasterizerStateType MeshRenderer::GetCullingMode()
{
    if (mMaterial)
    {
        return mMaterial->mRasterMode;
    }
    return eRasterizerStateType::BACKFACE_CULLING;
}

void _CALLBACK MeshRenderer::OnEnable()
{
    Start();
    return void _CALLBACK();
}

void _CALLBACK MeshRenderer::OnDisable()
{
    mMesh = nullptr;
    mMaterial = nullptr;
    return void _CALLBACK();
}

void _CALLBACK MeshRenderer::OnDestroy()
{
    return void _CALLBACK();
}

json MeshRenderer::Serialize()
{
    json ret;
    ret["id"] = GetId();
    ret["name"] = "MeshRenderer";
    ret["mesh handle"] = mMeshHandle.Serialize();
    ret["material handle"] = mMaterialHandle.Serialize();

    json mprop;
    ColorF diffuse = mMatCBuffer.MatProp.DiffuseRGB;
    ColorF ambient = mMatCBuffer.MatProp.AmbientRGB;
    ColorF specular = mMatCBuffer.MatProp.SpecularRGB;
    mprop["diffuse"] = { diffuse.r,diffuse.g, diffuse.b, diffuse.a };
    mprop["ambient"] = { ambient.r, ambient.g, ambient.b, ambient.a };
    mprop["specular"] = { specular.r, specular.g, specular.b, specular.a };
    mprop["roughness"] = mMatCBuffer.MatProp.RoughnessScale;
    mprop["metallic"] = mMatCBuffer.MatProp.MetallicScale;
    mprop["ao"] = mMatCBuffer.MatProp.AmbienOcclusionScale;

    ret["property"] = mprop;

    ret["use map"] = mMatCBuffer.UseMapFlag;

    return ret;
}

void MeshRenderer::Deserialize(json& j)
{
    SetId(j["id"].get<unsigned int>());

    if (j.contains("mesh handle"))
    {
        mMeshHandle.Deserialize(j["mesh handle"]);
        SetMesh(ResourceManager::GetResource<MeshResource>(mMeshHandle));
    }
        
    if (j.contains("material handle"))
    {
        mMaterialHandle.Deserialize(j["material handle"]);
        SetMaterial(mMaterial = ResourceManager::GetResource<MaterialResource>(mMaterialHandle));
    }

    if (j.contains("Property"))
    {
        json propJson = j["property"];

        for (int i = 0; i < 4; i++)
        {
            if (propJson.contains("diffuse")&& propJson["diffuse"].size() == 4)
                mMatCBuffer.MatProp.DiffuseRGB[i] = propJson["diffuse"][i].get<float>();
            if (propJson.contains("ambient") && propJson["ambient"].size() == 4)
                mMatCBuffer.MatProp.AmbientRGB[i] = propJson["ambient"][i].get<float>();
            if (propJson.contains("specular") && propJson["specular"].size() == 4)
                mMatCBuffer.MatProp.SpecularRGB[i] = propJson["specular"][i].get<float>();
        }

        if (propJson.contains("roughness"))
            mMatCBuffer.MatProp.RoughnessScale = propJson["roughness"].get<float>();
        if (propJson.contains("metallic"))
            mMatCBuffer.MatProp.MetallicScale = propJson["metallic"].get<float>();
        if (propJson.contains("ao"))
            mMatCBuffer.MatProp.AmbienOcclusionScale = propJson["ao"].get<float>();
    }

    if (j.contains("use map"))
        mMatCBuffer.UseMapFlag = j["use map"].get<unsigned int>();
}

#define USEMAP_MATERIAL_MAP_RESUORCE(typeIndex, typeEnum, label) \
if (mMaterial->mMaterialMapTexture[typeIndex]) \
{ \
    bool UseMap = (bool)mMatCBuffer.GetUsingMap(typeEnum);\
    ImGui::Separator();\
    if (ImGui::Checkbox(("Using " + std::string(label) + uid + std::to_string(typeIndex)).c_str(), &UseMap)) {\
        mMatCBuffer.SetUsingMap(typeEnum, UseMap);\
    } \
    if (ImGui::TreeNodeEx((std::string(label) + mMaterial->mMaterialMapTexture[typeIndex]->GetEID() + uid).c_str(), EDITOR_FLAG_RESOURCE)) { \
        mMaterial->mMaterialMapTexture[typeIndex]->EditorRendering(EditorViewerType::INSPECTOR); \
        ImGui::TreePop();\
    }\
}\

void MeshRenderer::EditorRendering(EditorViewerType _viewerType)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    //////////////////////////////////////////////////////////////////////
    // Mesh
    //////////////////////////////////////////////////////////////////////
    {
        std::string widgetID = "NULL Mesh";
        std::string name = "NULL Mesh";
        if (mMesh)
        {
            mMesh->EditorRendering(EditorViewerType::DEFAULT);
            name = Helper::ToString(mMesh->GetKey());
            widgetID = mMesh->GetEID();
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL);
            ImGui::Selectable(widgetID.c_str(), false, ImGuiSelectableFlags_Highlight);
            EDITOR_COLOR_POP(1);
        }
        if (EditorDragNDrop::ReceiveDragAndDropResourceData<MeshResource>(widgetID.c_str(), &mMeshHandle))
        {
            SetMesh(mMeshHandle);
        }
    }
    ImGui::Separator();
    //////////////////////////////////////////////////////////////////////
    // Material
    //////////////////////////////////////////////////////////////////////
    {
        std::string widgetID = "NULL Material";
        std::string name = "NULL Material";
        if (mMaterial)
        {
            mMaterial->EditorRendering(EditorViewerType::DEFAULT);
            name = Helper::ToString(mMaterial->GetKey());
            widgetID = mMaterial->GetEID();
            if (ImGui::TreeNodeEx(("Material Porperties" + uid).c_str(), EDITOR_FLAG_RESOURCE))
            {
                for (int type = 0; type < MATERIAL_MAP_SIZE; ++type)
                {
                    eMaterialMapType mapType = (eMaterialMapType)type;

                    switch (mapType)
                    {
                    case eMaterialMapType::DIFFUSE:
                        USEMAP_MATERIAL_MAP_RESUORCE(type, mapType, "Diffuse Map");
                        break;
                    case eMaterialMapType::SPECULAR:
                        USEMAP_MATERIAL_MAP_RESUORCE(type, mapType, "Specular Map");
                        break;
                    case eMaterialMapType::AMBIENT:
                        USEMAP_MATERIAL_MAP_RESUORCE(type, mapType, "Ambient Map");
                        break;
                    case eMaterialMapType::EMISSIVE:
                        USEMAP_MATERIAL_MAP_RESUORCE(type, mapType, "Emissive Map");
                        break;
                    case eMaterialMapType::NORMAL:
                        USEMAP_MATERIAL_MAP_RESUORCE(type, mapType, "Normal Map");
                        break;
                    case eMaterialMapType::ROUGHNESS:
                        USEMAP_MATERIAL_MAP_RESUORCE(type, mapType, "Roughness Map");
                        break;
                    case eMaterialMapType::OPACITY:
                        USEMAP_MATERIAL_MAP_RESUORCE(type, mapType, "Opacity Map");
                        break;
                    case eMaterialMapType::METALNESS:
                        USEMAP_MATERIAL_MAP_RESUORCE(type, mapType, "Metalness Map");
                        break;
                    case eMaterialMapType::AMBIENT_OCCLUSION:
                        USEMAP_MATERIAL_MAP_RESUORCE(type, mapType, "AmbientOcclusion Map");
                        break;
                    case eMaterialMapType::SIZE:
                        break;
                    default:
                        break;
                    }
                }
                ImGui::TreePop();
            }
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL);
            ImGui::Selectable(widgetID.c_str(), false, ImGuiSelectableFlags_Highlight);
            EDITOR_COLOR_POP(1);
        }
        if (EditorDragNDrop::ReceiveDragAndDropResourceData<MaterialResource>(widgetID.c_str(), &mMaterialHandle))
        {
            SetMaterial(mMaterialHandle);
        }
        ImGui::Separator();
    }
    //////////////////////////////////////////////////////////////////////
    // Lighting Properties
    //////////////////////////////////////////////////////////////////////
    {
        ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_EXTRA);
        if (ImGui::TreeNodeEx(("Lighting Properties" + uid).c_str(), ImGuiTreeNodeFlags_Selected))
        {
            ImGui::Checkbox(("Rendering Shadows" + uid).c_str(), &isCastShadow);
            ImGui::TreePop();
        }
    }
    EDITOR_COLOR_POP(1);
}
