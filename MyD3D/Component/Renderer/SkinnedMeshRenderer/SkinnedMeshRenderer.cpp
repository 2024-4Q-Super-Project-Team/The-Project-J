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
    // �޽��� �״�� ���ҽ� �����ϸ� �Ǵϱ� �����ͺ���
    clone->mMesh = this->mMesh;
    // ��Ʈ �� ����
    std::wstring rootBoneName = this->mRootBone->gameObject->GetName();
    auto ppRootBone = Helper::FindMap(rootBoneName, _objTable);
    if (ppRootBone)
    {
        clone->mRootBone = (*ppRootBone)->transform;
    }
    // ��Ƽ���� ����
    clone->SetMaterial(this->mMateiral->mMaterialResource);
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

std::shared_ptr<MeshResource> SkinnedMeshRenderer::GetMesh()
{
    return mMesh;
}

Material* SkinnedMeshRenderer::GetMaterial()
{
    return mMateiral;
}

void SkinnedMeshRenderer::SetMesh(std::shared_ptr<MeshResource> _mesh)
{
    mMesh = _mesh;
    isDirty = true;
}

void SkinnedMeshRenderer::SetRootBone(Transform* _rootBone)
{
    mRootBone = _rootBone;
    isDirty = true;
}

void SkinnedMeshRenderer::SetMaterial(std::shared_ptr<MaterialResource> _material)
{
    mMateiral->SetMaterial(_material);
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
        Transform** ppboneTransform = Helper::FindMap(bone->GetName(), mBoneMappingTable);
        if (ppboneTransform == nullptr)
            continue;
        // ���� �� ��Ʈ���� ��� (���� ������ ��Ʈ���� * ���� ���� ��Ʈ����)
        mFinalBoneMatrices.finalBoneMatrix[i] = XMMatrixTranspose(bone->mOffsetMatrix * (*ppboneTransform)->GetWorldMatrix());
    }
}

json SkinnedMeshRenderer::Serialize()
{
    json ret;
    ret["id"] = mId;
    ret["name"] = "SkinnedMeshRenderer";
    ret["mesh"] = mMesh->GetName();
    ret["material"] = mMateiral->mMaterialResource->GetName();
    ret["root bone"] = mRootBone ? mRootBone->mId : NULLID;
    return ret;
}

void SkinnedMeshRenderer::Deserialize(json& j)
{
 
}

void SkinnedMeshRenderer::EditorRendering()
{
    std::string uid = std::to_string(reinterpret_cast<uintptr_t>(this));
    if (ImGui::CollapsingHeader(("Skinned Mesh Renderer" + uid).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Separator();
        {
            std::string rootBoneName = "RootBone   : ";
            if (mRootBone)
                rootBoneName += Helper::ToString(mRootBone->gameObject->GetName());
            else
                rootBoneName += "NULL";
            ImGui::Text(rootBoneName.c_str());
        }
        {
            if (mMesh)
            {
				mMesh->EditorRendering();
            }
            else ImGui::Text("NULL Mesh");
        }
        ImGui::Separator();
        if (mMateiral)
        {
            if (mMateiral->mMaterialResource)
            {
                mMateiral->EditorRendering();
            }
            else ImGui::Text("NULL Material");
        }
    }
}
