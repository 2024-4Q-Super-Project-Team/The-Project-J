#pragma once

class PlayerScript;

class PlayerManager
{
public:
	static void SetPlayerInfo(PlayerScript* _player);
	static PlayerScript* GetPlayerInfo(INT _handle);
private:
	static PlayerScript* Player[2];
};

