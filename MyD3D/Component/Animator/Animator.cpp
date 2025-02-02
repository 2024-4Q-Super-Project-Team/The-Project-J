#include "pch.h"
#include "Animator.h"
#include "World/World.h"
#include "Object/Object.h"
#include "Resource/ResourceManager.h"


Animator::Animator(Object* _owner)
    : Component(_owner)
    , isPlaying(true)
    , isLoop(true)
    , mDuration(0.0f)
    , mFrameRateScale(1.0f)
{
    SetEID("Animator");
    mType = eComponentType::ANIMAITOR;
}

Animator::~Animator()
{
}

void Animator::Start()
{
    mActiveAnimation = ResourceManager::GetResource<AnimationResource>(mActiveAnimationHandle);
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
        mDuration += Time::GetScaledDeltaTime() *
            mActiveAnimation->GetFramePerSecond() *
            mFrameRateScale;
        // 총 프레임을 넘으면 빼준다.
        float TotalFrame = mActiveAnimation->GetTotalFrame();
        while (mDuration > TotalFrame)
        {
            if (isLoop == TRUE)
            {
                mDuration -= TotalFrame;
            }
            else
            {
                mDuration = TotalFrame;
            }
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
    mActiveAnimation = ResourceManager::GetResource<AnimationResource>(mActiveAnimationHandle);
}

void Animator::EditorGlobalUpdate()
{
    gameObject->GetOwnerWorld()->
        mNeedResourceHandleTable.insert(mActiveAnimationHandle.GetParentkey());
}

void Animator::EditorRender()
{
}

void Animator::SetCurrentAnimation(ResourceHandle _handle)
{
    if (_handle.GetResourceType() == eResourceType::AnimationResource)
    {
        auto pResource = ResourceManager::GetResource<AnimationResource>(_handle);
        mActiveAnimation = pResource;
        mActiveAnimationHandle = _handle;
    }
}

void _CALLBACK Animator::OnEnable()
{
    Start();
    return void _CALLBACK();
}

void _CALLBACK Animator::OnDisable()
{
    mActiveAnimation = nullptr;
    return void _CALLBACK();
}

void _CALLBACK Animator::OnDestroy()
{
    return void _CALLBACK();
}

void Animator::AddAnimation(std::wstring _key, ResourceHandle _handle)
{
    auto itr = mAnimationTable.find(_key);
    if (FIND_FAILED(itr, mAnimationTable))
    {
        mAnimationTable[_key] = _handle;
    }
}

void Animator::SetCurrentAnimation(std::wstring _key)
{
    auto itr = mAnimationTable.find(_key);
    if (FIND_SUCCESS(itr, mAnimationTable))
    {
        SetCurrentAnimation(itr->second);
        mActiveAnimationKey = _key;
    }
}

bool Animator::IsPlaying()
{
    return isPlaying == TRUE;
}

bool Animator::IsLoop()
{
    return isLoop == TRUE;
}

bool Animator::IsEnd()
{
    if (mActiveAnimation)
    {
        return mDuration == mActiveAnimation->GetTotalFrame();
    }
    return false;
}

json Animator::Serialize()
{ 
    json ret;
    ret["id"] = GetId();
    ret["name"] = "Animator";
    ret["active animation key"] = Helper::ToString(mActiveAnimationKey);;
    ret["active animation handle"] = mActiveAnimationHandle.Serialize();
    ret["frame rate scale"] = mFrameRateScale;
    ret["is playing"] = isPlaying;
    ret["is loop"] = isLoop;

    json tableJson;
    for (auto& anim : mAnimationTable)
    {
        tableJson["key"] = anim.first;
        tableJson["handle"] = anim.second.Serialize();
    }

    ret["table"] += tableJson;

    return ret;
}

void Animator::Deserialize(json& j)
{
    SetId(j["id"].get<unsigned int>());
    if (j.contains("active animation key"))
        mActiveAnimationKey = Helper::ToWString(j["active animation key"].get<std::string>());
    if(j.contains("active animation handle"))
        mActiveAnimationHandle.Deserialize(j["active animation handle"]);
    if (j.contains("frame rate scale"))
        mFrameRateScale = j["frame rate scale"].get<FLOAT>();
    if (j.contains("is playing"))
        mFrameRateScale = j["is playing"].get<BOOL>();
    if (j.contains("is loop"))
        mFrameRateScale = j["is loop"].get<BOOL>();

    if (j.contains("table"))
    {
        json tableJson = j["table"];
        for (json& animJson : tableJson)
        {
            if (animJson.contains("key"))
            {
                std::wstring key = Helper::ToWString(animJson["key"].get<std::string>());
                if (animJson.contains("handle"))
                {
                    ResourceHandle handle;
                    handle.Deserialize(animJson["handle"]);
                    mAnimationTable[key] = handle;
                }
            }
        }
    }
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
    {
        {
            ImGui::Text("Current Animation");
            std::string widgetID = "NULL Animation";
            if (mActiveAnimation)
            {
                mActiveAnimation->EditorRendering(EditorViewerType::DEFAULT);
                widgetID = mActiveAnimation->GetEID();
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL);
                ImGui::Selectable(widgetID.c_str(), false, ImGuiSelectableFlags_Highlight);
                EDITOR_COLOR_POP(1);
            }
            if (EditorDragNDrop::ReceiveDragAndDropResourceData<AnimationResource>(widgetID.c_str(), &mActiveAnimationHandle))
            {
                SetCurrentAnimation(mActiveAnimationHandle);
            }
        }
        ImGui::Separator();
        {
            ImGui::Text("Animation List");
            for (auto handle : mAnimationTable)
            {
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_ADDABLE);
                ImGui::Selectable(Helper::ToString(handle.second.GetKey()).c_str(), false, ImGuiSelectableFlags_Highlight);
                EDITOR_COLOR_POP(1);
            }
        }
        ImGui::Separator();
        {
            if (ImGui::Checkbox(("IsPlaying" + uid).c_str(), (bool*)&isPlaying))
            {
            }
        }
        ImGui::Separator();
        {
            if (ImGui::Checkbox(("isLoop" + uid).c_str(), (bool*)&isLoop))
            {
            }
        }
        ImGui::Separator();
        {
            ImGui::Separator();
            ImGui::Text("Duration : %f", mDuration);
        }
    }
}

