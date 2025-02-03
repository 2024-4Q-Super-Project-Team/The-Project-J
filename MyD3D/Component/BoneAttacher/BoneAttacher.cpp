#include "pch.h"
#include "BoneAttacher.h"

BoneAttacher::BoneAttacher(Object* _owner)
    : Component(_owner)
    , mDestBone(nullptr)
{
    mType = eComponentType::BONE_ATTACHER;
    SetEID("BoneAttacher");
}

BoneAttacher::~BoneAttacher()
{
}

void BoneAttacher::Start()
{
}

void BoneAttacher::Tick()
{
}

void BoneAttacher::FixedUpdate()
{
}

void BoneAttacher::PreUpdate()
{
}

void BoneAttacher::Update()
{
}

void BoneAttacher::PostUpdate()
{
}

void BoneAttacher::PreRender()
{
}

void BoneAttacher::Render()
{
    if (!mDestBone)
        return;

    // 1. mDestBone의 월드 변환 행렬 가져오기
    Matrix destWorldMatrix = mDestBone->GetWorldMatrix();

    // 2. 오프셋 변환 행렬 만들기 (위치, 회전, 스케일 포함)
    Matrix tr = DirectX::XMMatrixTranslationFromVector(mOffsetPosition);
    Matrix ro = DirectX::XMMatrixRotationQuaternion(mOffsetRotation);
    Matrix sc = DirectX::XMMatrixScalingFromVector(mOffsetScale);
    Matrix offsetMatrix = sc * ro * tr;

    // 3. 최종 변환 행렬 = mDestBone의 월드 변환 * 오프셋
    Matrix finalWorldMatrix = destWorldMatrix * offsetMatrix;

    // 4. gameObject의 트랜스폼을 최종 행렬로 설정
    gameObject->transform->SetLocalMatrix(finalWorldMatrix);
}

void BoneAttacher::Draw(Camera* _camera)
{
}

void BoneAttacher::PostRender()
{
}

void BoneAttacher::EditorUpdate()
{
    if (!mDestBone)
        return;

    // 1. mDestBone의 월드 변환 행렬 가져오기
    Matrix destWorldMatrix = mDestBone->GetWorldMatrix();

    // 2. 오프셋 변환 행렬 만들기 (위치, 회전, 스케일 포함)
    Matrix tr = DirectX::XMMatrixTranslationFromVector(mOffsetPosition);
    Matrix ro = DirectX::XMMatrixRotationQuaternion(mOffsetRotation);
    Matrix sc = DirectX::XMMatrixScalingFromVector(mOffsetScale);
    Matrix offsetMatrix = sc * ro * tr;

    // 3. 최종 변환 행렬 = mDestBone의 월드 변환 * 오프셋
    Matrix finalWorldMatrix = destWorldMatrix * offsetMatrix;

    // 4. gameObject의 트랜스폼을 최종 행렬로 설정
    gameObject->transform->SetLocalMatrix(finalWorldMatrix);
}

void BoneAttacher::EditorRender()
{
}

void BoneAttacher::EditorRendering(EditorViewerType _viewerType)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    {
        std::string widgetID = "NULL DestBone";
        if (mDestBone)
        {
            widgetID = Helper::ToString(mDestBone->gameObject->GetName());
            auto flags = ImGuiSelectableFlags_AllowDoubleClick;
            ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_RESOURCE);
            ImGui::Selectable((widgetID).c_str(), false, flags);
            EDITOR_COLOR_POP(1);
            if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            {
                Editor::InspectorViewer::SetFocusObject(mDestBone->gameObject);
                Editor::GuizmoManipulater::SetFocusObjedct(mDestBone->gameObject);
            }
            if (ImGui::IsItemHovered() && Input::IsMouseUp(Mouse::RIGHT))
            {
                SetDestBone(nullptr);
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
            SetDestBone(receiveObject->transform);
        }
    }
    {
        ImGui::Text("Offset Position : ");
        ImGui::DragFloat3((uid + "Position").c_str(), &mOffsetPosition.x, 0.1f);
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
        }
    }
    {
        ImGui::Text("Offset Scale : ");
        ImGui::DragFloat3((uid + "Scale").c_str(), &mOffsetScale.x, 0.1f);
    }
}

json BoneAttacher::Serialize()
{
    json ret;
    ret["id"] = GetId();
    ret["name"] = "BoneAttacher";
    ret["dest bone"] = mDestBone ? mDestBone->gameObject->GetId() : NULLID;

    ret["position"] = { mOffsetPosition.x, mOffsetPosition.y, mOffsetPosition.z };
    ret["rotation"] = { mOffsetRotation.x, mOffsetRotation.y, mOffsetRotation.z, mOffsetRotation.w };
    ret["scale"] = { mOffsetScale.x, mOffsetScale.y, mOffsetScale.z };
    return ret;
}

void BoneAttacher::Deserialize(json& j)
{
    SetId(j["id"].get<unsigned int>());
    unsigned int boneID = j["dest bone"].get<unsigned int>();
    if (boneID == NULLID)
        mDestBone = nullptr;
    else
        SetDestBone(static_cast<Object*>(Engine::SaveBase::mMap[boneID])->GetComponent<Transform>());

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
}
