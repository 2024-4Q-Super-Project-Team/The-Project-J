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

void SkinnedMeshRenderer::DrawCall()
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
    ret += {"mesh", mMesh->GetName()};
    ret += {"material", mMateiral->mMaterialResource->GetName()};
    return json();
}

json SkinnedMeshRenderer::Deserialize()
{
    return json();
}

#define SetMaterialEditor(typeEnum, label)                                                      \
    if (mMateiral->mMaterialMaps[(UINT)typeEnum])                                               \
    {                                                                                           \
        ImGui::Text(label);                                                                     \
        ImGui::Checkbox(("IsUse##" + std::string(label) + uid).c_str(), (bool*)&UseMap);        \
        mMateiral->mMatCBuffer.SetUsingMap(typeEnum, UseMap);                                   \
        std::wstring wTexPath = mMateiral->mMaterialResource->GetMaterialMapPath(typeEnum);     \
        std::string TexPath = Helper::ToString(wTexPath);                                       \
        ImGui::Text(TexPath.c_str());                                                           \
    }
void SkinnedMeshRenderer::EditorRendering()
{
    if (ImGui::CollapsingHeader("Skinned Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen))
    {
        std::string uid = std::to_string(reinterpret_cast<uintptr_t>(this));
        ImGui::Text("RootBone   : ");
        ImGui::SameLine(70);
        if (mRootBone)
        {
            std::string rootBoneName;
            rootBoneName.assign(mRootBone->gameObject->GetName().begin(),
                mRootBone->gameObject->GetName().end());
            ImGui::Text(rootBoneName.c_str());
        }
        else
        {
            ImGui::Text("NULL");
        }

        ImGui::Text("Mesh       : ");
        ImGui::SameLine(70);
        if (mMesh) {
            std::string meshMame;
            meshMame.assign(mMesh->GetName().begin(),
                mMesh->GetName().end());
            ImGui::Text(meshMame.c_str());
        }
        else
        {
            ImGui::Text("NULL");
        }

        ImGui::Text("Material   : ");
        ImGui::SameLine(70);
        if (mMateiral)
        {
            if (mMateiral->mMaterialResource)
            {
                MaterialResource* pMat = mMateiral->mMaterialResource.get();
                std::string name = Helper::ToString(pMat->GetName());
                ImGui::Text(name.c_str());

                ImGui::ColorEdit3(("Diffuse##" + name).c_str(), &mMateiral->mMatCBuffer.MatProp.DiffuseRGB.r);
                ImGui::ColorEdit3(("Ambient##" + name).c_str(), &mMateiral->mMatCBuffer.MatProp.AmbientRGB.r);
                ImGui::ColorEdit3(("Specular##" + name).c_str(), &mMateiral->mMatCBuffer.MatProp.SpecularRGB.r);
                ImGui::DragFloat(("Roughness Scale##" + name).c_str(), &mMateiral->mMatCBuffer.MatProp.RoughnessScale, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat(("Metallic Scale##" + name).c_str(), &mMateiral->mMatCBuffer.MatProp.MetallicScale, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat(("AmbienOcclusion Scale##" + name).c_str(), &mMateiral->mMatCBuffer.MatProp.AmbienOcclusionScale, 0.01f, 0.0f, 1.0f);

                for (int type = 0; type < MATERIAL_MAP_SIZE; ++type)
                {
                    BOOL UseMap = mMateiral->mMatCBuffer.GetUsingMap((eMaterialMapType)type);
                    switch (type)
                    {
                    case (UINT)eMaterialMapType::DIFFUSE:
                        SetMaterialEditor(eMaterialMapType::DIFFUSE, "Diffuse");
                        break;
                    case (UINT)eMaterialMapType::SPECULAR:
                        SetMaterialEditor(eMaterialMapType::SPECULAR, "Specular");
                        break;
                    case (UINT)eMaterialMapType::AMBIENT:
                        SetMaterialEditor(eMaterialMapType::AMBIENT, "Ambient");
                        break;
                    case (UINT)eMaterialMapType::EMISSIVE:
                        SetMaterialEditor(eMaterialMapType::EMISSIVE, "Emissive");
                        break;
                    case (UINT)eMaterialMapType::NORMAL:
                        SetMaterialEditor(eMaterialMapType::NORMAL, "Normal");
                        break;
                    case (UINT)eMaterialMapType::ROUGHNESS:
                        SetMaterialEditor(eMaterialMapType::ROUGHNESS, "Roughness");
                        break;
                    case (UINT)eMaterialMapType::OPACITY:
                        SetMaterialEditor(eMaterialMapType::OPACITY, "Opacity");
                        break;
                    case (UINT)eMaterialMapType::METALNESS:
                        SetMaterialEditor(eMaterialMapType::METALNESS, "Metalness");
                        break;
                    case (UINT)eMaterialMapType::AMBIENT_OCCLUSION:
                        SetMaterialEditor(eMaterialMapType::AMBIENT_OCCLUSION, "AmbienOcclusion");
                        break;
                    default:
                        break;
                    }
                }
            }
            else
            {
                ImGui::Text("NULL");
            }
        }
    }
}
