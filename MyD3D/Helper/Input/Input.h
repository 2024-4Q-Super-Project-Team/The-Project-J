#pragma once
#include "InputDevice/Key.h"
#include "InputDevice/Mouse.h"
#include "InputDevice/GamePad.h"

class Input
{
public:
	static void Initialize();
	static void Finalization();
	static void Update();

	//===================Keyboard===================
	template<typename T> 
	static bool IsKeyDown(T _keyCode);
	template<typename T> 
	static bool IsKeyHold(T _keyCode);
	template<typename T> 
	static bool IsKeyUp(T _keyCode);

	//===================Mouse===================
	static bool IsMouseDown(int _mouseType);
	static bool IsMouseHold(int _mouseType);
	static bool IsMouseUp(int _mouseType);
	static Vector2 GetCurrentMousePos();
	static Vector2 GetPreviousMousePos();
	static Vector2 GetDeltaMousePos();
	static SHORT   GetWheelDeltaPos();
	static void SetMousePosition(Vector2 _position);
	static void ShowMouseCursor(bool _isShow);

	//===================GamePad===================
	static bool PadIsDown(int button, int _handle = 0);
	static bool PadIsHold(int button, int _handle = 0);
	static bool PadIsUp(int button, int _handle = 0);
	static Vector2 GetPadStickForce(int _type, int _handle = 0);
	static float GetPadTriggerForce(int _type, int _handle = 0);
	// 패드 진동
	static void SetPadVibration(float _leftMotor, float _rightMotor, int _handle = 0);
	static void SetPadVibration(float _leftMotor, float _rightMotor, float _time, bool _isFade, int _handle = 0);
	static float GetPadVibration(int _dir, int _handle = 0);
	static int  ConnectedPadCount() { return (int)mPads.size(); }

	static void InputMessegeProcess(MSG _msg);
private:
	static Key* mKey;
	static Mouse* mMouse;
	static std::vector<GamePad*> mPads;
};

template<typename T>
static bool Input::IsKeyDown(T _keyCode)
{
	return mKey->IsKeyDown(static_cast<int>(_keyCode));
}
template<typename T>
static bool Input::IsKeyHold(T _keyCode)
{
	return mKey->IsKeyHold(static_cast<int>(_keyCode));
}
template<typename T>
static bool Input::IsKeyUp(T _keyCode)
{
	return mKey->IsKeyUp(static_cast<int>(_keyCode));
}