#include "pch.h"
#include "SkinnedMeshRenderer.h"
#include "Component/Camera/Camera.h"
#include "Component/Transform/Transform.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/Graphics/FBXModel/FBXModel.h"
#include "Resource/Graphics/Mesh/Mesh.h"
#include "Resource/Graphics/Material/Material.h"
#include "Resource/Graphics/Texture/Texture.h"
#include "Object/Object.h"

SkinnedMeshRenderer::SkinnedMeshRenderer(Object* _owner)
    : RendererComponent(_owner)
    , mMesh(nullptr)
    , mRootBone(nullptr)
    , mMateiral(nullptr)
{
    mType = eComponentType::SKINNED_MESH_RENDERER;
}

SkinnedMeshRenderer::~SkinnedMeshRenderer()
{
    SAFE_DELETE(mMateiral)
}

void SkinnedMeshRenderer::Start()
{
}

void SkinnedMeshRenderer::Tick()
{
}

void SkinnedMeshRenderer::FixedUpdate()
{
}

void SkinnedMeshRenderer::PreUpdate()
{
}

void SkinnedMeshRenderer::Update()
{
}

void SkinnedMeshRenderer::PostUpdate()
{
}

void SkinnedMeshRenderer::PreRender()
{
}

void SkinnedMeshRenderer::Render()
{
}

void SkinnedMeshRenderer::PostRender()
{
}

void SkinnedMeshRenderer::EditorUpdate()
{
}

void SkinnedMeshRenderer::EditorRender()
{
    if (mMesh && mRootBone)
    {
        // �޽��� ��Ʈ ���� �ٲ�κ��� ������ ����
        if (isDirty)
        {
            UpdateTable();
        }
        // �� Ʈ������ ���
        CalculateBoneTransform();
        EditorManager::mEditorCamera.PushDrawList(this);
    }
}

void SkinnedMeshRenderer::Draw(Camera* _camera)
{
    if (mMesh && mRootBone)
    {
        // �޽��� ��Ʈ ���� �ٲ�κ��� ������ ����
        if (isDirty)
        {
            UpdateTable();
        }
        // �� Ʈ������ ���
        CalculateBoneTransform();
        mTransformMatrices.World = XMMatrixTranspose(mRootBone->GetWorldMatrix());
        mTransformMatrices.View = XMMatrixTranspose(_camera->GetView());
        mTransformMatrices.Projection = XMMatrixTranspose(_camera->GetProjection());

        _camera->PushDrawList(this);
    }
}

void SkinnedMeshRenderer::Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable)
{
    auto clone = _owner->AddComponent<SkinnedMeshRenderer>();
    // ��Ʈ �� ����
    std::wstring rootBoneName = this->mRootBone->gameObject->GetName();
    auto ppRootBone = Helper::FindMap(rootBoneName, _objTable);
    if (ppRootBone)
    {
        clone->mRootBone = (*ppRootBone)->transform;
    }
    clone->SetMesh(this->mMeshHandle);
    clone->SetMaterial(this->mMaterialaHandle);
}

void SkinnedMeshRenderer::DrawObject(Matrix& _view, Matrix& _projection)
{
    // �޽� ���ε�
    if (mMesh)
    {
        // ��Ƽ���� ���ε�
        if (mMateiral)
        {
            mMateiral->Bind();
            GraphicsManager::GetConstantBuffer(eCBufferType::Material)->UpdateGPUResoure(&mMatCBuffer);
        }
        mMesh->Bind();
        mTransformMatrices.World = XMMatrixTranspose(mRootBone->GetWorldMatrix());
        mTransformMatrices.View = XMMatrixTranspose(_view);
        mTransformMatrices.Projection = XMMatrixTranspose(_projection);
        GraphicsManager::GetConstantBuffer(eCBufferType::BoneMatrix)->UpdateGPUResoure(&mFinalBoneMatrices);
        GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->UpdateGPUResoure(&mTransformMatrices);
        D3DGraphicsRenderer::DrawCall(static_cast<UINT>(mMesh->mIndices.size()), 0, 0);
    }
}

void SkinnedMeshRenderer::DrawShadow(Light* _pLight)
{
    if (isCastShadow)
    {
        // '�޽���' ���ε�
        if (mMesh)
        {
            mMesh->Bind();
            // View, Projection�� �׸����� V,P�� ����Ѵ�.
            mTransformMatrices.World = XMMatrixTranspose(gameObject->transform->GetWorldMatrix());
            mTransformMatrices.View = _pLight->GetProperty().ShadowView;
            mTransformMatrices.Projection = _pLight->GetProperty().ShadowProjection;
            GraphicsManager::GetConstantBuffer(eCBufferType::BoneMatrix)->UpdateGPUResoure(&mFinalBoneMatrices);
            GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->UpdateGPUResoure(&mTransformMatrices);
            D3DGraphicsRenderer::DrawCall(static_cast<UINT>(mMesh->mIndices.size()), 0, 0);
        }
    }
}

MeshResource* SkinnedMeshRenderer::GetMesh()
{
    return mMesh;
}

MaterialResource* SkinnedMeshRenderer::GetMaterial()
{
    return mMateiral;
}

eBlendModeType SkinnedMeshRenderer::GetBlendMode()
{
    if (mMateiral)
    {
        return mMateiral->mBlendMode;
    }
    return eBlendModeType::OPAQUE_BLEND;
}

void SkinnedMeshRenderer::SetMesh(ResourceHandle _handle)
{
    if (_handle.GetResourceType() == eResourceType::MeshResource)
    {
        mMeshHandle = _handle;
        mMesh = ResourceManager::GetResource<MeshResource>(_handle);
        isDirty = true;
    }
}

void SkinnedMeshRenderer::SetMesh(MeshResource* _pResource)
{
    if (_pResource)
    {
        mMeshHandle = _pResource->GetHandle();
        mMesh = _pResource;
    }
}

void SkinnedMeshRenderer::SetMaterial(ResourceHandle _handle)
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
        mMatCBuffer.MatProp = mMateiral->mMaterialProperty;
        
    }
}

void SkinnedMeshRenderer::SetMaterial(MaterialResource* _pResource)
{
    if (_pResource)
    {
        mMaterialaHandle = _pResource->GetHandle();
        mMateiral = _pResource;
        mMatCBuffer.MatProp = mMateiral->mMaterialProperty;
    }
}

void SkinnedMeshRenderer::SetRootBone(Transform* _rootBone)
{
    mRootBone = _rootBone;
    isDirty = true;
}

void SkinnedMeshRenderer::UpdateTable()
{
    // �� �� nullptr�̸� �ȵȴ�.
    if (!mMesh || !mRootBone)
        return;

    mBoneMappingTable.clear();
    BoneMapping(mRootBone);
    isDirty = false;
}

void SkinnedMeshRenderer::BoneMapping(Transform* curBone)
{
    if (!curBone) return;
    // ��ϵ� �޽��� �ش� ������Ʈ �̸� �˻�
    if (Helper::FindMap(curBone->gameObject->GetName(), mMesh->mBoneTable))
    {
        // �����ϸ� �� �������̺� �־��ش�.
        mBoneMappingTable[curBone->gameObject->GetName()] = curBone;
    }
    // �ڽ� ��嵵 �˻�
    // �˻��ϴ� ������ �ڽ� �����Ե� ������ ���� �� �ֱ� �����̴�.
    for (auto& child : curBone->GetChildren())
    {
        BoneMapping(child);
    }
}

void SkinnedMeshRenderer::CalculateBoneTransform()
{
    // mFinalBoneMatrices�� �޽��� �����̺��� �ε����� �°� 1��1 ��Ī�ؼ� ��Ʈ������ �־��ش�.
    for (int i = 0; i < mMesh->mBoneArray.size(); ++i)
    {
        Bone* const bone = mMesh->mBoneArray[i];
        // �޽��� �� �̸��� ���� �ش� ���� Ʈ�������� �����´�.
        Transform** ppboneTransform = Helper::FindMap(bone->mKey, mBoneMappingTable);
        if (ppboneTransform == nullptr)
            continue;
        // ���� �� ��Ʈ���� ��� (���� ������ ��Ʈ���� * ���� ���� ��Ʈ����)
        mFinalBoneMatrices.finalBoneMatrix[i] = XMMatrixTranspose(bone->mOffsetMatrix * (*ppboneTransform)->GetWorldMatrix());
    }
}

json SkinnedMeshRenderer::Serialize()
{
    json ret;
    ret["id"] = GetId();
    ret["name"] = "SkinnedMeshRenderer";
    if (mMesh) 
        ret["mesh"] = Helper::ToString(mMesh->GetKey());
    else 
        ret["mesh"] = nullptr;

    if (mMateiral) 
        ret["material"] = mMateiral->GetKey();
    else  
        ret["material"] = nullptr;
    ret["root bone"] = mRootBone ? mRootBone->GetId() : NULLID;
    return ret;
}

void SkinnedMeshRenderer::Deserialize(json& j)
{

}

void SkinnedMeshRenderer::EditorRendering(EditorViewerType _viewerType)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    if (ImGui::TreeNodeEx(("Skinned Mesh Renderer" + uid).c_str(), EDITOR_FLAG_MAIN))
    {
        if (mRootBone)
            ImGui::Text(Helper::ToString(mRootBone->gameObject->GetName()).c_str());
        else
            ImGui::Text("NULL RootBone");
        
        ImGui::Separator();
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

            if (mMateiral)
            {
                mMateiral->EditorRendering(EditorViewerType::DEFAULT);
                name = Helper::ToString(mMateiral->GetKey());
                widgetID = mMateiral->GetID();
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
