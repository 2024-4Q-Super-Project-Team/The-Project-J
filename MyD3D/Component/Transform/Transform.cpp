#include "pch.h"
#include "transform.h"
#include "Object/Object.h"

Transform::Transform(Object* _owner)
    : Component(_owner)
    , mRootParent(nullptr)
    , mParent(nullptr)
    , mChildren()
    , mLocalMatrix(Matrix::Identity)
    , mWorldMatrix(Matrix::Identity)
    , position(Vector3::Zero)
    , rotation(Quaternion::Identity)
    , scale(Vector3::One)
{
    SetEID("Transform");
}
// 객체의 계층적 삭제는 소유자에게 책임을 전달한다.
// 왜 Why? 소유자가 이미 삭제한 댕글링포인터에 접근할 가능성이 있기 때문에.
// 따라서 댕글링 포인터에 접근하지 않도록 소유자가 안전하게 직접 삭제하길 권유
Transform::~Transform()
{
    // 부모의 컨테이너에서 내 자신을 제거해야한다.
    if (mParent)
    {
        if (!mParent->mChildren.empty())
        {
            auto itr = FIND_CONTAINER(mParent->mChildren,
                [this](const Transform* _other) {
                    return this == _other;
                });
            if (FIND_SUCCESS(itr, mParent->mChildren))
            {
                mParent->mChildren.erase(itr);
            }
        }
    }
}

void Transform::Start()
{

}

void Transform::Tick()
{
}

void Transform::FixedUpdate()
{
}

void Transform::PreUpdate()
{
}

void Transform::Update()
{
}

void Transform::PostUpdate()
{
}

void Transform::PreRender()
{
}

void Transform::Render()
{
}

void Transform::Draw(Camera* _camera)
{
}

void Transform::PostRender()
{
}

void Transform::EditorUpdate()
{
}

void Transform::EditorRender()
{
}

void Transform::UpdatePxTransform()
{
    memcpy_s(&mPxTransform.p, sizeof(float) * 3, &position, sizeof(float) * 3);
    memcpy_s(&mPxTransform.q, sizeof(float) * 4, &rotation, sizeof(float) * 4);
}

void Transform::UpdateFromPxTransform(PxTransform pxTransform)
{
    mPxTransform = pxTransform;
    PxTransform localTransform = mPxTransform;
    if (mParent)
    {
        PxTransform parentInverse = mParent->mPxTransform.getInverse();
        localTransform = parentInverse.transform(mPxTransform);
    }

    memcpy_s(&position, sizeof(float) * 3, &localTransform.p, sizeof(float) * 3);
    memcpy_s(&rotation, sizeof(float) * 4, &localTransform.q, sizeof(float) * 4);
}

void Transform::Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable)
{
    Transform* clone = _owner->transform;
    clone->position  = this->position;
    clone->rotation  = this->rotation;
    clone->scale     = this->scale;

    if (this->GetParent())
    {
        std::wstring parentName = this->GetParent()->gameObject->GetName();
        auto ppParent =
            Helper::FindMap(parentName, _objTable);
        if (ppParent)
            clone->SetParent((*ppParent)->transform);
    }
}

void Transform::UpdateMatrix()
{
    Matrix tr = DirectX::XMMatrixTranslationFromVector(position);
    Matrix ro = DirectX::XMMatrixRotationQuaternion(rotation);
    Matrix sc = DirectX::XMMatrixScalingFromVector(scale);

    mLocalMatrix = sc * ro * tr;

    if (mParent)
    {
        mWorldMatrix = mLocalMatrix * mParent->mWorldMatrix;
    }
    else
    {
        mWorldMatrix = mLocalMatrix;
    }
    for (auto& child : mChildren)
    {
        child->UpdateMatrix();
    }
}

void Transform::SetParent(Transform* _parent)
{
    if (mParent == _parent) return;
    if (mParent) {
        auto itr = std::find(mParent->mChildren.begin(), mParent->mChildren.end(), this);
        if (itr != mParent->mChildren.end())
            mParent->mChildren.erase(itr);
    }
    if (_parent) {
        _parent->mChildren.push_back(this);
    }
    mParent = _parent;
}

void Transform::SetWorldMatrix(Matrix& _worldMatrix)
{
    _worldMatrix.Decompose(scale, rotation, position);
    UpdateMatrix();
}

json Transform::Serialize()
{
    json ret;
    ret["id"] = GetId();
    ret["name"] = "Transform";
 
    ret["position"] = { position.x, position.y, position.z };
    ret["rotation"] = { rotation.x, rotation.y, rotation.z, rotation.w };
    ret["scale"] = { scale.x, scale.y, scale.z };
    ret["root parent"] = mRootParent ? mRootParent->gameObject->GetId() : NULLID;
    ret["parent"] = mParent ? mParent->gameObject->GetId() : NULLID;

    return ret;
}

void Transform::Deserialize(json& j)
{
    SetId(j["id"].get<unsigned int>());

    position.x = j["position"][0].get<float>();
    position.y = j["position"][1].get<float>();
    position.z = j["position"][2].get<float>();

    rotation.x = j["rotation"][0].get<float>();
    rotation.y = j["rotation"][1].get<float>();
    rotation.z = j["rotation"][2].get<float>();
    rotation.w = j["rotation"][3].get<float>();

    scale.x = j["scale"][0].get<float>();
    scale.y = j["scale"][1].get<float>();
    scale.z = j["scale"][2].get<float>();

    unsigned int rootId = j["root parent"].get<unsigned int>();
    if (rootId == NULLID)
        mRootParent = nullptr;
    else
        mRootParent = static_cast<Object*>(Engine::SaveBase::mMap[rootId])->GetComponent<Transform>();

    unsigned int parentId = j["parent"].get<unsigned int>();
    if (parentId == NULLID)
        mParent = nullptr;
    else
        SetParent(static_cast<Object*>(Engine::SaveBase::mMap[parentId])->GetComponent<Transform>());
}

void Transform::EditorRendering(EditorViewerType _viewerType)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    {
        ImGui::Text("Position : ");
        ImGui::DragFloat3((uid + "Position").c_str(), &position.x, 0.1f);
    }
    {
        // 쿼터니언에서 오일러로
        Vector3 Euler = GetEulerAngles();
        Vector3 EulerDegrees;
        EulerDegrees.x = DirectX::XMConvertToDegrees(Euler.x);
        EulerDegrees.y = DirectX::XMConvertToDegrees(Euler.y);
        EulerDegrees.z = DirectX::XMConvertToDegrees(Euler.z);
        ImGui::Text("Rotation : ");
        bool isTrigger = ImGui::DragFloat3((uid + "Rotation").c_str(), &EulerDegrees.x, 0.1f);
        if (isTrigger)
        {
            Euler.x = DirectX::XMConvertToRadians(EulerDegrees.x);
            Euler.y = DirectX::XMConvertToRadians(EulerDegrees.y);
            Euler.z = DirectX::XMConvertToRadians(EulerDegrees.z);

            // 라디안에서 쿼터니언으로
            SetEulerAngles(Euler);
        }
    }
    {
        ImGui::Text("Scale : ");
        ImGui::DragFloat3((uid + "Scale").c_str(), &scale.x, 0.1f);
    }
    ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_EXTRA);
    if (ImGui::TreeNodeEx(("Matrix" + uid).c_str(), ImGuiTreeNodeFlags_Selected))
    {
        {
            ImGui::Text("Local Matrix");
            Matrix& m = mLocalMatrix;
            {
                ImGui::Text("%.3f", m._11); ImGui::SameLine();
                ImGui::Text("%.3f", m._12); ImGui::SameLine();
                ImGui::Text("%.3f", m._13); ImGui::SameLine();
                ImGui::Text("%.3f", m._14);
                ImGui::Text("%.3f", m._21); ImGui::SameLine();
                ImGui::Text("%.3f", m._22); ImGui::SameLine();
                ImGui::Text("%.3f", m._23); ImGui::SameLine();
                ImGui::Text("%.3f", m._24);
                ImGui::Text("%.3f", m._31); ImGui::SameLine();
                ImGui::Text("%.3f", m._32); ImGui::SameLine();
                ImGui::Text("%.3f", m._33); ImGui::SameLine();
                ImGui::Text("%.3f", m._34);
                ImGui::Text("%.3f", m._41); ImGui::SameLine();
                ImGui::Text("%.3f", m._42); ImGui::SameLine();
                ImGui::Text("%.3f", m._43); ImGui::SameLine();
                ImGui::Text("%.3f", m._44);
            }
        }
        {
            ImGui::Text("Wolrd Matrix");
            Matrix& m = mWorldMatrix;
            {
                ImGui::Text("%.3f", m._11); ImGui::SameLine();
                ImGui::Text("%.3f", m._12); ImGui::SameLine();
                ImGui::Text("%.3f", m._13); ImGui::SameLine();
                ImGui::Text("%.3f", m._14);
                ImGui::Text("%.3f", m._21); ImGui::SameLine();
                ImGui::Text("%.3f", m._22); ImGui::SameLine();
                ImGui::Text("%.3f", m._23); ImGui::SameLine();
                ImGui::Text("%.3f", m._24);
                ImGui::Text("%.3f", m._31); ImGui::SameLine();
                ImGui::Text("%.3f", m._32); ImGui::SameLine();
                ImGui::Text("%.3f", m._33); ImGui::SameLine();
                ImGui::Text("%.3f", m._34);
                ImGui::Text("%.3f", m._41); ImGui::SameLine();
                ImGui::Text("%.3f", m._42); ImGui::SameLine();
                ImGui::Text("%.3f", m._43); ImGui::SameLine();
                ImGui::Text("%.3f", m._44);
            }
        }
        ImGui::TreePop();
    }
    EDITOR_COLOR_POP(1);
}

void Transform::LookAt(Transform* _dest, float duration, Dotween::EasingEffect easingEffect)
{
    Vector3 targetPosition = _dest->GetWorldPosition();
    float elapsedTime = 0.0f;

    while (elapsedTime < duration)
    {
        float t = elapsedTime / duration;
        UpdateLookAt(targetPosition, t, easingEffect);
        elapsedTime += 0.016f; // 60 FPS 기준
    }

    UpdateLookAt(targetPosition, 1.0f, easingEffect);
}

void Transform::UpdateLookAt(const Vector3& targetPosition, float t, Dotween::EasingEffect easingEffect)
{
    Vector3 currentPosition = GetWorldPosition();
    Vector3 direction = targetPosition - currentPosition;
    direction.Normalize();

    // 목표 방향으로 회전하는 Quaternion 계산
    Quaternion targetRotation = Quaternion::CreateFromYawPitchRoll(atan2(direction.x, direction.z), asin(direction.y), 0.0f);

    // 현재 회전과 목표 회전 사이를 보간
    Quaternion currentRotation = rotation;
    rotation = Quaternion::Slerp(currentRotation, targetRotation, Dotween::EasingFunction[static_cast<unsigned int>(easingEffect)](t));

    UpdateMatrix();
}

void Transform::Rotate360(float duration, Dotween::EasingEffect easingEffect)
{
    float elapsedTime = 0.0f;

    // UpdateRotation 함수를 일정 시간 동안 호출하여 부드럽게 회전
    while (elapsedTime < duration)
    {
        float t = elapsedTime / duration;
        UpdateRotation(t, easingEffect);
        elapsedTime += 0.016f;
    }

    // 한 바퀴 회전
    UpdateRotation(1.0f, easingEffect);
}

void Transform::UpdateRotation(float t, Dotween::EasingEffect easingEffect)
{
    // 2PI(360도) 회전
    Quaternion startRotation = rotation;
    Quaternion endRotation = Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, XM_2PI);

    // 현재 회전과 목표 회전 사이 보간
    rotation = Quaternion::Slerp(startRotation, endRotation, Dotween::EasingFunction[static_cast<unsigned int>(easingEffect)](t));

    // 행렬 업데이트
    UpdateMatrix();
}
