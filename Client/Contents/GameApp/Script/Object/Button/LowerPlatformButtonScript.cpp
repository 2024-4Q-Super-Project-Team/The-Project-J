#include "pch.h"
#include "LowerPlatformButtonScript.h"
#include "LowerPlatformCollisionScript.h"

#define PLAYER_SFX_WOOD                 L"SFX_wood.mp3"

LowerPlatformButtonScript::LowerPlatformButtonScript(Object* _owner)
    : ButtonScript(_owner), platform(nullptr)
{
}

void LowerPlatformButtonScript::Start()
{
    AudioInit();

    auto& children = gameObject->transform->GetChildren();
    for (auto& child : children)
    {
        if (child->gameObject->GetName() == L"ButtonObject")
            mButtonObject = child->gameObject;
        if (child->gameObject->GetName() == L"LowerWallPlate")
            mLowerPlatformObject = child->gameObject;
    }

    {
        mButtonCollisionScript = mButtonObject->GetComponent<LowerPlatformCollisionScript>();
        if (mButtonCollisionScript == nullptr)
            mButtonCollisionScript = mButtonObject->AddComponent<LowerPlatformCollisionScript>();
        mButtonCollisionScript->SetOwnerScript(this);
    }

    ResourceHandle SoundHandle;
    SoundHandle.mResourceType = eResourceType::AudioResource;
    SoundHandle.mMainKey = PLAYER_SFX_WOOD;
    SoundHandle.mPath = L"resource/sound/" + std::wstring(PLAYER_SFX_WOOD);
    if (ResourceManager::GetResource<AudioResource>(SoundHandle) == nullptr)
    {
        ResourceManager::LoadFileFromHandle(SoundHandle);
    }
    mAudioSource->AddAudio(PLAYER_SFX_WOOD, SoundHandle);
}

void LowerPlatformButtonScript::Update()
{
    if (!mRotated && isCollided)
    {
        Transform* tr = mLowerPlatformObject->transform;
        tr->RotateTo(2.f, Quaternion::CreateFromYawPitchRoll(0, 0, -1.6f));
        mRotated = true;
        mAudioSource->Play(PLAYER_SFX_WOOD);
    }  
}

void LowerPlatformButtonScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
}

void _CALLBACK LowerPlatformButtonScript::OnCollisionEnterCallback(Rigidbody* _origin, Rigidbody* _destination)
{
    isCollided = true;
}

void LowerPlatformButtonScript::RotatePlatform()
{
    //if (mLowerPlatformObject)
    //{
    //    Transform* tr = mLowerPlatformObject->transform;
    //    tr->RotateTo(2.f, Quaternion::CreateFromYawPitchRoll(0, 0, 80));
    //}
}