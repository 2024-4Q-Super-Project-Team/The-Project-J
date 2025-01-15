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
        // 메쉬나 루트 본이 바뀐부분이 있으면 갱신
        if (isDirty)
        {
            UpdateTable();
        }
        // 본 트랜스폼 계산
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
    // 메쉬는 그대로 리소스 참조하면 되니까 포인터복사
    clone->mMesh = this->mMesh;
    // 루트 본 생성
    std::wstring rootBoneName = this->mRootBone->gameObject->GetName();
    auto ppRootBone = Helper::FindMap(rootBoneName, _objTable);
    if (ppRootBone)
    {
        clone->mRootBone = (*ppRootBone)->transform;
    }
    // 머티리얼 생성
    clone->SetMaterial(this->mMateiral->mMaterialResource);
}

void SkinnedMeshRenderer::DrawMesh(Camera* _camera)
{
    // 머티리얼 바인딩
    if (mMateiral)
    {
        mMateiral->Bind();
    }
    // 메쉬 바인딩
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
    // '메쉬만' 바인딩
    if (mMesh)
    {
        mMesh->Bind();
    }
    // View, Projection은 그림자의 V,P로 써야한다.
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
    // 둘 다 nullptr이면 안된다.
    if (!mMesh || !mRootBone)
        return;

    mBoneMappingTable.clear();
    BoneMapping(mRootBone);
    isDirty = false;
}

void SkinnedMeshRenderer::BoneMapping(Transform* curBone)
{
    if (!curBone) return;
    // 등록된 메쉬에 해당 오브젝트 이름 검색
    if (Helper::FindMap(curBone->gameObject->GetName(), mMesh->mBoneTable))
    {
        // 존재하면 본 매핑테이블에 넣어준다.
        mBoneMappingTable[curBone->gameObject->GetName()] = curBone;
    }
    // 자식 노드도 검사
    // 검사하는 이유는 자식 본에게도 영향을 받을 수 있기 때문이다.
    for (auto& child : curBone->GetChildren())
    {
        BoneMapping(child);
    }
}

void SkinnedMeshRenderer::CalculateBoneTransform()
{
    // mFinalBoneMatrices는 메쉬의 본테이블의 인덱스에 맞게 1대1 매칭해서 매트릭스를 넣어준다.
    for (int i = 0; i < mMesh->mBoneArray.size(); ++i)
    {
        Bone* const bone = mMesh->mBoneArray[i];
        // 메쉬의 본 이름을 통해 해당 본의 트랜스폼을 가져온다.
        Transform** ppboneTransform = Helper::FindMap(bone->GetName(), mBoneMappingTable);
        if (ppboneTransform == nullptr)
            continue;
        // 최종 본 매트릭스 계산 (본의 오프셋 매트릭스 * 본의 월드 매트릭스)
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
