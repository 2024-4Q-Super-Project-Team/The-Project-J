#include "pch.h"
#include "MeshRenderer.h"
#include "Object/Object.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/Graphics/FBXModel/FBXModel.h"
#include "Resource/Graphics/Mesh/Mesh.h"
#include "Resource/Graphics/Material/Material.h"
#include "Resource/Graphics/Texture/Texture.h"
// Editor
#include "Editor/Handler/EditorDragNDrop.h"

MeshRenderer::MeshRenderer(Object* _owner)
    : RendererComponent(_owner)
    , mMesh(nullptr)
    , mMateiral(nullptr)
{
    mType = eComponentType::MESH_RENDERER;
}

MeshRenderer::~MeshRenderer()
{
    SAFE_DELETE(mMateiral)
}

void MeshRenderer::Start()
{
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
    }
}

void MeshRenderer::Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable)
{
    auto clone = _owner->AddComponent<MeshRenderer>();
    clone->SetMesh(this->mMeshHandle);
    clone->SetMaterial(this->mMaterialaHandle);
}

void MeshRenderer::DrawObject(Matrix& _view, Matrix& _projection)
{
    
    if (mMesh)
    {
    // 머티리얼 바인딩
    if (mMateiral)
    {
        mMateiral->Bind();
        GraphicsManager::GetConstantBuffer(eCBufferType::Material)->UpdateGPUResoure(&mMatCBuffer);
    }
    // 메쉬 바인딩
    mMesh->Bind();
    mTransformMatrices.World        = XMMatrixTranspose(gameObject->transform->GetWorldMatrix());
    mTransformMatrices.View         = XMMatrixTranspose(_view);
    mTransformMatrices.Projection   = XMMatrixTranspose(_projection);
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
        mMaterialaHandle = _handle;
        auto MatResource = ResourceManager::GetResource<MaterialResource>(_handle);
        if (MatResource)
        {
            mMateiral = MatResource;
            mMatCBuffer.MatProp = mMateiral->mMaterialProperty;
        }
    }
}
void MeshRenderer::SetMaterial(MaterialResource* _pResource)
{
    if (_pResource)
    {
        mMaterialaHandle = _pResource->GetHandle();
        mMateiral = _pResource;
        mMatCBuffer.MatProp = mMateiral->mMaterialProperty;
    }
}

MeshResource* MeshRenderer::GetMesh()
{
    return mMesh;
}
MaterialResource* MeshRenderer::GetMaterial()
{
    return mMateiral;
}

eBlendModeType MeshRenderer::GetBlendMode()
{
    if (mMateiral)
    {
        return mMateiral->mBlendMode;
    }
    return eBlendModeType::OPAQUE_BLEND;
}

eRasterizerStateType MeshRenderer::GetCullingMode()
{
    if (mMateiral)
    {
        return mMateiral->mRasterMode;
    }
    return eRasterizerStateType::BACKFACE_CULLING;
}

json MeshRenderer::Serialize()
{
    json ret;
    ret["id"] = GetId();
    ret["name"] = "MeshRenderer";
    if (mMesh) 
        ret["mesh"] = Helper::ToString(mMesh->GetKey());
    else 
        ret["mesh"] = nullptr;

    if (mMateiral && mMateiral) 
        ret["material"] = mMateiral->GetKey();
    else  
        ret["material"] = nullptr;
    return ret;
}

void MeshRenderer::Deserialize(json& j)
{
}


void MeshRenderer::EditorRendering(EditorViewerType _viewerType)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    if (ImGui::TreeNodeEx(("Mesh Renderer##" + uid).c_str(), EDITOR_FLAG_MAIN))
    {
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
                widgetID = mMesh->GetID();
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL);
                ImGui::Selectable(widgetID.c_str() , false, ImGuiSelectableFlags_Highlight);
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
            if (mMateiral)
            {
                mMateiral->EditorRendering(EditorViewerType::DEFAULT);
                name = Helper::ToString(mMateiral->GetKey());
                uid = mMateiral->GetID();
                if (ImGui::TreeNodeEx(("Material Porperties" + uid).c_str(), EDITOR_FLAG_RESOURCE))
                {
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
                    for (int type = 0; type < MATERIAL_MAP_SIZE; ++type)
                    {
                        if (mMateiral->mMaterialMapTexture[type])
                        {
                            bool UseMap = (bool)mMatCBuffer.GetUsingMap((eMaterialMapType)type);
                            if (ImGui::Checkbox(("Using Map" + uid + std::to_string(type)).c_str(), (bool*)&UseMap))
                            {
                                mMatCBuffer.SetUsingMap((eMaterialMapType)type, UseMap);
                            }
                            mMateiral->mMaterialMapTexture[type]->EditorRendering(EditorViewerType::DEFAULT);
                            ImGui::Separator();
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
            if (EditorDragNDrop::ReceiveDragAndDropResourceData<MaterialResource>(widgetID.c_str(), &mMaterialaHandle))
            {
                SetMaterial(mMaterialaHandle);
            }
        }

        ImGui::Separator();

        ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_EXTRA);
        if (ImGui::TreeNodeEx(("Lighting" + uid).c_str(), ImGuiTreeNodeFlags_Selected))
        {
            ImGui::Checkbox(("Rendering Shadows" + uid).c_str(), &isCastShadow);
            ImGui::TreePop();
        }
        EDITOR_COLOR_POP(1);

        ImGui::TreePop();
    }
   
}
