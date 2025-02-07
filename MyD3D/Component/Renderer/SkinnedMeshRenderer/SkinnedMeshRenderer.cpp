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
    if (mMaterial)
    {
        for (int type = 0; type < MATERIAL_MAP_SIZE; ++type)
        {
            BOOL hasMap = mMaterial->mMaterialMapTexture[type] ? TRUE : FALSE;
            mMatCBuffer.SetHasingMap((eMaterialMapType)type, hasMap);
        }
    }
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
    if (mMaterial)
    {
        for (int type = 0; type < MATERIAL_MAP_SIZE; ++type)
        {
            BOOL hasMap = mMaterial->mMaterialMapTexture[type] ? TRUE : FALSE;
            mMatCBuffer.SetHasingMap((eMaterialMapType)type, hasMap);
        }
    }
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

        //if (Editor::InspectorViewer::IsFocusObject(gameObject))
        //{
        //    mOutlineCBuffer.outlineOffset = 1.0f;
        //    GraphicsManager::GetConstantBuffer(eCBufferType::Outline)->UpdateGPUResoure(&mOutlineCBuffer);
        //    EditorManager::mEditorCamera.PushOutlineDrawList(this);
        //}
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
        mTransformMatrices.World = XMMatrixTranspose(mRootBone->GetWorldMatrix() * mOffsetMatrix);
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
            mTransformMatrices.World = XMMatrixTranspose(gameObject->transform->GetWorldMatrix() * mOffsetMatrix);
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
        DebugRenderer::UpdateWorld(gameObject->transform->GetWorldMatrix() * mOffsetMatrix);
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
    ret["cast shadow"] = isCastShadow;

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

    ret["position"] = { mOffsetPosition.x, mOffsetPosition.y, mOffsetPosition.z };
    ret["rotation"] = { mOffsetRotation.x, mOffsetRotation.y, mOffsetRotation.z, mOffsetRotation.w };
    ret["scale"] = { mOffsetScale.x, mOffsetScale.y, mOffsetScale.z };

    return ret;
}

void SkinnedMeshRenderer::Deserialize(json& j)
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

    if (j.contains("cast shadow"))
        isCastShadow = j["cast shadow"].get<bool>();

    if (j.contains("Property"))
    {
        json propJson = j["property"];

        for (int i = 0; i < 4; i++)
        {
            if (propJson.contains("diffuse") && propJson["diffuse"].size() == 4)
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

    if (j.contains("position") && j["position"].size() == 3)
    {
        mOffsetPosition.x = j["position"][0].get<float>();
        mOffsetPosition.y = j["position"][1].get<float>();
        mOffsetPosition.z = j["position"][2].get<float>();
    }
    if (j.contains("rotation") && j["rotation"].size() == 4)
    {
        mOffsetRotation.x = j["rotation"][0].get<float>();
        mOffsetRotation.y = j["rotation"][1].get<float>();
        mOffsetRotation.z = j["rotation"][2].get<float>();
        mOffsetRotation.w = j["rotation"][3].get<float>();
    }
    if (j.contains("scale") && j["scale"].size() == 3)
    {
        mOffsetScale.x = j["scale"][0].get<float>();
        mOffsetScale.y = j["scale"][1].get<float>();
        mOffsetScale.z = j["scale"][2].get<float>();
    }

    if (j.contains("use map"))
        mMatCBuffer.UseMapFlag = j["use map"].get<unsigned int>();

    if (j.contains("root"))
    {
        Object* rootObj = static_cast<Object*>(Engine::SaveBase::mMap[j["root"].get<unsigned int>()]);
        mRootBone = rootObj->transform;
    }
}

void SkinnedMeshRenderer::EditorRendering(EditorViewerType _viewerType)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

    //////////////////////////////////////////////////////////////////////
    // RootBone
    //////////////////////////////////////////////////////////////////////
    {
        std::string widgetID = "NULL RootBone";
        if (mRootBone)
        {
            widgetID = Helper::ToString(mRootBone->gameObject->GetName());
            auto flags = ImGuiSelectableFlags_AllowDoubleClick;
            ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_RESOURCE);
            ImGui::Selectable((widgetID).c_str(), false, flags);
            EDITOR_COLOR_POP(1);
            if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            {
                Editor::InspectorViewer::SetFocusObject(mRootBone->gameObject);
                Editor::GuizmoManipulater::SetFocusObjedct(mRootBone->gameObject);
            }
            if (ImGui::IsItemHovered() && Input::IsMouseUp(Mouse::RIGHT)) 
            {
                SetRootBone(nullptr);
            }
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL);
            ImGui::Selectable(widgetID.c_str(), false, ImGuiSelectableFlags_Highlight);
            EDITOR_COLOR_POP(1);
        }
        Object* receiveObject = nullptr;
        if (EditorDragNDrop::ReceiveDragAndDropObjectData(widgetID.c_str(), &receiveObject))
        {
            SetRootBone(receiveObject->transform);
        }
    }
    ImGui::Separator();
    //////////////////////////////////////////////////////////////////////
    // Mesh
    //////////////////////////////////////////////////////////////////////
    {
        std::string widgetID = "NULL Mesh";
        if (mMesh)
        {
            mMesh->EditorRendering(EditorViewerType::DEFAULT);
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

        if (mMaterial)
        {
            mMaterial->EditorRendering(EditorViewerType::DEFAULT);
            widgetID = mMaterial->GetEID();
            if (ImGui::TreeNodeEx(("Material Porperties" + uid).c_str(), EDITOR_FLAG_RESOURCE))
            {
               for (int type = 0; type < MATERIAL_MAP_SIZE; ++type)
               {
                    eMaterialMapType mapType = (eMaterialMapType)type;
                    switch (mapType)
                    {
                    case eMaterialMapType::DIFFUSE:
                        ShowMaerialProperties(mapType, "Diffuse Map");
                        break;
                    case eMaterialMapType::SPECULAR:
                        ShowMaerialProperties(mapType, "Specular Map");
                        break;
                    case eMaterialMapType::AMBIENT:
                        ShowMaerialProperties(mapType, "Ambient Map");
                        break;
                    case eMaterialMapType::EMISSIVE:
                        ShowMaerialProperties(mapType, "Emissive Map");
                        break;
                    case eMaterialMapType::NORMAL:
                        ShowMaerialProperties(mapType, "Normal Map");
                        break;
                    case eMaterialMapType::ROUGHNESS:
                        ShowMaerialProperties(mapType, "Roughness Map");
                        break;
                    case eMaterialMapType::OPACITY:
                        ShowMaerialProperties(mapType, "Opacity Map");
                        break;
                    case eMaterialMapType::METALNESS:
                        ShowMaerialProperties(mapType, "Metalness Map");
                        break;
                    case eMaterialMapType::AMBIENT_OCCLUSION:
                        ShowMaerialProperties(mapType, "AmbientOcclusion Map");
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
        {
            ImGui::Text("Offset Position : ");
            if (ImGui::DragFloat3((uid + "Position").c_str(), &mOffsetPosition.x, 0.1f))
            {
                UpdateOffsetMatrix();
            }
        }
        {
            // 쿼터니언에서 오일러로
            Vector3 Euler = mOffsetRotation.ToEuler();
            Vector3 EulerDegrees;
            EulerDegrees.x = DirectX::XMConvertToDegrees(Euler.x);
            EulerDegrees.y = DirectX::XMConvertToDegrees(Euler.y);
            EulerDegrees.z = DirectX::XMConvertToDegrees(Euler.z);
            ImGui::Text("Offset Rotation : ");
            if (ImGui::DragFloat3((uid + "Rotation").c_str(), &EulerDegrees.x, 0.1f))
            {
                Euler.x = DirectX::XMConvertToRadians(EulerDegrees.x);
                Euler.y = DirectX::XMConvertToRadians(EulerDegrees.y);
                Euler.z = DirectX::XMConvertToRadians(EulerDegrees.z);
                // 라디안에서 쿼터니언으로
                mOffsetRotation = Quaternion::CreateFromYawPitchRoll(Euler.y, Euler.x, Euler.z);
                UpdateOffsetMatrix();
            }
        }
        {
            ImGui::Text("Offset Scale : ");
            if (ImGui::DragFloat3((uid + "Scale").c_str(), &mOffsetScale.x, 0.1f))
            {
                UpdateOffsetMatrix();
            }
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

void SkinnedMeshRenderer::ShowMaerialProperties(eMaterialMapType _type, const char* _label)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    if (mMaterial->mMaterialMapTexture[(UINT)_type])
    { 
        bool UseMap = (bool)mMatCBuffer.GetUsingMap(_type);
        ImGui::Separator();
        if (ImGui::Checkbox(("Using " + std::string(_label) + uid + std::to_string((UINT)_type)).c_str(), &UseMap)) {
            
                mMatCBuffer.SetUsingMap(_type, UseMap);
        } 
        if (ImGui::TreeNodeEx((std::string(_label) + mMaterial->mMaterialMapTexture[(UINT)_type]->GetEID() + uid).c_str(), EDITOR_FLAG_RESOURCE)) 
        {
                mMaterial->mMaterialMapTexture[(UINT)_type]->EditorRendering(EditorViewerType::INSPECTOR);
                ImGui::TreePop();
        }
    }
}
