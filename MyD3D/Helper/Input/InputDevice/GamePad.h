#pragma once
#include "InputDevice.h"

class GamePad : public InputDevice::InputDeviceBase
{
    using PADHANDLE = UINT;
    enum class StickType;
    enum class TriggerType;
public:
    GamePad(PADHANDLE _handle);
    virtual ~GamePad() {}
public:
    virtual void Initialize();
    virtual void Update();
    virtual void Reset();
public:
    bool IsConnected() const { return isConnected; }
    bool IsButtonDown(WORD _button) const;
    bool IsButtonHold(WORD _button) const;
    bool IsButtonUp(WORD _button) const;
    Vector2 GetStickForce(int _type) const;
    float GetTriggerForce(int _type) const;

    void SetVibration(float _leftMotor, float _rightMotor);
    void SetVibration(float _leftMotor, float _rightMotor, float _time, bool _isFade = false);
    float GetVibration(int _dir);
    static void SetDeadZone(float _value) { mDeadZone = _value; }

private:
    PADHANDLE mHandle;
    XINPUT_STATE mCurrState;
    XINPUT_STATE mPrevState;
    bool isConnected;
    static float mDeadZone;

    float m_leftVibration = 0.0f;
    float m_rightVibration = 0.0f;
    bool  isVibrationTimeLimit = false;
    bool  isVibrationFade = false;
    float m_vibrationTime = 0.0f;

public:

    enum ButtonType
    {
        A = XINPUT_GAMEPAD_A,
        B = XINPUT_GAMEPAD_B,
        X = XINPUT_GAMEPAD_X,
        Y = XINPUT_GAMEPAD_Y,
        D_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
        D_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
        D_UP = XINPUT_GAMEPAD_DPAD_UP,
        D_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
        LEFT_THUMB = XINPUT_GAMEPAD_LEFT_THUMB,
        RIGHT_THUMB = XINPUT_GAMEPAD_RIGHT_THUMB,
        LEFT_SHOULD = XINPUT_GAMEPAD_LEFT_SHOULDER,
        RIGHT_SHOULD = XINPUT_GAMEPAD_RIGHT_SHOULDER,
        START = XINPUT_GAMEPAD_START,
        BACK = XINPUT_GAMEPAD_BACK
    };

    enum PadDirection
    {
        LEFT = -1,
        RIGHT = 1,
    };
};

