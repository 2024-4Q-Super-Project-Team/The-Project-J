#include "pch.h"
#include "Input.h"

Key*					Input::mKey = nullptr;
Mouse*					Input::mMouse = nullptr;
std::vector<GamePad*>	Input::mPads;

void Input::Initialize()
{
	mKey = new Key();
	mKey->Initialize();
	mMouse = new Mouse();
	mMouse->Initialize();
	for (int i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		DWORD dwResult = XInputGetState(i, &state);
		if (dwResult != ERROR_SUCCESS) break;

		GamePad* pad = new GamePad(i);
		pad->Update();
		if (pad->IsConnected())
		{
			mPads.push_back(pad);
		}
		else
		{
			delete pad;
		}
	}
}

void Input::Finalization()
{
	SAFE_DELETE(mKey);
	SAFE_DELETE(mMouse);
	SAFE_DELETE_VECTOR(mPads);
}

void Input::Update()
{
    if (mKey)
    {
        mKey->Reset();
        mKey->Update();
    }
    if (mMouse)
    {
        mMouse->Reset();
        mMouse->Update();
    }
	for (auto& pad : mPads)
	{
		pad->Reset();
		pad->Update();
	}
}

bool Input::IsMouseDown(int _mouseType)
{
	return mMouse->IsMouseDown(_mouseType);
}

bool Input::IsMouseHold(int _mouseType)
{
	return mMouse->IsMouseHold(_mouseType);
}

bool Input::IsMouseUp(int _mouseType)
{
	return mMouse->IsMouseUp(_mouseType);
}

Vector2 Input::GetCurrentMousePos()
{
	return mMouse->GetCurrentPosition();
}

Vector2 Input::GetPreviousMousePos()
{
	return mMouse->GetPreviousPosition();
}

Vector2 Input::GetDeltaMousePos()
{
	return mMouse->GetDeltaPosition();
}

SHORT Input::GetWheelDeltaPos()
{
	return mMouse->GetWheelDeltaPosition();
}

void Input::SetMousePosition(Vector2 _position)
{
	mMouse->SetMousePosition(_position);
}

void Input::ShowMouseCursor(bool _isShow)
{
	mMouse->ShowMouseCursor(_isShow);
}

void Input::InputMessegeProcess(MSG _msg)
{
	mMouse->MouseMessegeProcess(_msg);
}
