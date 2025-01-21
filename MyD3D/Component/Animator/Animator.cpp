#include "pch.h"
#include "Animator.h"
#include "Object/Object.h"
#include "Resource/ResourceManager.h"


Animator::Animator(Object* _owner)
    : Component(_owner)
    , isPlaying(true)
{
    mType = eComponentType::ANIMAITOR;
}

Animator::~Animator()
{
}

void Animator::Start()
{
}

void Animator::Tick()
{
}

void Animator::FixedUpdate()
{
}

void Animator::PreUpdate()
{
}

void Animator::Update()
{
}

void Animator::PostUpdate()
{
}

void Animator::PreRender()
{
}

void Animator::Render()
{
    if (isPlaying && mActiveAnimation)
    {
        mDuration += Time::GetScaledDeltaTime() * mActiveAnimation->GetFramePerSecond();
        float TotalFrame = mActiveAnimation->GetTotalFrame();
        while (mDuration >= TotalFrame)
        {
            mDuration -= TotalFrame;
        }
        CalculateAnimationTramsform(gameObject->transform);
    }
}

void Animator::Draw(Camera* _camera)
{
}

void Animator::PostRender()
{
}

void Animator::EditorUpdate()
{
}

void Animator::EditorRender()
{
}

void Animator::SetAnimation(ResourceHandle _handle)
{
    if (_handle.GetResourceType() == eResourceType::AnimationResource)
    {
        mAnimationHandle = _handle;
        auto pResource = ResourceManager::GetResource<AnimationResource>(_handle);
        if (pResource)
        {
            mActiveAnimation = pResource;
        }
    }
}

void Animator::SetAnimation(AnimationResource* _pAnim)
{
    if (_pAnim)
    {
        mActiveAnimation = _pAnim;
    }
}

json Animator::Serialize()
{
    json ret;
    ret["id"] = GetId();
    ret["name"] = "Animator";

    ret["active animation"] = mActiveAnimation->GetKey();

    return ret;
}

void Animator::Deserialize(json& j)
{
}

void Animator::CalculateAnimationTramsform(Transform* _pBone)
{
    auto pChannel = mActiveAnimation->GetChannel(_pBone->gameObject->GetName());
    if (pChannel)
    {
        _pBone->position = CalculateAnimationPosition(pChannel);
        _pBone->rotation = CalculateAnimationRotation(pChannel);
        _pBone->scale    = CalculateAnimationScaling(pChannel);
    }
    for (auto child : _pBone->GetChildren())
    {
        CalculateAnimationTramsform(child);
    }
}

Vector3 Animator::CalculateAnimationPosition(AnimationNode* _pChannel)
{
    Vector3 Position = Vector3::Zero;
    int     NextFrame = 1;
    int     CurrFrame = 0;
    while (_pChannel->mPositionKeys[NextFrame].Time < mDuration)
    {
        ++NextFrame;
    }
    CurrFrame = NextFrame - 1;
    float FrameRatio = (mDuration - _pChannel->mPositionKeys[CurrFrame].Time) /
        (_pChannel->mPositionKeys[NextFrame].Time - _pChannel->mPositionKeys[CurrFrame].Time);

    Position = Vector3::Lerp(
        _pChannel->mPositionKeys[CurrFrame].Value,
        _pChannel->mPositionKeys[NextFrame].Value,
        FrameRatio);

    return Position;
}

Quaternion Animator::CalculateAnimationRotation(AnimationNode* _pChannel)
{
    Quaternion Rotation = Quaternion::Identity;
    int     NextFrame = 1;
    int     CurrFrame = 0;
    while (_pChannel->mRotationKeys[NextFrame].Time < mDuration)
    {
        ++NextFrame;
    }
    CurrFrame = NextFrame - 1;
    float FrameRatio = (mDuration - _pChannel->mRotationKeys[CurrFrame].Time) /
        (_pChannel->mRotationKeys[NextFrame].Time - _pChannel->mRotationKeys[CurrFrame].Time);

    Rotation = Quaternion::Lerp(
        _pChannel->mRotationKeys[CurrFrame].Value,
        _pChannel->mRotationKeys[NextFrame].Value,
        FrameRatio);

    return Rotation;

}

Vector3 Animator::CalculateAnimationScaling(AnimationNode* _pChannel)
{
    Vector3 Scaling = Vector3::Zero;
    int     NextFrame = 1;
    int     CurrFrame = 0;
    while (_pChannel->mScalingKeys[NextFrame].Time < mDuration)
    {
        ++NextFrame;
    }
    CurrFrame = NextFrame - 1;
    float FrameRatio = (mDuration - _pChannel->mScalingKeys[CurrFrame].Time) /
        (_pChannel->mScalingKeys[NextFrame].Time - _pChannel->mScalingKeys[CurrFrame].Time);

    Scaling = Vector3::Lerp(
        _pChannel->mScalingKeys[CurrFrame].Value,
        _pChannel->mScalingKeys[NextFrame].Value,
        FrameRatio);

    return Scaling;
}

void Animator::EditorRendering(EditorViewerType _viewerType)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
	if (ImGui::TreeNodeEx(("Animator" + uid).c_str(), EDITOR_FLAG_MAIN))
	{
        //////////////////////////////////////////////////////////////////////
        // Mesh
        //////////////////////////////////////////////////////////////////////
        {
            std::string widgetID = "NULL Animation";
            std::string name = "NULL Animation";
            if (mActiveAnimation)
            {
                mActiveAnimation->EditorRendering(EditorViewerType::DEFAULT);
                name = Helper::ToString(mActiveAnimation->GetKey());
                widgetID = mActiveAnimation->GetID();
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL);
                ImGui::Selectable(widgetID.c_str(), false, ImGuiSelectableFlags_Highlight);
                EDITOR_COLOR_POP(1);
            }
            if (EditorDragNDrop::ReceiveDragAndDropResourceData<AnimationResource>(widgetID.c_str(), &mAnimationHandle))
            {
                SetAnimation(mAnimationHandle);
            }
        }
        ImGui::Separator();
        ImGui::Text("Duration : %f", mDuration);
		
		ImGui::TreePop();
	}
}

