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

json Transform::Serialize()
{
    json ret;
    ret["id"] = GetId();
    ret["name"] = "Transform";
 
    ret["position"] = { position.x, position.y, position.z };
    ret["rotation"] = { rotation.x, rotation.y, rotation.z, rotation.w };
    ret["scale"] = { scale.x, scale.y, scale.z };
    ret["root parent"] = mRootParent ? mRootParent->GetId() : NULLID;
    ret["parent"] = mParent ? mParent->GetId() : NULLID;
    //mchildren�� deserialize �� �� parent ���� �̿��ؼ� �־��ֱ� (SetParent())

    //������Ʈ�� id�� ��� �ֱ� ������, ������Ʈ�� ��� ������ �� ���� 
    //�����ʹ� ���� �������� �־��ش�.
    //������Ʈ �׷� - ������Ʈ - ������Ʈ����  �̷��� ���� (1) -> ���� ������ȭ
    //������Ʈ ��� {id, ������} �̷��� ���� (2) -> ���߿� ������ȭ. id�� ������Ʈ ã�Ƽ� ������ �־���.
    return ret;
}

void Transform::Deserialize(json& j)
{
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
    else;
        //TODO: id�� Transform ã�Ƽ� �ִ´�. 

    unsigned int parentId = j["parent"].get<unsigned int>();
    if (parentId == NULLID)
        mRootParent = nullptr;
    else;
        //TODO: id�� Transform ã�Ƽ� �ִ´�. 
}

void Transform::EditorRendering(EditorViewerType _viewerType)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    if (ImGui::TreeNodeEx(("Transform" + uid).c_str(), EDITOR_FLAG_MAIN))
    {
        {
            ImGui::Text("Position : ");
            ImGui::DragFloat3((uid + "Position").c_str(), &position.x, 0.1f);
        }
        {
            Vector3 Euler = GetEulerAngles();
            ImGui::Text("Rotation : ");
            bool isTrigger =
                ImGui::DragFloat3((uid + "Rotation").c_str(), &Euler.x, 0.1f);
            if (isTrigger)
            {
                SetEulerAnglesFromRadian(Euler);
            }
        }
        {
            ImGui::Text("Scale : ");
            ImGui::DragFloat3((uid + "Scale").c_str(), &scale.x, 0.1f);
        }
        EDITOR_COLOR_EXTRA;
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
        ImGui::TreePop();
    }
}