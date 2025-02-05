#include "pch.h"
#include "GamePad.h"

float GamePad::mDeadZone = 0.3f;

GamePad::GamePad(PADHANDLE _handle)
    : mHandle(_handle), isConnected(false)
{
    ZeroMemory(&mCurrState, sizeof(XINPUT_STATE));
    ZeroMemory(&mPrevState, sizeof(XINPUT_STATE));
}

void GamePad::Initialize()
{
}

void GamePad::Update()
{
    DWORD result = XInputGetState(mHandle, &mCurrState);

    if (result == ERROR_SUCCESS)
    {
        isConnected = true;
    }
    else
    {
        isConnected = false;
    }

    if (isVibrationTimeLimit)
    {
        mVibrationTime -= Time::GetUnScaledDeltaTime();
        if (isVibrationFade)
        {
            if (mVibrationTime <= 1.0f)
            {
                SetVibration(mLeftVibration * mVibrationTime, mRightVibration * mVibrationTime, mVibrationTime, true);
            }
        }
        if (mVibrationTime <= 0.0f)
        {
            SetVibration(0, 0);
            mVibrationTime = 0.0f;
            isVibrationTimeLimit = false;
        }
    }
}

void GamePad::Reset()
{
    if (isConnected)
    {
        mPrevState = mCurrState;
        ZeroMemory(&mCurrState, sizeof(XINPUT_STATE));
    }
}

bool GamePad::IsButtonDown(WORD button) const
{
    return (mCurrState.Gamepad.wButtons & button) && !(mPrevState.Gamepad.wButtons & button);
}

bool GamePad::IsButtonHold(WORD button) const
{
    return (mCurrState.Gamepad.wButtons & button) && (mPrevState.Gamepad.wButtons & button);
}

bool GamePad::IsButtonUp(WORD button) const
{
    return !(mCurrState.Gamepad.wButtons & button) && (mPrevState.Gamepad.wButtons & button);
}

void GamePad::SetVibration(float _leftMotor, float _rightMotor)
{
    XINPUT_VIBRATION vibration;
    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
    vibration.wLeftMotorSpeed = _leftMotor * 65530;
    vibration.wRightMotorSpeed = _rightMotor * 65530;
    mLeftVibration = _leftMotor;
    mRightVibration = _rightMotor;
    isVibrationTimeLimit = false;
    XInputSetState(mHandle, &vibration);
}

void GamePad::SetVibration(float _leftMotor, float _rightMotor, float _time, bool _isFade)
{
    XINPUT_VIBRATION vibration;
    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
    vibration.wLeftMotorSpeed = _leftMotor * 65530;
    vibration.wRightMotorSpeed = _rightMotor * 65530;
    mLeftVibration = _leftMotor;
    mRightVibration = _rightMotor;
    isVibrationTimeLimit = true;
    isVibrationFade = _isFade;
    mVibrationTime = _time;

    XInputSetState(mHandle, &vibration);
}

float GamePad::GetVibration(int _dir)
{
    if (_dir == LEFT) return mLeftVibration;
    if (_dir == RIGHT) return mRightVibration;
}

Vector2 GamePad::GetStickForce(int _type) const
{
    float xForce = 0.0f, yForce = 0.0f;

    if (_type == LEFT)
    {
        xForce = static_cast<float>(mCurrState.Gamepad.sThumbLX) / 32768.0f;
        yForce = -static_cast<float>(mCurrState.Gamepad.sThumbLY) / 32768.0f;
    }
    else if (_type == RIGHT)
    {
        xForce = static_cast<float>(mCurrState.Gamepad.sThumbRX) / 32768.0f;
        yForce = -static_cast<float>(mCurrState.Gamepad.sThumbRY) / 32768.0f;
    }

    if (std::abs(xForce) < mDeadZone) xForce = 0.0f;
    if (std::abs(yForce) < mDeadZone) yForce = 0.0f;

    return Vector2(xForce, -yForce);
}

float GamePad::GetTriggerForce(int _type) const
{
    float force = 0.0f;
    if (_type == LEFT)
    {
        force = static_cast<float>(mCurrState.Gamepad.bLeftTrigger) / (float)UCHAR_MAX;
    }
    else if (_type == RIGHT)
    {
        force = static_cast<float>(mCurrState.Gamepad.bRightTrigger) / (float)UCHAR_MAX;
    }

    if (std::abs(force) < mDeadZone) force = 0.0f;

    return force;
}
