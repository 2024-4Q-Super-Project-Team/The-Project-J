#include "pch.h"
#include "SkinnedMeshRenderer.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"
#include "ViewportScene/ViewportManager.h"
#include "World/WorldManager.h"
#include "Resource/ResourceManager.h"

#include "ViewportScene/ViewportScene.h"
#include "World/World.h"
#include "Object/Object.h"

SkinnedMeshRenderer::SkinnedMeshRenderer(Object* _owner)
    : RendererComponent(_owner)
    , mMesh(nullptr)
    , mRootBone(nullptr)
    , mMaterial(nullptr)
{
    SetEID("SkinnedMeshRenderer");
    mType = eComponentType::SKINNED_MESH_RENDERER;
}

SkinnedMeshRenderer::~SkinnedMeshRenderer()
{
}

void SkinnedMeshRenderer::Start()
{
    SetMesh(mMeshHandle);
    SetMaterial(mMaterialHandle);
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
    SetMesh(mMeshHandle);
    SetMaterial(mMaterialHandle);
}

void SkinnedMeshRenderer::EditorGlobalUpdate()
{
    gameObject->GetOwnerWorld()->
        mNeedResourceHandleTable.insert(mMeshHandle.GetParentkey());
    gameObject->GetOwnerWorld()->
        mNeedResourceHandleTable.insert(mMaterialHandle.GetParentkey());
}

void SkinnedMeshRenderer::EditorRender()
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
        EditorManager::mEditorCamera.PushDrawList(this);
    }
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
        //_camera->PushWireList(this);
    }
}

void SkinnedMeshRenderer::Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable)
{
    auto clone = _owner->AddComponent<SkinnedMeshRenderer>();
    // 루트 본 생성
    std::wstring rootBoneName = this->mRootBone->gameObject->GetName();
    auto ppRootBone = Helper::FindMap(rootBoneName, _objTable);
    if (ppRootBone)
    {
        clone->mRootBone = (*ppRootBone)->transform;
    }
    clone->SetMesh(this->mMeshHandle);
    clone->SetMaterial(this->mMaterialHandle);
}

void SkinnedMeshRenderer::DrawObject(Matrix& _view, Matrix& _projection)
{
    // 메쉬 바인딩
    if (mMesh)
    {
        // 머티리얼 바인딩
        if (mMaterial)
        {
            mMaterial->Bind();
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
        // '메쉬만' 바인딩
        if (mMesh)
        {
            mMesh->Bind();
            // View, Projection은 그림자의 V,P로 써야한다.
            mTransformMatrices.World = XMMatrixTranspose(gameObject->transform->GetWorldMatrix());
            mTransformMatrices.View = _pLight->GetProperty().ShadowView;
            mTransformMatrices.Projection = _pLight->GetProperty().ShadowProjection;
            GraphicsManager::GetConstantBuffer(eCBufferType::BoneMatrix)->UpdateGPUResoure(&mFinalBoneMatrices);
            GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->UpdateGPUResoure(&mTransformMatrices);
            D3DGraphicsRenderer::DrawCall(static_cast<UINT>(mMesh->mIndices.size()), 0, 0);
        }
    }
}

void SkinnedMeshRenderer::DrawWire()
{
    if (mMesh)
    {
        mMesh->Bind();
        DebugRenderer::UpdateWorld(gameObject->transform->GetWorldMatrix());
        D3DGraphicsRenderer::DrawCall(static_cast<UINT>(mMesh->mIndices.size()), 0, 0);
    }
}

MeshResource* SkinnedMeshRenderer::GetMesh()
{
    return mMesh;
}

MaterialResource* SkinnedMeshRenderer::GetMaterial()
{
    return mMaterial;
}

eBlendModeType SkinnedMeshRenderer::GetBlendMode()
{
    if (mMaterial)
    {
        return mMaterial->mBlendMode;
    }
    return eBlendModeType::OPAQUE_BLEND;
}

Vector3 SkinnedMeshRenderer::GetDistanceFromCamera(Camera* _camera)
{
    return _camera->GetDistance(gameObject->transform);
}

eRasterizerStateType SkinnedMeshRenderer::GetCullingMode()
{
    if (mMaterial)
    {
        return mMaterial->mRasterMode;
    }
    return eRasterizerStateType::BACKFACE_CULLING;
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
        mMaterialHandle = _handle;
        auto MatResource = ResourceManager::GetResource<MaterialResource>(_handle);
        if (MatResource)
        {
            mMaterial = MatResource;
            mMatCBuffer.MatProp = mMaterial->mMaterialProperty;
        }
        mMatCBuffer.MatProp = mMaterial->mMaterialProperty;
        
    }
}

void SkinnedMeshRenderer::SetMaterial(MaterialResource* _pResource)
{
    if (_pResource)
    {
        mMaterialHandle = _pResource->GetHandle();
        mMaterial = _pResource;
        mMatCBuffer.MatProp = mMaterial->mMaterialProperty;
    }
}

void SkinnedMeshRenderer::SetRootBone(Transform* _rootBone)
{
    mRootBone = _rootBone;
    isDirty = true;
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
        Transform** ppboneTransform = Helper::FindMap(bone->mKey, mBoneMappingTable);
        if (ppboneTransform == nullptr)
            continue;
        // 최종 본 매트릭스 계산 (본의 오프셋 매트릭스 * 본의 월드 매트릭스)
        mFinalBoneMatrices.finalBoneMatrix[i] = XMMatrixTranspose(bone->mOffsetMatrix * (*ppboneTransform)->GetWorldMatrix());
    }
}

void _CALLBACK SkinnedMeshRenderer::OnEnable()
{
    Start();
    return void _CALLBACK();
}

void _CALLBACK SkinnedMeshRenderer::OnDisable()
{
    mMesh = nullptr;
    mMaterial = nullptr;
    return void _CALLBACK();
}

void _CALLBACK SkinnedMeshRenderer::OnDestroy()
{
    return void _CALLBACK();
}

json SkinnedMeshRenderer::Serialize()
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
    ret["root"] = mRootBone->gameObject->GetId();

    ret["use map"] = mMatCBuffer.UseMapFlag;

    return ret;
}

void SkinnedMeshRenderer::Deserialize(json& j)
{
    SetId(j["id"].get<unsigned int>());

    mMeshHandle.Deserialize(j["mesh handle"]);
    mMaterialHandle.Deserialize(j["material handle"]);

    mMesh = ResourceManager::GetResource<MeshResource>(mMeshHandle);
    mMaterial = ResourceManager::GetResource<MaterialResource>(mMaterialHandle);

    json mProp = j["property"];

    for (int i = 0; i < 4; i++)
    {
        mMatCBuffer.MatProp.DiffuseRGB[i] = mProp["diffuse"][i].get<float>();
        mMatCBuffer.MatProp.AmbientRGB[i] = mProp["ambient"][i].get<float>();
        mMatCBuffer.MatProp.SpecularRGB[i] = mProp["specular"][i].get<float>();
    }

    mMatCBuffer.MatProp.RoughnessScale = mProp["roughness"].get<float>();
    mMatCBuffer.MatProp.MetallicScale = mProp["metallic"].get<float>();
    mMatCBuffer.MatProp.AmbienOcclusionScale = mProp["ao"].get<float>();

    mMatCBuffer.UseMapFlag = j["use map"].get<unsigned int>();

    Object* rootObj = static_cast<Object*>(Engine::SaveBase::mMap[j["root"].get<unsigned int>()]);
    mRootBone = rootObj->transform;
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

void SkinnedMeshRenderer::EditorRendering(EditorViewerType _viewerType)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

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
                    eMaterialMapType mapType = (eMaterialMapType)type;

                    switch (mapType)
                    {
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
    }

    ImGui::Separator();

    ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_EXTRA);
    if (ImGui::TreeNodeEx(("Lighting" + uid).c_str(), ImGuiTreeNodeFlags_Selected))
    {
        ImGui::Checkbox(("Rendering Shadows" + uid).c_str(), &isCastShadow);
        ImGui::TreePop();
    }
    EDITOR_COLOR_POP(1);
}
