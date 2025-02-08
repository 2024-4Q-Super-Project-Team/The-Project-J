#include "pch.h"
#include "InputSyncer.h"

std::unordered_map<InputSyncer::eInputType, std::vector<UINT>> InputSyncer::mKeySet[2];
std::unordered_map<InputSyncer::eInputType, std::vector<UINT>> InputSyncer::mPadSet;

void InputSyncer::Initialize()
{
	// 인풋 동기화 초기화
	mKeySet[Player_1][LEFT].push_back((UINT)'A');
	mKeySet[Player_1][RIGHT].push_back((UINT)'D');
	mKeySet[Player_1][UP].push_back((UINT)'W');
	mKeySet[Player_1][DOWN].push_back((UINT)'S');
	mKeySet[Player_1][MOVE_FIRE].push_back('E');
	mKeySet[Player_1][OFF_FIRE].push_back('Q');
	mKeySet[Player_1][JUMP].push_back(Key::SPACEBAR);
	
	mKeySet[Player_2][RIGHT].push_back(Key::RIGHT);
	mKeySet[Player_2][LEFT].push_back(Key::LEFT);
	mKeySet[Player_2][UP].push_back(Key::UP);
	mKeySet[Player_2][DOWN].push_back(Key::DOWN);
	mKeySet[Player_2][MOVE_FIRE].push_back(188); // ,
	mKeySet[Player_2][OFF_FIRE].push_back(190); // .
	mKeySet[Player_2][JUMP].push_back(191); // .
	
	mPadSet[LEFT].push_back(GamePad::D_LEFT);
	mPadSet[RIGHT].push_back(GamePad::D_RIGHT);
	mPadSet[UP].push_back(GamePad::D_UP);
	mPadSet[DOWN].push_back(GamePad::D_DOWN);
	mPadSet[OFF_FIRE].push_back(GamePad::B);
	mPadSet[OFF_FIRE].push_back(GamePad::Y);
	mPadSet[JUMP].push_back(GamePad::A);
	mPadSet[JUMP].push_back(GamePad::X);
}

bool InputSyncer::IsKeyDown(UINT _handle, eInputType _type)
{
	std::vector<UINT>& keyCodes = mKeySet[_handle][_type];
	std::vector<UINT>& padCodes = mPadSet[_type];

	for (auto keycode : keyCodes)
	{
		if (Input::IsKeyDown(keycode))
		{
			return true;
		}
	}
	for (auto padcode : padCodes)
	{
		if (Input::PadIsDown(padcode, _handle))
		{
			return true;
		}
	}
	return false;
}

bool InputSyncer::IsKeyHold(UINT _handle, eInputType _type)
{
	std::vector<UINT>& keyCodes = mKeySet[_handle][_type];
	std::vector<UINT>& padCodes = mPadSet[_type];

	if (_type == MOVE_FIRE)
	{
		if (Input::GetPadTriggerForce(GamePad::RIGHT, _handle) != 0.0f)
		{
			return true;
		}
	} 

	for (auto keycode : keyCodes)
	{
		if (Input::IsKeyHold(keycode))
		{
			Display::Console::Log("Key Hold - handle = ", _handle, ", code : ", keycode, '\n');
			return true;
		}
	}
	for (auto padcode : padCodes)
	{
		if (Input::PadIsHold(padcode, _handle))
		{
			Display::Console::Log("Pad Hold - handle = ", _handle, ", code : ", padcode, '\n');
			return true;
		}
	}
	return false;
}

bool InputSyncer::IsKeyUp(UINT _handle, eInputType _type)
{
	std::vector<UINT>& keyCodes = mKeySet[_handle][_type];
	std::vector<UINT>& padCodes = mPadSet[_type];

	for (auto keycode : keyCodes)
	{
		if (Input::IsKeyUp(keycode))
		{
			return true;
		}
	}
	for (auto padcode : padCodes)
	{
		if (Input::PadIsUp(padcode, _handle))
		{
			return true;
		}
	}
	return false;
}

Vector2 InputSyncer::GetInputDirection(UINT _handle)
{
	Vector2 force = Vector2::Zero;

	// Pad의 왼쪽 조이스틱 우선 확인
	if (Input::GetPadStickForce(GamePad::LEFT, _handle).x != 0 ||
		Input::GetPadStickForce(GamePad::LEFT, _handle).y != 0)
	{
		force = Input::GetPadStickForce(GamePad::LEFT, _handle);
	}
	// PadInput이 없다면 KeyInput확인
	else
	{
		if (IsKeyHold(_handle, LEFT)) force.x -= 1;
		if (IsKeyHold(_handle, RIGHT)) force.x += 1;
		if (IsKeyHold(_handle, UP)) force.y += 1;
		if (IsKeyHold(_handle, DOWN)) force.y -= 1;
	}
	return force;
}
