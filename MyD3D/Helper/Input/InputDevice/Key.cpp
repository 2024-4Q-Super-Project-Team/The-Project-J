#include "pch.h"
#include "Key.h"

std::unordered_map<std::string, Key::Type> Key::keyMap;

void Key::Initialize()
{
    memset(m_currentKeyState, 0, sizeof(m_currentKeyState));
    memset(m_previousKeyState, 0, sizeof(m_previousKeyState));

   keyMap[u8"UP"] = UP;
   keyMap[u8"DOWN"] = DOWN;
   keyMap[u8"LEFT"] = LEFT;
   keyMap[u8"RIGHT"] = RIGHT;
   keyMap[u8"SPACEBAR"] = SPACEBAR;
   keyMap[u8"ESCAPE"] = ESCAPE;
   keyMap[u8"LSHIFT"] = LSHIFT;
   keyMap[u8"LCONTROL"] = LCONTROL;
   keyMap[u8"TAB"] = TAB;
   keyMap[u8"W"] = W;
   keyMap[u8"A"] = A;
   keyMap[u8"S"] = S;
   keyMap[u8"D"] = D;
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