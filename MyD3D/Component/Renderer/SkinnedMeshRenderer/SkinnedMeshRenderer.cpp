#include "pch.h"
#include "SkinnedMeshRenderer.h"
#include "Component/Camera/Camera.h"
#include "Component/Transform/Transform.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/Graphics/FBXModel/FBXModel.h"
#include "Resource/Graphics/Mesh/Mesh.h"
#include "Resource/Graphics/Material/Material.h"
#include "Resource/Graphics/Bone/Bone.h"
#include "Resource/Graphics/Texture/Texture.h"
#include "Object/Object.h"

SkinnedMeshRenderer::SkinnedMeshRenderer(Object* _owner)
    : RendererComponent(_owner)
    , mMesh(nullptr)
    , mRootBone(nullptr)
    , mMateiral(new Material)
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

void SkinnedMeshRenderer::DrawMesh(Camera* _camera)
{
    // ��Ƽ���� ���ε�
    if (mMateiral)
    {
        mMateiral->Bind();
    }
    // �޽� ���ε�
    if (mMesh)
    {
        mMesh->Bind();
    }
    mTransformMatrices.World = XMMatrixTranspose(mRootBone->GetWorldMatrix());
    mTransformMatrices.View = XMMatrixTranspose(_camera->GetView());
    mTransformMatrices.Projection = XMMatrixTranspose(_camera->GetProjection());
    GraphicsManager::GetConstantBuffer(eCBufferType::BoneMatrix)->UpdateGPUResoure(&mFinalBoneMatrices);
    GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->UpdateGPUResoure(&mTransformMatrices);
    D3DGraphicsRenderer::DrawCall(static_cast<UINT>(mMesh->mIndices.size()), 0, 0);
}

void SkinnedMeshRenderer::DrawShadow(Light* _pLight)
{
    if (isCastShadow)
    {
        // '�޽���' ���ε�
        if (mMesh)
        {
            mMesh->Bind();
        }
        // View, Projection�� �׸����� V,P�� ����Ѵ�.
        mTransformMatrices.World = XMMatrixTranspose(gameObject->transform->GetWorldMatrix());
        mTransformMatrices.View = _pLight->GetProperty().ShadowView;
        mTransformMatrices.Projection = _pLight->GetProperty().ShadowProjection;
        GraphicsManager::GetConstantBuffer(eCBufferType::BoneMatrix)->UpdateGPUResoure(&mFinalBoneMatrices);
        GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->UpdateGPUResoure(&mTransformMatrices);
        D3DGraphicsRenderer::DrawCall(static_cast<UINT>(mMesh->mIndices.size()), 0, 0);
    }
}

std::shared_ptr<MeshResource> SkinnedMeshRenderer::GetMesh()
{
    return mMesh;
}

Material* SkinnedMeshRenderer::GetMaterial()
{
    return mMateiral;
}

void SkinnedMeshRenderer::SetMesh(ResourceHandle _handle)
{
    if (_handle.GetResourceType() == eResourceType::Mesh)
    {
        mMeshHandle = _handle;
        mMesh = ResourceManager::RequestResource<MeshResource>(_handle);
        isDirty = true;
    }
}

void SkinnedMeshRenderer::SetRootBone(Transform* _rootBone)
{
    mRootBone = _rootBone;
    isDirty = true;
}

void SkinnedMeshRenderer::SetMaterial(ResourceHandle _handle)
{
    if (_handle.GetResourceType() == eResourceType::Material)
    {
        mMaterialaHandle = _handle;
        auto MatResource = ResourceManager::RequestResource<MaterialResource>(_handle);
        if (MatResource)
        {
            mMateiral->SetMaterial(MatResource);
        }
    }
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
        Transform** ppboneTransform = Helper::FindMap(bone->GetKey(), mBoneMappingTable);
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

    if (mMateiral && mMateiral->mMaterialResource) 
        ret["material"] = mMateiral->mMaterialResource->GetKey();
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

		if (mMesh)
		{
			mMesh->EditorRendering(EditorViewerType::DEFAULT);
		}
		else ImGui::Text("NULL Mesh");

        ImGui::Separator();

        if (mMateiral)
        {
            if (mMateiral->mMaterialResource)
            {
                mMateiral->EditorRendering(EditorViewerType::DEFAULT);
            }
            else ImGui::Text("NULL Material");
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
