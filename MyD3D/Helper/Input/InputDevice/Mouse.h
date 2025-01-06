#pragma once
#include "InputDevice.h"


class Mouse : public InputDevice::InputDeviceBase
{
public:
	Mouse() = default;
	virtual ~Mouse() = default;

	virtual void Initialize();
	virtual void Update();
	virtual void Reset();

	inline Vector2 GetCurrentPosition() { return m_curState.Position; }
	inline Vector2 GetPreviousPosition() { return m_preState.Position; }
	inline Vector2 GetDeltaPosition() { return m_curState.Position - m_preState.Position; }
	inline SHORT   GetWheelDeltaPosition() { return m_curState.Wheel_zDelta; }

	bool    IsMouseDown(int _mouseType);
	bool    IsMouseHold(int _mouseType);
	bool    IsMouseUp(int _mouseType);

	void	ShowMouseCursor(bool _isShow);
	void    SetMousePosition(Vector2 _position);

	void	MouseMessegeProcess(MSG _msg);
private:
	struct MouseState {
		bool Left = false;
		bool Right = false;
		bool Mid = false;
		Vector2	Position = { 0,0 };
		SHORT Wheel_zDelta = 0;
	};
	bool isShow = true;
	MouseState m_curState;
	MouseState m_preState;
public:
	enum Type // ¸¶¿ì½º
	{
		LEFT = VK_LBUTTON,
		MID = VK_MBUTTON,
		RIGHT = VK_RBUTTON,
	};
};

