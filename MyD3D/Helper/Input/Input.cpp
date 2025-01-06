#include "pch.h"
#include "Input.h"

Key* Input::m_key = nullptr;
Mouse* Input::m_mouse = nullptr;

void Input::Initialize()
{
	m_key = new Key();
	m_mouse = new Mouse();
	m_mouse->Initialize();
}

void Input::Update()
{
    if (m_key)
    {
        m_key->Reset();
        m_key->Update();
    }
    if (m_mouse)
    {
        m_mouse->Reset();
        m_mouse->Update();
    }
}

void Input::Reset()
{
	m_key->Reset();
	m_mouse->Reset();
}

bool Input::IsMouseDown(int _mouseType)
{
	return m_mouse->IsMouseDown(_mouseType);
}

bool Input::IsMouseHold(int _mouseType)
{
	return m_mouse->IsMouseHold(_mouseType);
}

bool Input::IsMouseUp(int _mouseType)
{
	return m_mouse->IsMouseUp(_mouseType);
}

Vector2 Input::GetCurrentMousePos()
{
	return m_mouse->GetCurrentPosition();
}

Vector2 Input::GetPreviousMousePos()
{
	return m_mouse->GetPreviousPosition();
}

Vector2 Input::GetDeltaMousePos()
{
	return m_mouse->GetDeltaPosition();
}

SHORT Input::GetWheelDeltaPos()
{
	return m_mouse->GetWheelDeltaPosition();
}

void Input::SetMousePosition(Vector2 _position)
{
	m_mouse->SetMousePosition(_position);
}

void Input::ShowMouseCursor(bool _isShow)
{
	m_mouse->ShowMouseCursor(_isShow);
}

void Input::InputMessegeProcess(MSG _msg)
{
	m_mouse->MouseMessegeProcess(_msg);
}
