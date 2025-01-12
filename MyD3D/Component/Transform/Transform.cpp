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
    , rotation(Vector3::Zero)
    , scale(Vector3::One)
{
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

json Transform::Serialize()
{
    json ret;
    ret["id"] = mId;
 
    ret["position"] = { position.x, position.y, position.z };
    ret["rotation"] = { rotation.x, rotation.y, rotation.z, rotation.w };
    ret["scale"] = { scale.x, scale.y, scale.z };
    ret["root parent"] = mRootParent ? mRootParent->mId : NULLID;
    ret["parent"] = mParent ? mParent->mId : NULLID;
    //mchildren은 deserialize 할 때 parent 정보 이용해서 넣어주기 (SetParent())

    //컴포넌트의 id를 들고 있기 때문에, 컴포넌트를 모두 생성만 한 다음 
    //데이터는 따로 마지막에 넣어준다.
    //오브젝트 그룹 - 오브젝트 - 컴포넌트종류  이렇게 파일 (1) -> 먼저 역직렬화
    //컴포넌트 목록 {id, 데이터} 이렇게 파일 (2) -> 나중에 역직렬화. id로 컴포넌트 찾아서 데이터 넣어줌.
    return ret;
}

void Transform::EditorRendering()
{
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

        {
            std::string label("Position");
            ImGui::Text(label.c_str());
            ImGui::SameLine(70);
            label += uid;
            ImGui::DragFloat3(label.c_str(), &position.x, 0.1f);
        }
        {
            Vector3 Euler = GetEulerAngles();
            std::string label("Rotation");
            ImGui::Text(label.c_str());
            ImGui::SameLine(70);
            label += uid;
            bool isTrigger;
            isTrigger = ImGui::DragFloat3(label.c_str(), &Euler.x, 0.1f);
            if (isTrigger)
            {
                SetEulerAnglesFromRadian(Euler);
            }
        }
        {
            std::string label("Scale");
            ImGui::Text(label.c_str());
            ImGui::SameLine(70);
            label += uid;
            ImGui::DragFloat3(label.c_str(), &scale.x, 0.1f);
        }
        if (ImGui::CollapsingHeader(("Matrix" + uid).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            {
                std::string label("Local");
                ImGui::Text(label.c_str());
                Matrix& m = mLocalMatrix;
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
            {
                std::string label("Wolrd");
                ImGui::Text(label.c_str());
                Matrix& m = mWorldMatrix;
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
        /*if (ImGui::CollapsingHeader(("Tree##" + uid).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            if(mParent)
            {
                ImGui::Text("Parent : ");ImGui::SameLine();
                std::string name = Helper::ToString(mParent->gameObject->GetName());
                ImGui::Text((name + uid).c_str());
            }
            if(!mChildren.empty())
            {
                ImGui::NewLine();
                ImGui::Text(("Num Children : " + std::to_string(mChildren.size())).c_str());
                ImGui::Text("Children : ");
                for (auto& child : mChildren)
                {
                    std::string name = Helper::ToString(child->gameObject->GetName());
                    ImGui::Text(name.c_str());
                }
            }
        }*/
    }
}