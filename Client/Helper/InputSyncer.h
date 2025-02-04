#pragma once



class InputSyncer
{
public:
	enum eInputType
	{
		LEFT,
		RIGHT,
		UP,
		DOWN,
		MOVE_FIRE,
		OFF_FIRE,
		JUMP,
		SIZE
	};
	enum ePlayerType
	{
		Player_1,
		Player_2,
	};
public:
	static void Initialize();
	static bool IsKeyDown(UINT _handle, eInputType _type);
	static bool IsKeyHold(UINT _handle, eInputType _type);
	static bool IsKeyUp(UINT _handle, eInputType _type);
	static Vector2 GetMoveDirection(UINT _handle);
private:
	
	static std::unordered_map<eInputType, std::vector<UINT>> mKeySet[2];
	static std::unordered_map<eInputType, std::vector<UINT>> mPadSet;
};