#include "pch.h"
#include "Mouse.h"

void Mouse::Initialize()
{
}

void Mouse::Update()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(GetFocus(), &pt);

	m_curState.Position = { static_cast<float>(pt.x) , static_cast<float>(pt.y) };
	m_curState.Left = (GetKeyState(VK_LBUTTON) & 0x8000) != 0;
	m_curState.Right = (GetKeyState(VK_RBUTTON) & 0x8000) != 0;
	m_curState.Mid = (GetKeyState(VK_MBUTTON) & 0x8000) != 0;
}

void Mouse::Reset()
{
	m_preState = m_curState;
	m_curState.Wheel_zDelta = 0;
}

bool Mouse::IsMouseDown(int _mouseType)
{
	if (_mouseType == RIGHT && !m_preState.Right)	 return m_curState.Right;
	else if (_mouseType == MID && !m_preState.Mid)	 return m_curState.Mid;
	else if (_mouseType == LEFT && !m_preState.Left) return m_curState.Left;
	return false;
}

bool Mouse::IsMouseHold(int _mouseType)
{
	if (_mouseType == RIGHT)	 return m_curState.Right;
	else if (_mouseType == MID)	 return m_curState.Mid;
	else if (_mouseType == LEFT) return m_curState.Left;
	return false;
}

bool Mouse::IsMouseUp(int _mouseType)
{
	if (_mouseType == RIGHT && !m_curState.Right && m_preState.Right) return true;
	if (_mouseType == MID && !m_curState.Mid && m_preState.Mid) return true;
	if (_mouseType == LEFT && !m_curState.Left && m_preState.Left) return true;
	return false;
}

void Mouse::ShowMouseCursor(bool _isShow)
{
	if (isShow != _isShow)
	{
		ShowCursor(_isShow);
		isShow = _isShow;
	}
}

void Mouse::SetMousePosition(Vector2 _position)
{
	POINT pt = {};
	ScreenToClient(GetFocus(), &pt);
	SetCursorPos(pt.x, pt.y);
}

void Mouse::MouseMessegeProcess(MSG _msg)
{
	if (_msg.message == WM_MOUSEWHEEL)
	{
		m_curState.Wheel_zDelta += GET_WHEEL_DELTA_WPARAM(_msg.wParam);
	}
}
