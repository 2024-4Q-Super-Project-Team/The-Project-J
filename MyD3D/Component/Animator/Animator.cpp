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

void Animator::SetAnimation(std::shared_ptr<AnimationResource> _pAnim)
{
    mActiveAnimation = _pAnim;
}

json Animator::Serialize()
{
    json ret;
    ret["id"] = GetId();
    ret["name"] = "Animator";

    ret["active animation"] = mActiveAnimation->GetName();

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

void Animator::EditorRendering()
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
	if (ImGui::TreeNodeEx(("Animator" + uid).c_str(), EDITOR_FLAG_MAIN))
	{
        ImGui::Text("Duration : %f", mDuration);

        ImGui::Separator();

		if (mActiveAnimation)
		{
			
            ImGui::Separator();
            mActiveAnimation->EditorRendering();
		}
		
		ImGui::TreePop();
	}
}

