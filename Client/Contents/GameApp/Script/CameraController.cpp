#include "pch.h"
#include "CameraController.h"

CameraController::~CameraController()
{
}

void CameraController::Start()
{
	mCamera = FindObject(L"Main_Camera", L"Default")->GetComponent<Camera>();
	mTr = mCamera->gameObject->transform;
	mMoveSpeed = 100.0f;

	mTr->position = Vector3(0, 100, -100);

	CurrentAngles[Yaw] = Degree::ToRadian(180);
	CurrentAngles[Pitch] = 0;
	CurrentAngles[Roll] = 0;
	//CurrentAngles[Roll] = Degree(180);
}

void CameraController::Update()
{
	Vector3 InputVector = Vector3::Zero;
	if (Input::IsMouseHold(Mouse::RIGHT) || Input::IsMouseHold(Mouse::MID))
	{
		Input::ShowMouseCursor(false);
		// 각도를 라디안으로 계산 (Pitch, Yaw)
		Radian deltaAngleX;
		Radian deltaAngleY;
		deltaAngleX = Input::GetDeltaMousePos().x * sensitivity;
		deltaAngleY = -Input::GetDeltaMousePos().y * sensitivity;
		CurrentAngles[Yaw] += deltaAngleX;
		CurrentAngles[Pitch] += deltaAngleY;
		CurrentAngles[Pitch] = Clamp(CurrentAngles[Pitch], -maxYAngle, maxYAngle);
	}
	else
	{
		Input::ShowMouseCursor(true);
	}
	if (Input::IsKeyHold('Q'))
	{
		mTr->position += mMoveSpeed * mTr->Up() * Time::GetScaledDeltaTime();
	}
	if (Input::IsKeyHold('E'))
	{
		mTr->position -= mMoveSpeed * mTr->Up() * Time::GetScaledDeltaTime();
	}
	if (Input::IsKeyHold('W'))
	{
		mTr->position += mMoveSpeed * mTr->Forward() * Time::GetScaledDeltaTime();
	}
	if (Input::IsKeyHold('S'))
	{
		mTr->position -= mMoveSpeed * mTr->Forward() * Time::GetScaledDeltaTime();
	}
	if (Input::IsKeyHold('A'))
	{
		mTr->position += mMoveSpeed * mTr->Right() * Time::GetScaledDeltaTime();
	}
	if (Input::IsKeyHold('D'))
	{
		mTr->position -= mMoveSpeed * mTr->Right() * Time::GetScaledDeltaTime();
	}
	if (Input::GetWheelDeltaPos() != 0)
	{
		mTr->position += Input::GetWheelDeltaPos() * mTr->Forward() * Time::GetScaledDeltaTime() * 10.0f;
	}
	mTr->rotation = DirectX::XMQuaternionRotationRollPitchYaw(CurrentAngles[Pitch], CurrentAngles[Yaw], CurrentAngles[Roll]);
}
