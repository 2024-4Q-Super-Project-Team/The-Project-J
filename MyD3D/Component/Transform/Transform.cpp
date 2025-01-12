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
// ��ü�� ������ ������ �����ڿ��� å���� �����Ѵ�.
// �� Why? �����ڰ� �̹� ������ ��۸������Ϳ� ������ ���ɼ��� �ֱ� ������.
// ���� ��۸� �����Ϳ� �������� �ʵ��� �����ڰ� �����ϰ� ���� �����ϱ� ����
Transform::~Transform()
{
    // �θ��� �����̳ʿ��� �� �ڽ��� �����ؾ��Ѵ�.
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
    //mchildren�� deserialize �� �� parent ���� �̿��ؼ� �־��ֱ� (SetParent())

    //������Ʈ�� id�� ��� �ֱ� ������, ������Ʈ�� ��� ������ �� ���� 
    //�����ʹ� ���� �������� �־��ش�.
    //������Ʈ �׷� - ������Ʈ - ������Ʈ����  �̷��� ���� (1) -> ���� ������ȭ
    //������Ʈ ��� {id, ������} �̷��� ���� (2) -> ���߿� ������ȭ. id�� ������Ʈ ã�Ƽ� ������ �־���.
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