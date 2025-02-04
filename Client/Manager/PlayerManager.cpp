#include "pch.h"
#include "PlayerManager.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"

PlayerScript* PlayerManager::Player[2] = { nullptr, nullptr };

void PlayerManager::SetPlayerInfo(PlayerScript* _player)
{
    if (_player)
    {
        Player[_player->GetPlayerHandle()] = _player;
    }
}

PlayerScript* PlayerManager::GetPlayerInfo(INT _handle)
{
    return Player[_handle];
}
