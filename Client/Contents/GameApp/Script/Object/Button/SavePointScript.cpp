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
    
}

void SavePointScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{
    PlayerScript* player = _destination->gameObject->GetComponent<PlayerScript>();

    // 체력 회복
    player->SetHP(player->GetMaxHpValue());

    if (player && !mIsSaved)
    {
        // 세이브 좌표 저장
        SavePointManager::GetInstance().AddSavePoint(this);
        mIsSaved = true; 
        Vector3 coord = this->GetSavePointPosition();
        Display::Console::Log("saved! coord : ",coord.x, " ", coord.y, " ", coord.z, "\n");
    }
}
