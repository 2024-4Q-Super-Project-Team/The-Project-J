#pragma once
#include "InputDevice/Key.h"
#include "InputDevice/Mouse.h"
#include "InputDevice/GamePad.h"

class Input
{
public:
	static void Initialize();
	static void Update();
	static void Reset();

	template<typename T> 
	static bool IsKeyDown(T _keyCode);
	template<typename T> 
	static bool IsKeyHold(T _keyCode);
	template<typename T> 
	static bool IsKeyUp(T _keyCode);

	static bool IsMouseDown(int _mouseType);
	static bool IsMouseHold(int _mouseType);
	static bool IsMouseUp(int _mouseType);
	static Vector2 GetCurrentMousePos();
	static Vector2 GetPreviousMousePos();
	static Vector2 GetDeltaMousePos();
	static SHORT   GetWheelDeltaPos();
	static void SetMousePosition(Vector2 _position);
	static void ShowMouseCursor(bool _isShow);

	static void InputMessegeProcess(MSG _msg);
private:
	static Key* m_key;
	static Mouse* m_mouse;
};

template<typename T>
static bool Input::IsKeyDown(T _keyCode)
{
	return m_key->IsKeyDown(static_cast<int>(_keyCode));
}
template<typename T>
static bool Input::IsKeyHold(T _keyCode)
{
	return m_key->IsKeyHold(static_cast<int>(_keyCode));
}
template<typename T>
static bool Input::IsKeyUp(T _keyCode)
{
	return m_key->IsKeyUp(static_cast<int>(_keyCode));
}