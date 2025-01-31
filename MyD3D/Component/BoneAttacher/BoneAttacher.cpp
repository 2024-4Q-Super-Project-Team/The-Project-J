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
}

void BoneAttacher::Draw(Camera* _camera)
{
}

void BoneAttacher::PostRender()
{
}

void BoneAttacher::EditorUpdate()
{
}

void BoneAttacher::EditorRender()
{
}

void BoneAttacher::EditorRendering(EditorViewerType _viewerType)
{

}

json BoneAttacher::Serialize()
{
    json ret;
    ret["id"] = GetId();
    ret["name"] = "BoneAttacher";
    ret["dest bone"] = mDestBone ? mDestBone->gameObject->GetId() : NULLID;

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
}
