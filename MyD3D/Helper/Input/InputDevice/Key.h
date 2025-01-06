#pragma once
#include "InputDevice.h"

#define KEYBOARD_MAX 256

class Key : public InputDevice::InputDeviceBase
{
public:
    Key() = default;
    virtual ~Key() = default;

    virtual void Initialize();
    virtual void Update();
    virtual void Reset();

    bool IsKeyDown(int _code);
    bool IsKeyHold(int _code);
    bool IsKeyUp(int _code);
private:
    BYTE m_currentKeyState[256];  // 현재 키 상태 배열
    BYTE m_previousKeyState[256]; // 이전 키 상태 배열
public:
    enum Type // 키보드
    {
        UP = VK_UP,
        DOWN = VK_DOWN,
        LEFT = VK_LEFT,
        RIGHT = VK_RIGHT,
        SPACEBAR = VK_SPACE,
        ESCAPE = 27,
        LSHIFT = 16,
        LCONTROL = 17,
        TAB = 9
    };
};