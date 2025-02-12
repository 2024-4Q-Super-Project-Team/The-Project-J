#include "pch.h"
#include "SavePointScript.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"
#include "Manager/SavePointManager.h"

std::unordered_map<INT, SavePointScript*> SavePointScript::TotalSavePointTable;

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

    TotalSavePointTable[mIndex.val] = this;
}

void SavePointScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{
    PlayerScript* player = _destination->gameObject->GetComponent<PlayerScript>();
    if (player)
    {
        mAudioSource->Play(L"save_success");
        player->SetHP(player->GetMaxHpValue());
        // 세이브 좌표 저장
        SavePointManager::GetInstance().AddSavePoint(this);
    }
}

json SavePointScript::Serialize()
{
    json ret = MonoBehaviour::Serialize();
    ret["save index"] = mIndex.val;
    return ret;
}

void SavePointScript::Deserialize(json& j)
{
    MonoBehaviour::Deserialize(j);

    if (j.contains("save index"))
    {
        mIndex.val = j["save index"].get<INT>();
    }
}
