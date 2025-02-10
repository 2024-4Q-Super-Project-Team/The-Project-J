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
    SetCurrentAnimation(mActiveAnimationHandle);
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
    if (mActiveAnimation)
    {
        if (isPlaying)
        {
            float AnimCount = Time::GetScaledDeltaTime() *
                mActiveAnimation->GetFramePerSecond() *
                mFrameRateScale;
            float TotalFrame = mActiveAnimation->GetTotalFrame();
            // TotalFrame이 0이면 무한루프를 돌아버리므로 예외처리
            if (TotalFrame <= 0.0f) return;
            if (isReverse == FALSE)
            {
                mDuration += AnimCount;
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
            }
            else if (isReverse == TRUE)
            {
                mDuration -= AnimCount;
                while (mDuration < 0)
                {
                    if (isLoop == TRUE)
                    {
                        mDuration += TotalFrame;
                    }
                    else
                    {
                        mDuration = 0.0f;
                    }
                }
            }
        }
        CalculateAnimationTramsform(gameObject->transform);
        if (mBlendAnimation)
        {
            mBlendElapsed += Time::GetScaledDeltaTime();

            if (mBlendElapsed > mBlendTime)
            {
                mBlendAnimation = nullptr;
                mBlendElapsed = 0.0f;
            }
        }
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

void Animator::EditorGlobalUpdate()
{
    for (auto& [key, handle] : mAnimationTable)
    {
        gameObject->GetOwnerWorld()->
            mNeedResourceHandleTable.insert(handle.GetParentkey());
    }
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
        mDuration = 0.0f;
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

void Animator::SetCurrentAnimation(std::wstring _key, float _blendTime)
{
    if (mActiveAnimationKey == _key)
        return;
    auto itr = mAnimationTable.find(_key);
    if (FIND_SUCCESS(itr, mAnimationTable))
    {
        mBlendTime = _blendTime;
        mBlendDuration = mDuration;
        mBlendElapsed = 0.0f;
        if (_blendTime == 0.0f)
        {
            
            mBlendAnimation = nullptr;
        }
        else
        {
            mBlendAnimation = mActiveAnimation;
        }
       
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
    ret["is reverse"] = isReverse;
    ret["position"] = { mOffsetPosition.x, mOffsetPosition.y, mOffsetPosition.z };
    ret["rotation"] = { mOffsetRotation.x, mOffsetRotation.y, mOffsetRotation.z, mOffsetRotation.w };
    ret["scale"] = { mOffsetScale.x, mOffsetScale.y, mOffsetScale.z };

    json tableJson = json::array(); // JSON 배열로 초기화

    for (auto& anim : mAnimationTable)
    {
        json entry;
        entry["key"] = Helper::ToString(anim.first);
        entry["handle"] = anim.second.Serialize();

        tableJson.push_back(entry); // 배열에 추가
    }

    ret["table"] = tableJson; // 배열을 최종 JSON 객체에 추가

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
        isPlaying = j["is playing"].get<BOOL>();
    if (j.contains("is loop"))
        isLoop = j["is loop"].get<BOOL>();
    if (j.contains("is reverse"))
        isReverse = j["is reverse"].get<BOOL>();

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
    auto pActiveChannel = mActiveAnimation->GetChannel(_pBone->gameObject->GetName());
   
    if (pActiveChannel)
    {
        Vector3     AnimPosition;
        Quaternion  AnimQuaternion;
        Vector3     AnimScaling;

        AnimPosition = CalculateAnimationPosition(pActiveChannel, mDuration);
        AnimQuaternion = CalculateAnimationRotation(pActiveChannel, mDuration);
        AnimScaling = CalculateAnimationScaling(pActiveChannel, mDuration);
        
        if (mBlendAnimation)
        {
            auto pBlendChannel = mBlendAnimation->GetChannel(_pBone->gameObject->GetName());
            if (pBlendChannel)
            {
                Vector3     BlendPosition = CalculateAnimationPosition(pBlendChannel, mBlendDuration);
                Quaternion  BlendQuaternion = CalculateAnimationRotation(pBlendChannel, mBlendDuration);
                Vector3     BlendScaling = CalculateAnimationScaling(pBlendChannel, mBlendDuration);

                FLOAT BlendFactor = Clamp(mBlendElapsed / mBlendTime, 0.0f, 1.0f);

                AnimPosition = Vector3::Lerp(BlendPosition, AnimPosition, BlendFactor);
                AnimQuaternion = Quaternion::Slerp(BlendQuaternion, AnimQuaternion, BlendFactor);
                AnimScaling = Vector3::Lerp(BlendScaling, AnimScaling, BlendFactor);
            }
        }
        _pBone->position = AnimPosition;
        _pBone->rotation = AnimQuaternion;
        _pBone->scale    = AnimScaling;
    }
    for (auto child : _pBone->GetChildren())
    {
        CalculateAnimationTramsform(child);
    }
}

Vector3 Animator::CalculateAnimationPosition(AnimationNode* _pChannel, FLOAT _duration)
{
    if (_pChannel)
    {
        Vector3 Position = Vector3::Zero;
        int     NextFrame = 1;
        int     CurrFrame = 0;
        while (_pChannel->mPositionKeys[NextFrame].Time < _duration)
        {
            ++NextFrame;
        }
        CurrFrame = NextFrame - 1;
        float FrameRatio = (_duration - _pChannel->mPositionKeys[CurrFrame].Time) /
            (_pChannel->mPositionKeys[NextFrame].Time - _pChannel->mPositionKeys[CurrFrame].Time);

        Position = Vector3::Lerp(
            _pChannel->mPositionKeys[CurrFrame].Value,
            _pChannel->mPositionKeys[NextFrame].Value,
            FrameRatio);

        return Position;
    }
    return Vector3::Zero;
}

Quaternion Animator::CalculateAnimationRotation(AnimationNode* _pChannel, FLOAT _duration)
{
    if (_pChannel)
    {
        Quaternion Rotation = Quaternion::Identity;
        int     NextFrame = 1;
        int     CurrFrame = 0;
        while (_pChannel->mRotationKeys[NextFrame].Time < _duration)
        {
            ++NextFrame;
        }
        CurrFrame = NextFrame - 1;
        float FrameRatio = (_duration - _pChannel->mRotationKeys[CurrFrame].Time) /
            (_pChannel->mRotationKeys[NextFrame].Time - _pChannel->mRotationKeys[CurrFrame].Time);

        Rotation = Quaternion::Lerp(
            _pChannel->mRotationKeys[CurrFrame].Value,
            _pChannel->mRotationKeys[NextFrame].Value,
            FrameRatio);

        return Rotation;
    }
    return Quaternion::Identity;
}

Vector3 Animator::CalculateAnimationScaling(AnimationNode* _pChannel, FLOAT _duration)
{
    if (_pChannel)
    {
        Vector3 Scaling = Vector3::Zero;
        int     NextFrame = 1;
        int     CurrFrame = 0;
        while (_pChannel->mScalingKeys[NextFrame].Time < _duration)
        {
            ++NextFrame;
        }
        CurrFrame = NextFrame - 1;
        float FrameRatio = (_duration - _pChannel->mScalingKeys[CurrFrame].Time) /
            (_pChannel->mScalingKeys[NextFrame].Time - _pChannel->mScalingKeys[CurrFrame].Time);

        Scaling = Vector3::Lerp(
            _pChannel->mScalingKeys[CurrFrame].Value,
            _pChannel->mScalingKeys[NextFrame].Value,
            FrameRatio);

        return Scaling;
    }
    return Vector3::One;
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
        }
        {
            ImGui::Text("Blending Animation");
            std::string widgetID = "NULL Animation";
            if (mBlendAnimation)
            {
                mBlendAnimation->EditorRendering(EditorViewerType::DEFAULT);
                widgetID = mBlendAnimation->GetEID();
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL);
                ImGui::Selectable(widgetID.c_str(), false, ImGuiSelectableFlags_Highlight);
                EDITOR_COLOR_POP(1);
            }
        }
        ImGui::Separator();
        {
            ImGui::Text("Animation List");
            for (auto itr = mAnimationTable.begin(); itr != mAnimationTable.end();)
            {
                bool isDelete = false;
                const std::wstring& key = itr->first;
                const ResourceHandle& handle = itr->second;
                std::string str = Helper::ToString(key) + " : " + Helper::ToString(handle.GetKey());
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_RESOURCE);
                ImGui::Selectable((str).c_str(), false, ImGuiSelectableFlags_Highlight);
                EDITOR_COLOR_POP(1);
                if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
                {
                    ImGui::OpenPopup(("AnimatorPopup" + Helper::ToString(key)).c_str());
                }
                if (ImGui::BeginPopup(("AnimatorPopup" + Helper::ToString(key)).c_str()))
                {
                    if (ImGui::MenuItem("Set Active Animation")) {
                        SetCurrentAnimation(key);
                    }
                    if (ImGui::MenuItem("Delete Animation")) {
                        isDelete = true;
                        itr = mAnimationTable.erase(itr);
                    }
                    ImGui::EndPopup();
                }
                if(!isDelete)
                    ++itr;
            }
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_ADDABLE);
            ImGui::Selectable("Add Animation", false, ImGuiSelectableFlags_Highlight);
            EDITOR_COLOR_POP(1);
            if (EditorDragNDrop::ReceiveDragAndDropResourceData<AnimationResource>("Add Animation", &receiveHandle))
            {
                isAddAnimatorPopup = true;
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
            ImGui::Text("FrameRateScale : ");
            ImGui::DragFloat((uid + "FrameRateScale").c_str(), &mFrameRateScale, 0.01f, 0.0f, 1.0f);
        }
        ImGui::Separator();
        {
            ImGui::Separator();
            ImGui::Text("Duration : %f", mDuration);
        }
        ImGui::Separator();
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
    ShowAddAnimationPopup();
}

void Animator::ShowAddAnimationPopup()
{
    std::string id = "Add Animation";
    if (isAddAnimatorPopup)
    {
        ImGui::OpenPopup(id.c_str());
        isAddAnimatorPopup = false;
    }
    if (ImGui::BeginPopupModal(id.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Key : ");
        static char Name[128] = "";
        ImGui::InputText("##AddAnimationKey", Name, IM_ARRAYSIZE(Name));
        ImGui::Separator();

        ImGui::Text(std::string("Animation : " + Helper::ToString(receiveHandle.GetKey())).c_str());
        ImGui::Separator();

        const char* defaultName = "";
        if (ImGui::Button(("OK##" + id).c_str()) || Input::IsKeyDown(Key::ENTER))
        {
            std::wstring newName = Helper::ToWString(std::string(Name));
            AddAnimation(newName, receiveHandle);
            ImGui::CloseCurrentPopup();
            strcpy_s(Name, defaultName);
        }
        ImGui::SameLine();
        if (ImGui::Button(("NO##" + id).c_str()) || Input::IsKeyDown(Key::ESCAPE))
        {
            ImGui::CloseCurrentPopup();
            strcpy_s(Name, defaultName);
        }
        ImGui::EndPopup();
    }
}

