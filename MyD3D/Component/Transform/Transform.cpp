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

    UpdatePxTransform();
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
    for (auto& child : mChildren)
    {
        child->SetParent(nullptr);
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
    if (isMoving)
    {
        moveElapsedTime += Time::GetScaledDeltaTime();
        float t = moveElapsedTime / moveDuration;               // 버튼 누르고 지난 시간 / duration
        UpdateMove(t, easingEffect);                            // t가 1이 될 때까지 회전
        if (moveElapsedTime >= moveDuration)
        {
            isMoving = false;
            moveElapsedTime = 0.0f;
            UpdateMove(1.0f, easingEffect);
        }
    }

    if (isRotating)
    {
        rotationElapsedTime += Time::GetScaledDeltaTime();
        float t = rotationElapsedTime / rotationDuration;           
        UpdateRotation(t, easingEffect);							

        if (rotationElapsedTime >= rotationDuration)
        {
            isRotating = false;                 
            rotationElapsedTime = 0.0f;         
            UpdateRotation(1.0f, easingEffect); 
        }
    }
 
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
	// Dotween 테스트용으로 일단 여기에 돌림. 회전
    if (isRotating)
    {
        rotationElapsedTime += Time::GetScaledDeltaTime();
        float t = rotationElapsedTime / rotationDuration;           // 버튼 누르고 지난 시간 / duration
		UpdateRotation(t, easingEffect);							// t가 1이 될 때까지 회전
        Display::Console::Log(rotationElapsedTime, "\n");

        if (rotationElapsedTime >= rotationDuration)
        {
            isRotating = false;                 // 회전 완료
			rotationElapsedTime = 0.0f;         // 초기화
            UpdateRotation(1.0f, easingEffect); // 최종 위치로 설정
        }
    }

	// 바라보기
    if (isLookingAt)
    {
        rotationElapsedTime += Time::GetScaledDeltaTime();
        float t = rotationElapsedTime / rotationDuration;
        UpdateLookAt(t, easingEffect);
        Display::Console::Log(rotationElapsedTime, "\n");

        if (rotationElapsedTime >= rotationDuration)
        {
            isLookingAt = false;               
            rotationElapsedTime = 0.0f;         
            UpdateLookAt(1.0f, easingEffect);  
        }
    }
}

void Transform::EditorRender()
{
}

void Transform::UpdatePxTransform()
{
    memcpy_s(&mPxWorldTransform.p, sizeof(float) * 3, &GetWorldPosition(), sizeof(float) * 3);
    memcpy_s(&mPxWorldTransform.q, sizeof(float) * 4, &rotation, sizeof(float) * 4);
}

void Transform::UpdateFromPxTransform(PxTransform _pxWorldTransform)
{
    mPxWorldTransform = _pxWorldTransform;

    if (mParent)
    {

        PxMat44 parentWorldMatrix = PxMat44(mParent->mPxWorldTransform);
        PxMat44 parentWorldInverse = parentWorldMatrix.inverseRT();
        PxMat44 pxWorldMatrix = PxMat44(mPxWorldTransform);
        PxMat44 pxLocalMatrix = parentWorldInverse * pxWorldMatrix;
        PxTransform localTransform = PxTransform(pxLocalMatrix);

        memcpy_s(&position, sizeof(float) * 3, &localTransform.p, sizeof(float) * 3);
        memcpy_s(&rotation, sizeof(float) * 4, &localTransform.q, sizeof(float) * 4);
    }
    else
    {
        memcpy_s(&position, sizeof(float) * 3, &mPxWorldTransform.p, sizeof(float) * 3);
        memcpy_s(&rotation, sizeof(float) * 4, &mPxWorldTransform.q, sizeof(float) * 4);
    }
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

Vector3 Transform::LocalToWorld(const Vector3& localPosition) const
{
    return Vector3::Transform(localPosition, mWorldMatrix);
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

void Transform::SetLocalMatrix(Matrix& _matrix)
{
    _matrix.Decompose(scale, rotation, position);
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

    if (j.contains("position") && j["position"].size() == 3)
    {
        position.x = j["position"][0].get<float>();
        position.y = j["position"][1].get<float>();
        position.z = j["position"][2].get<float>();
    }
    if (j.contains("rotation") && j["rotation"].size() == 4)
    {
        rotation.x = j["rotation"][0].get<float>();
        rotation.y = j["rotation"][1].get<float>();
        rotation.z = j["rotation"][2].get<float>();
        rotation.w = j["rotation"][3].get<float>();
    }

    if (j.contains("scale") && j["scale"].size() == 3)
    {
        scale.x = j["scale"][0].get<float>();
        scale.y = j["scale"][1].get<float>();
        scale.z = j["scale"][2].get<float>();
    }

    if (j.contains("root parent"))
    {
        unsigned int rootId = j["root parent"].get<unsigned int>();
        if (rootId == NULLID)
            mRootParent = nullptr;
        else
            mRootParent = static_cast<Object*>(Engine::SaveBase::mMap[rootId])->GetComponent<Transform>();
    }
    else mRootParent = nullptr;

    if (j.contains("parent"))
    {
        unsigned int parentId = j["parent"].get<unsigned int>();
        if (parentId == NULLID)
            mParent = nullptr;
        else
            SetParent(static_cast<Object*>(Engine::SaveBase::mMap[parentId])->GetComponent<Transform>());
    }
    else mParent = nullptr;
   
}

void Transform::EditorRendering(EditorViewerType _viewerType)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    switch (_viewerType)
    {
    case EditorViewerType::DEFAULT:
    {
        std::string ownerName = Helper::ToString(gameObject->GetName());
        ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_RESOURCE);
        auto flags = ImGuiSelectableFlags_AllowDoubleClick;
        if (ImGui::Selectable((GetEID() + ownerName).c_str(), false, flags))
        {
        }
        //EditorItemState state;
        //state.mComponentPtr = this;
        //state.mName = GetEID() + ownerName;
        //EditorDragNDrop::SendDragAndDropData(GetEID(), state);
        EDITOR_COLOR_POP(1);
        break;
    }
    case EditorViewerType::HIERARCHY:
        break;
    case EditorViewerType::INSPECTOR: 
    {
        //////////////////////////////////////////////////
        // Position, Rotation, Scale
        //////////////////////////////////////////////////
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
        ImGui::Separator();
        //////////////////////////////////////////////////
        // Parent, Child
        //////////////////////////////////////////////////
        {
            {
                ImGui::Text("Parent : ");

                std::string widgetID = "NULL Parent";
                Object* receiveParent = nullptr;
                if (GetParent())
                {
                    std::string widgetID = GetParent()->GetEID();
                    GetParent()->EditorRendering(EditorViewerType::DEFAULT);
                }
                else
                {
                    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL);
                    ImGui::Selectable(widgetID.c_str(), false, ImGuiSelectableFlags_Highlight);
                    EDITOR_COLOR_POP(1);
                }
                if (EditorDragNDrop::ReceiveDragAndDropObjectData(widgetID.c_str(), &receiveParent))
                {
                    // 자신과 다를때만 
                    if (receiveParent != gameObject)
                    {
                        SetParent(receiveParent->transform);
                    }
                }
            }
            {
                ImGui::Text("Child : ");
                if (GetChildren().empty() == false)
                {
                    for (Transform* child : GetChildren())
                    {
                        child->EditorRendering(EditorViewerType::DEFAULT);
                    }
                }
                else
                {
                    ImGui::Text("NULL Children");
                }
            }
            
        }
        ImGui::Separator();
        //////////////////////////////////////////////////
        // Matrix
        //////////////////////////////////////////////////
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
        break;
    }
    default:
        break;
    }
}

void Transform::Rotate90(float _duration, Dotween::EasingEffect _easingEffect)
{
    if (isRotating) return; // 이미 회전 중이면 중복 실행 X

    isRotating = true;
    rotationDuration = _duration;
    rotationElapsedTime = 0.0f;
    easingEffect = _easingEffect;

    startRotation = rotation;
    endRotation = startRotation * Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, XM_PIDIV2); // 90도 회전
}

void Transform::RotateByPivot(const Vector3& pivot, const Vector3& axis, float angle, float duration, Dotween::EasingEffect easingEffect)
{
    if (isRotating) return;

    isRotating = true;
    rotationDuration = duration;
    rotationElapsedTime = 0.0f;
    this->easingEffect = easingEffect;

    startRotation = rotation;
    startPosition = position;

    // 피벗 기준으로 회전
    Vector3 direction = position - pivot;
    Quaternion rotationQuat = Quaternion::CreateFromAxisAngle(axis, angle);
    Vector3 rotatedDirection = Vector3::Transform(direction, rotationQuat);
    endPosition = pivot + rotatedDirection;

    endRotation = startRotation * rotationQuat;
}

void Transform::LookAt(const Vector3& targetPosition, float _duration, Dotween::EasingEffect _easingEffect)
{
    if (isRotating) return;

    isRotating = true;
    rotationDuration = _duration;
    rotationElapsedTime = 0.0f;
    easingEffect = _easingEffect;
    startRotation = rotation;

    Vector3 direction = targetPosition - position;
    direction.Normalize();
    endRotation = Quaternion::LookRotation(direction, Vector3::Up);
}

void Transform::MoveTo(const Vector3& targetPosition, float _duration, Dotween::EasingEffect _easingEffect)
{
	if (isMoving) return;
	isMoving = true;
	moveDuration = _duration;
	moveElapsedTime = 0.0f;
	easingEffect = _easingEffect;
	startPosition = position;
	endPosition = targetPosition;
}

void Transform::UpdateRotation(float t, Dotween::EasingEffect easingEffect)
{
    // 현재 회전과 목표 회전 사이 보간
    rotation = Quaternion::Slerp(startRotation, endRotation, Dotween::EasingFunction[static_cast<unsigned int>(easingEffect)](t));
    position = Vector3::Lerp(startPosition, endPosition, Dotween::EasingFunction[static_cast<unsigned int>(easingEffect)](t));
    UpdateMatrix();
}

void Transform::UpdateLookAt(float t, Dotween::EasingEffect easingEffect)
{
    // 현재 회전과 목표 회전 사이 보간
    rotation = Quaternion::Slerp(startRotation, endRotation, Dotween::EasingFunction[static_cast<unsigned int>(easingEffect)](t));
    UpdateMatrix();
}

void Transform::UpdateMove(float t, Dotween::EasingEffect easingEffect)
{
	position = Vector3::Lerp(startPosition, endPosition, Dotween::EasingFunction[static_cast<unsigned int>(easingEffect)](t));
	UpdateMatrix();
}

