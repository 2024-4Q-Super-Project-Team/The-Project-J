#include "pch.h"
#include "SavePointScript.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"
#include "Manager/SavePointManager.h"

void SavePointScript::Start()
{
    gameObject->AddComponent<Rigidbody>();

    BoxCollider* boxCol = gameObject->AddComponent<BoxCollider>();
    boxCol->SetExtents(Vector3(80, 10, 80));
    boxCol->SetTrigger(true);

    // 사운드 관련
    mAudioSource = gameObject->AddComponent<AudioSource>();
    ResourceHandle ButtonSoundHandle;
    ButtonSoundHandle.mResourceType = eResourceType::AudioResource;
    ButtonSoundHandle.mMainKey = L"SFX_save";
    ButtonSoundHandle.mPath = L"resource/sound/SFX_save.mp3";
    if (ResourceManager::GetResource<AudioResource>(ButtonSoundHandle) == nullptr)
    {
        ResourceManager::LoadFileFromHandle(ButtonSoundHandle);
    }
    mAudioSource->AddAudio(L"save_success", ButtonSoundHandle);
}

void SavePointScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{
    PlayerScript* player = _destination->gameObject->GetComponent<PlayerScript>();

    // 체력 회복
    player->SetHP(player->GetMaxHpValue());
    mAudioSource->Play(L"save_success");
    if (player && !mIsSaved)
    {
        // 세이브 좌표 저장
        SavePointManager::GetInstance().AddSavePoint(this);
        mIsSaved = true; 
        Vector3 coord = this->GetSavePointPosition();
        Display::Console::Log("saved! coord : ",coord.x, " ", coord.y, " ", coord.z, "\n");
    }
}
