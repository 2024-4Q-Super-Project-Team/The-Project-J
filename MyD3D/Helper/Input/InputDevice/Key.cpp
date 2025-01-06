#include "pch.h"
#include "Key.h"

void Key::Initialize()
{
    memset(m_currentKeyState, 0, sizeof(m_currentKeyState));
    memset(m_previousKeyState, 0, sizeof(m_previousKeyState));
}

void Key::Update()
{
	if (!GetKeyboardState(m_currentKeyState))
	{
		//Window::Console::Log("Warning! (KeyUpdate - false)\n");
	}
}

void Key::Reset()
{
    memcpy(m_previousKeyState, m_currentKeyState, sizeof(m_currentKeyState));
}

bool Key::IsKeyDown(int _code)
{
    return (m_currentKeyState[_code] & 0x80) && !(m_previousKeyState[_code] & 0x80);
}

bool Key::IsKeyHold(int _code)
{
    return (m_currentKeyState[_code] & 0x80) && (m_previousKeyState[_code] & 0x80);
}

bool Key::IsKeyUp(int _code)
{
    return !(m_currentKeyState[_code] & 0x80) && (m_previousKeyState[_code] & 0x80);
}