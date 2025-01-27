#include "pch.h"
#include "CameraController.h"

void CameraController::Start()
{
	mCamera = FindObjectWithName(L"Main_Camera")->GetComponent<Camera>();
	//mCamera->SetSkyBox(pSkyBox);
	mCamera->gameObject->AddComponent<AudioListener>();

	mTr = mCamera->gameObject->transform;
	mMoveSpeed.val = 100.0f;

	mTr->position = Vector3(0, 100, -100);

	CurrentAngles[Yaw] = Degree::ToRadian(180);
	CurrentAngles[Pitch] = 0;
	CurrentAngles[Roll] = 0;

	CurrentSubCameraAngles[Yaw] = Degree::ToRadian(180);
	CurrentSubCameraAngles[Pitch] = 0;
	CurrentSubCameraAngles[Roll] = 0;

	auto SubCamera = CreateObject(L"Sub_Camera", L"Default");
	SubCamera->transform->position = Vector3(0, 100, -100);
	mSubCamera = SubCamera->AddComponent<Camera>();
	mSubCamera->SetCameraSize(Vector2(0.3f, 0.3f));
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
		deltaAngleX = Input::GetDeltaMousePos().x * sensitivity.val * Time::GetScaledDeltaTime();
		deltaAngleY = -Input::GetDeltaMousePos().y * sensitivity.val * Time::GetScaledDeltaTime();
		if (Input::IsKeyHold(Key::LCONTROL))
		{
			CurrentSubCameraAngles[Yaw] += deltaAngleX;
			CurrentSubCameraAngles[Pitch] += deltaAngleY;
			CurrentSubCameraAngles[Pitch] = Clamp(CurrentSubCameraAngles[Pitch], -maxYAngle.val, (FLOAT)maxYAngle.val);
		}
		else
		{
			CurrentAngles[Yaw] += deltaAngleX;
			CurrentAngles[Pitch] += deltaAngleY;
			CurrentAngles[Pitch] = Clamp(CurrentAngles[Pitch], -maxYAngle.val, (FLOAT)maxYAngle.val);
		}
	}
	else
	{
		Input::ShowMouseCursor(true);
	}
	if (Input::IsKeyHold('Q'))
	{
		if (Input::IsKeyHold(Key::LCONTROL))
		{
			mSubCamera->gameObject->transform->position += mMoveSpeed.val * mSubCamera->gameObject->transform->Up() * Time::GetScaledDeltaTime() * 10.0f;
		}
		else
			mTr->position += mMoveSpeed.val * mTr->Up() * Time::GetScaledDeltaTime();
	}
	if (Input::IsKeyHold('E'))
	{
		if (Input::IsKeyHold(Key::LCONTROL))
		{
			mSubCamera->gameObject->transform->position -= mMoveSpeed.val * mSubCamera->gameObject->transform->Up() * Time::GetScaledDeltaTime() * 10.0f;
		}
		else
			mTr->position -= mMoveSpeed.val * mTr->Up() * Time::GetScaledDeltaTime();
	}
	if (Input::IsKeyHold('W'))
	{
		if (Input::IsKeyHold(Key::LCONTROL))
		{
			mSubCamera->gameObject->transform->position += mMoveSpeed.val * mSubCamera->gameObject->transform->Forward() * Time::GetScaledDeltaTime() * 10.0f;
		}
		else
			mTr->position += mMoveSpeed.val * mTr->Forward() * Time::GetScaledDeltaTime();
	}
	if (Input::IsKeyHold('S'))
	{
		if (Input::IsKeyHold(Key::LCONTROL))
		{
			mSubCamera->gameObject->transform->position -= mMoveSpeed.val * mSubCamera->gameObject->transform->Forward() * Time::GetScaledDeltaTime() * 10.0f;
		}
		else
			mTr->position -= mMoveSpeed.val * mTr->Forward() * Time::GetScaledDeltaTime();
	}
	if (Input::IsKeyHold('A'))
	{
		if (Input::IsKeyHold(Key::LCONTROL))
		{
			mSubCamera->gameObject->transform->position += mMoveSpeed.val * mSubCamera->gameObject->transform->Right() * Time::GetScaledDeltaTime() * 10.0f;
		}
		else
			mTr->position += mMoveSpeed.val * mTr->Right() * Time::GetScaledDeltaTime();
	}
	if (Input::IsKeyHold('D'))
	{
		if (Input::IsKeyHold(Key::LCONTROL))
		{
			mSubCamera->gameObject->transform->position -= mMoveSpeed.val * mSubCamera->gameObject->transform->Right() * Time::GetScaledDeltaTime() * 10.0f;
		}
		else
			mTr->position -= mMoveSpeed.val * mTr->Right() * Time::GetScaledDeltaTime();
	}
	if (Input::GetWheelDeltaPos() != 0)
	{
		if (Input::IsKeyHold(Key::LCONTROL))
		{
			mSubCamera->gameObject->transform->position += Input::GetWheelDeltaPos() * mSubCamera->gameObject->transform->Forward() * Time::GetScaledDeltaTime() * 10.0f;
		}
		else
		{
			mTr->position += Input::GetWheelDeltaPos() * mTr->Forward() * Time::GetScaledDeltaTime() * 10.0f;
		}
	}

	mTr->rotation
		= DirectX::XMQuaternionRotationRollPitchYaw(
			CurrentAngles[Pitch],
			CurrentAngles[Yaw],
			CurrentAngles[Roll]
		);

	mSubCamera->gameObject->transform->rotation
		= DirectX::XMQuaternionRotationRollPitchYaw(
			CurrentSubCameraAngles[Pitch],
			CurrentSubCameraAngles[Yaw],
			CurrentSubCameraAngles[Roll]
		);
}

json CameraController::Serialize()
{
	json ret;

	ret["id"] = GiveId();
	ret["name"] = "CameraController";
	ret["speed"] = mMoveSpeed.val;
	ret["sensitivity"] = sensitivity.val;
	ret["max angle"] = maxYAngle.val;

	ret["cur angle"] = { CurrentAngles[0], CurrentAngles[1], CurrentAngles[2] };
	ret["cur sub angle"] = { CurrentSubCameraAngles[0], CurrentSubCameraAngles[1], CurrentSubCameraAngles[2] };

	return ret;
}

void CameraController::Deserialize(json& j)
{
	SetId(j["id"].get<unsigned int>());

	mMoveSpeed.val = j["speed"].get<float>();
	sensitivity.val = j["sensitivity"].get<float>();
	maxYAngle.val = j["max angle"].get<float>();

	for (int i = 0; i < 3; i++)
	{
		CurrentAngles[i] = j["cur angle"][i].get<float>();
		CurrentSubCameraAngles[i] = j["cur sub angle"][i].get<float>();
	}
}
